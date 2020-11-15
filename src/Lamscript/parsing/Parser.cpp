#include <Lamscript/parsing/Parser.h>

#include <initializer_list>
#include <typeinfo>
#include <vector>
#include <memory>

#include <Lamscript/Lamscript.h>
#include <Lamscript/parsable/Expression.h>
#include <Lamscript/parsable/Statement.h>
#include <Lamscript/parsing/Token.h>
#include <Lamscript/parsing/TokenType.h>

namespace lamscript {
namespace parsing {


// ---------------------------------- PUBLIC -----------------------------------

std::vector<std::unique_ptr<parsable::Statement>> Parser::Parse() {
  std::vector<std::unique_ptr<parsable::Statement>> statements;

  while (!HasReachedEOF()) {
    statements.emplace_back(std::move(ParseDeclaration()));
  }

  return statements;
}

// ---------------------------------- PRIVATE ----------------------------------

Token Parser::Peek() {
  return tokens_.at(current_token_);
}

Token Parser::Previous() {
  return tokens_.at(current_token_ - 1);
}

bool Parser::HasReachedEOF() {
  if (current_token_ == 0) { return false; }
  return Peek().Type == END_OF_FILE;
}

Token Parser::Advance() {
  if (!HasReachedEOF()) {
    current_token_++;
  }

  return Previous();
}

bool Parser::CheckToken(const TokenType& token_type) {
  if (HasReachedEOF()) {
    return false;
  }

  return Peek().Type == token_type;
}

bool Parser::CheckAndConsumeTokens(
    const std::initializer_list<TokenType>& token_types) {
  for (TokenType token_type : token_types) {
    if (CheckToken(token_type)) {
      Advance();
      return true;
    }
  }
  return false;
}

/// By doing so, this should hopefully remove all potential ghost errrors
/// that associated with the current malformed expression.
void Parser::Synchronize() {
  Advance();

  while (!HasReachedEOF()) {
    if (Previous().Type == SEMICOLON) {
      return;
    }

    switch (Peek().Type) {
      case CLASS: break;
      case FUN: break;
      case VAR: break;
      case FOR: break;
      case IF: break;
      case WHILE: break;
      case PRINT: break;
      case RETURN: return;
    }

    Advance();
  }
}

ParseError Parser::Error(Token token, const std::string& message) {
  Lamscript::Error(token, message);
  return ParseError(message.c_str());
}

Token Parser::Consume(TokenType type, const std::string& message) {
  if (CheckToken(type)) {
    return Advance();
  }

  throw Error(Peek(), message);
}

// -------------------------------- STATEMENTS ---------------------------------

/// Synchronizes to the next valid statement when it runs into an invalid
/// statement.
std::unique_ptr<parsable::Statement> Parser::ParseDeclaration() {
  try {
    if (CheckAndConsumeTokens({FUN})) {
      return std::move(ParseFunction("function"));
    }

    if (CheckAndConsumeTokens({VAR})) {
        return std::move(ParseVariableDeclaration());
    }

    return std::move(ParseStatement());
  } catch (ParseError error) {
    Synchronize();
    return nullptr;
  }
}

std::unique_ptr<parsable::Statement> Parser::ParseVariableDeclaration() {
  Token name = Consume(IDENTIFIER, "Expect a variable name.");
  std::unique_ptr<parsable::Expression> initializer = nullptr;

  // Variables can be optionally initialized.
  if (CheckAndConsumeTokens({EQUAL})) {
    initializer = ParseExpression();
  }

  Consume(SEMICOLON, "Expect ';' after variable declaration.");

  std::unique_ptr<parsable::Statement> statement;
  statement.reset(
      new parsable::VariableStatement(name, std::move(initializer)));
  return std::move(statement);
}

std::vector<
    std::unique_ptr<parsable::Statement>> Parser::ParseBlockStatements() {
  std::vector<std::unique_ptr<parsable::Statement>> statements;

  // Check for right side braces without consuming any tokens.
  while (!CheckToken(RIGHT_BRACE) && !HasReachedEOF()) {
    statements.emplace_back(std::move(ParseDeclaration()));
  }

  Consume(RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}

/// This function checks in list order for:
/// * For statements.
/// * If statements.
/// * Print statements.
/// * While statements.
/// * Block statements.
/// * Expression statements.
std::unique_ptr<parsable::Statement> Parser::ParseStatement() {
  if (CheckAndConsumeTokens({FOR})) {
    return std::move(ParseForStatement());
  }

  if (CheckAndConsumeTokens({IF})) {
    return std::move(ParseIfStatement());
  }

  if (CheckAndConsumeTokens({PRINT})) {
    return std::move(ParsePrintStatement());
  }

  if (CheckAndConsumeTokens({WHILE})) {
    return std::move(ParseWhileStatement());
  }

  if (CheckAndConsumeTokens({LEFT_BRACE})) {
    std::unique_ptr<parsable::Statement> statement;
    statement.reset(new parsable::Block(ParseBlockStatements()));
    return std::move(statement);
  }

  return std::move(ParseExpressionStatement());
}

std::unique_ptr<parsable::Statement> Parser::ParsePrintStatement() {
  std::unique_ptr<parsable::Expression> value = ParseExpression();
  Consume(SEMICOLON, "Expect ';' after value.");

  std::unique_ptr<parsable::Statement> statement;
  statement.reset(new parsable::Print(std::move(value)));
  return std::move(statement);
}

std::unique_ptr<parsable::Statement> Parser::ParseExpressionStatement() {
  std::unique_ptr<parsable::Expression> value = ParseExpression();
  Consume(SEMICOLON, "Expect ';' after value.");


  std::unique_ptr<parsable::Statement> statement;
  statement.reset(new parsable::ExpressionStatement(std::move(value)));
  return std::move(statement);
}

std::unique_ptr<parsable::Statement> Parser::ParseIfStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'if'.");
  std::unique_ptr<parsable::Expression> condition = ParseExpression();
  Consume(RIGHT_PAREN, "Expect ')' after if condition.");

  std::unique_ptr<parsable::Statement> then_branch = ParseStatement();
  std::unique_ptr<parsable::Statement> else_branch = nullptr;

  if (CheckAndConsumeTokens({ELSE})) {
    else_branch = ParseStatement();
  }

  std::unique_ptr<parsable::Statement> statement;
  statement.reset(
      new parsable::If(
        std::move(condition), std::move(then_branch), std::move(else_branch)));
  return std::move(statement);
}

std::unique_ptr<parsable::Statement> Parser::ParseWhileStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'while'.");
  std::unique_ptr<parsable::Expression> condition = ParseExpression();
  Consume(RIGHT_PAREN, "Expect ')' after condition.");

  std::unique_ptr<parsable::Statement> body = ParseStatement();

  std::unique_ptr<parsable::Statement> statement;
  statement.reset(new parsable::While(std::move(condition), std::move(body)));

  return std::move(statement);
}

std::unique_ptr<parsable::Statement> Parser::ParseForStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'while'.");
  std::unique_ptr<parsable::Statement> initializer = nullptr;

  // Parse the initializer.
  if (CheckAndConsumeTokens({SEMICOLON})) {
    initializer = nullptr;
  } else if (CheckAndConsumeTokens({VAR})) {
    initializer = ParseVariableDeclaration();
  } else {
    initializer = ParseExpressionStatement();
  }

  // Parse the condition.
  std::unique_ptr<parsable::Expression> condition = nullptr;
  if (!CheckToken(SEMICOLON)) {
    condition = ParseExpression();
  }
  Consume(SEMICOLON, "Expect ';' after loop condition.");

  // Parse the increment.
  std::unique_ptr<parsable::Expression> increment = nullptr;
  if (!CheckToken(RIGHT_PAREN)) {
    increment = ParseExpression();
  }
  Consume(RIGHT_PAREN, "Expect ')' after for clauses.");

  std::unique_ptr<parsable::Statement> body = ParseStatement();

  // If there's an increment, move it into the for loops local scope and
  if (increment != nullptr) {
    std::unique_ptr<parsable::Statement> expression;
    expression.reset(new parsable::ExpressionStatement(std::move(increment)));

    std::vector<std::unique_ptr<parsable::Statement>> statements;
    statements.emplace_back(std::move(body));
    statements.emplace_back(std::move(expression));
    body.reset(new parsable::Block(std::move(statements)));
  }

  if (condition == nullptr) {
    condition.reset(new parsable::Literal(true));
  }

  body.reset(new parsable::While(std::move(condition), std::move(body)));

  if (initializer != nullptr) {
    std::vector<std::unique_ptr<parsable::Statement>> statements;
    statements.emplace_back(std::move(initializer));
    statements.emplace_back(std::move(body));
    body.reset(new parsable::Block(std::move(statements)));
  }

  return std::move(body);
}

std::unique_ptr<parsable::Statement> Parser::ParseFunction(
    const std::string& kind) {
  Token name = Consume(IDENTIFIER, "Expect " + kind + " name.");
  Consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");

  std::vector<Token> parameters;

  // Parse function arguments.
  if (!CheckToken(RIGHT_PAREN)) {
    do {
      if (parameters.size() > 255) {
        Error(Peek(), "Can't have more than 255 parameters.");
      }

      parameters.push_back(Consume(IDENTIFIER, "Expect parameter name."));
    } while (CheckAndConsumeTokens({COMMA}));
  }
  Consume(RIGHT_PAREN, "Expect ')' after parameters.");

  Consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
  std::vector<std::unique_ptr<parsable::Statement>> body =
      ParseBlockStatements();

  std::unique_ptr<parsable::Statement> statement;
  statement.reset(new parsable::Function(name, parameters, std::move(body)));
  return std::move(statement);
}

// -------------------------------- EXPRESSIONS --------------------------------

/// @todo (C3NZ): I need to keep an eye on this just to make sure that the
/// initial pointer expression is being passed into the binary expression and
/// that the binary expression isn't holding a reference to itself after
/// assignment.
std::unique_ptr<parsable::Expression> Parser::ParseEquality() {
  std::unique_ptr<parsable::Expression> expression = ParseComparison();

  while (CheckAndConsumeTokens({BANG_EQUAL, EQUAL_EQUAL})) {
    Token expr_operator = Previous();
    std::unique_ptr<parsable::Expression> right_side = ParseComparison();
    expression.reset(
        new parsable::Binary(
            std::move(expression), expr_operator, std::move(right_side)));
  }

  return std::move(expression);
}

std::unique_ptr<parsable::Expression> Parser::ParseAssignment() {
  std::unique_ptr<parsable::Expression> expression = ParseOr();

  if (CheckAndConsumeTokens({EQUAL})) {
    Token equals = Previous();
    std::unique_ptr<parsable::Expression> value = ParseAssignment();

    if (parsable::Variable* var = dynamic_cast<parsable::Variable*>(
          expression.get())) {
      expression.reset(new parsable::Assign(var->GetName(), std::move(value)));
      return std::move(expression);
    }

    Error(equals, "Ivalid assignment target");
  }

  return std::move(expression);
}

std::unique_ptr<parsable::Expression> Parser::ParseExpression() {
  return std::move(ParseAssignment());
}

/// @brief Parses primary expressions into literals.
std::unique_ptr<parsable::Expression> Parser::ParsePrimary() {
  std::unique_ptr<parsable::Expression> expression;

    if (CheckAndConsumeTokens({FALSE})) {
      expression.reset(new parsable::Literal(false));
      return std::move(expression);
    }

    if (CheckAndConsumeTokens({TRUE})) {
      expression.reset(new parsable::Literal(true));
      return std::move(expression);
    }

    if (CheckAndConsumeTokens({NIL})) {
      expression.reset(new parsable::Literal());
      return std::move(expression);
    }

    if (CheckAndConsumeTokens({NUMBER, STRING})) {
      Token token = Previous();

      if (token.Literal.type() == typeid(double)) {
        expression.reset(new parsable::Literal(
              std::any_cast<double>(token.Literal)));
        return std::move(expression);
      }

      expression.reset(new parsable::Literal(
            std::any_cast<std::string&>(token.Literal)));
      return std::move(expression);
    }

    if (CheckAndConsumeTokens({IDENTIFIER})) {
      expression.reset(new parsable::Variable(Previous()));
      return std::move(expression);
    }

    if (CheckAndConsumeTokens({LEFT_PAREN})) {
      std::unique_ptr<parsable::Expression> expression = ParseExpression();
      Consume(RIGHT_PAREN, "Expect ')' after expression");
      expression.reset(new parsable::Grouping(std::move(expression)));
      return std::move(expression);
    }

    throw Error(Peek(), "Expect expression.");
}

/// Precedence will have ! parsed first and then - afterwards.
std::unique_ptr<parsable::Expression> Parser::ParseUnary() {
  if (CheckAndConsumeTokens({BANG, MINUS})) {
    Token unary_operator = Previous();
    std::unique_ptr<parsable::Expression> right_side = ParseUnary();
    right_side.reset(
        new parsable::Unary(unary_operator, std::move(right_side)));
    return std::move(right_side);
  }

  return std::move(ParseCall());
}

/// Checks for subtraction first and then addition after.
std::unique_ptr<parsable::Expression> Parser::ParseTerm() {
  std::unique_ptr<parsable::Expression> expression = ParseFactor();

  while (CheckAndConsumeTokens({MINUS, PLUS})) {
    Token expr_operator = Previous();
    std::unique_ptr<parsable::Expression> right_side = ParseUnary();
    expression.reset(
        new parsable::Binary(
            std::move(expression), expr_operator, std::move(right_side)));
  }

  return std::move(expression);
}

std::unique_ptr<parsable::Expression> Parser::ParseFactor() {
  std::unique_ptr<parsable::Expression> expression = ParseUnary();

  while (CheckAndConsumeTokens({SLASH, STAR, MODULUS})) {
    Token expr_operator = Previous();
    std::unique_ptr<parsable::Expression> right_side = ParseUnary();
    expression.reset(
        new parsable::Binary(
            std::move(expression), expr_operator, std::move(right_side)));
  }

  return std::move(expression);
}

/// This matches >, >=, <, <= and creates a Binary expression from the
/// result of the parse.
std::unique_ptr<parsable::Expression> Parser::ParseComparison() {
  std::unique_ptr<parsable::Expression> expression = ParseTerm();

  while (CheckAndConsumeTokens({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token expr_operator = Previous();
    std::unique_ptr<parsable::Expression> right_side = ParseTerm();
    expression.reset(
        new parsable::Binary(
            std::move(expression), expr_operator, std::move(right_side)));
  }

  return std::move(expression);
}

std::unique_ptr<parsable::Expression> Parser::ParseOr() {
  std::unique_ptr<parsable::Expression> expression = ParseAnd();

  while (CheckAndConsumeTokens({OR})) {
    Token or_operator = Previous();
    std::unique_ptr<parsable::Expression> right_operand = ParseAnd();
    expression.reset(
        new parsable::Logical(
            std::move(expression), or_operator, std::move(right_operand)));
  }

  return std::move(expression);
}

std::unique_ptr<parsable::Expression> Parser::ParseAnd() {
  std::unique_ptr<parsable::Expression> expression = ParseEquality();

  while (CheckAndConsumeTokens({AND})) {
    Token and_operator = Previous();
    std::unique_ptr<parsable::Expression> right_operand = ParseEquality();
    expression.reset(
        new parsable::Logical(
            std::move(expression), and_operator, std::move(right_operand)));
  }

  return std::move(expression);
}

std::unique_ptr<parsable::Expression> Parser::ParseCall() {
  std::unique_ptr<parsable::Expression> expression = ParsePrimary();

  while (true) {
    if (CheckAndConsumeTokens({LEFT_PAREN})) {
      expression = FinishCall(std::move(expression));
    } else {
      break;
    }
  }

  return std::move(expression);
}

std::unique_ptr<parsable::Expression> Parser::FinishCall(
    std::unique_ptr<parsable::Expression> callee) {
  std::vector<std::unique_ptr<parsable::Expression>> arguments;

  if (!CheckToken(RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255) {
        Error(Peek(), "Can't have more than 255 arguments.");
      }
      arguments.emplace_back(std::move(ParseExpression()));
    } while (CheckAndConsumeTokens({COMMA}));
  }

  Token parent = Consume(RIGHT_PAREN, "Expect ')' after arguments.");

  std::unique_ptr<parsable::Expression> expression;
  expression.reset(
      new parsable::Call(std::move(callee), parent, std::move(arguments)));
  return std::move(expression);
}

}  // namespace parsing
}  // namespace lamscript
