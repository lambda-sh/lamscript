#include <Lamscript/Parser.h>

#include <initializer_list>
#include <typeinfo>
#include <vector>

#include <Lamscript/Expression.h>
#include <Lamscript/Lamscript.h>
#include <Lamscript/Statement.h>
#include <Lamscript/Token.h>
#include <Lamscript/TokenType.h>

namespace lamscript {

// ---------------------------------- PUBLIC -----------------------------------

std::vector<Statement*> Parser::Parse() {
  std::vector<Statement*> statements;

  while (!HasReachedEOF()) {
    statements.push_back(ParseDeclaration());
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

/// @brief validates that the current token matches any given set of types.
bool Parser::CheckTokens(const std::initializer_list<TokenType>& token_types) {
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
Statement* Parser::ParseDeclaration() {
  try {
    if (CheckTokens({VAR})) {
        return ParseVariableDeclaration();
    }

    return ParseStatement();
  } catch (ParseError error) {
    Synchronize();
    return nullptr;
  }
}

Statement* Parser::ParseVariableDeclaration() {
  Token name = Consume(IDENTIFIER, "Expect a variable name.");
  Expression* initializer = nullptr;

  // Variables can be optionally initialized.
  if (CheckTokens({EQUAL})) {
    initializer = ParseExpression();
  }

  Consume(SEMICOLON, "Expect ';' after variable declaration.");
  return new VariableStatement(name, initializer);
}

std::vector<Statement*> Parser::ParseBlockStatements() {
  std::vector<Statement*> statements;

  // Check for right side braces without consuming any tokens.
  while (!CheckToken(RIGHT_BRACE) && !HasReachedEOF()) {
    statements.push_back(ParseDeclaration());
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
Statement* Parser::ParseStatement() {
  if (CheckTokens({FOR})) {
    return ParseForStatement();
  }

  if (CheckTokens({IF})) {
    return ParseIfStatement();
  }

  if (CheckTokens({PRINT})) {
    return ParsePrintStatement();
  }

  if (CheckTokens({WHILE})) {
    return ParseWhileStatement();
  }

  if (CheckTokens({LEFT_BRACE})) {
    return new Block(ParseBlockStatements());
  }

  return ParseExpressionStatement();
}

Statement* Parser::ParsePrintStatement() {
  Expression* value = ParseExpression();
  Consume(SEMICOLON, "Expect ';' after value.");

  return new Print(value);
}

Statement* Parser::ParseExpressionStatement() {
    Expression* value = ParseExpression();
    Consume(SEMICOLON, "Expect ';' after value.");

    return new ExpressionStatement(value);
}


Statement* Parser::ParseIfStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'if'.");
  Expression* condition = ParseExpression();
  Consume(RIGHT_PAREN, "Expect ')' after if condition.");

  Statement* then_branch = ParseStatement();
  Statement* else_branch = nullptr;

  if (CheckTokens({ELSE})) {
    else_branch = ParseStatement();
  }

  return new If(condition, then_branch, else_branch);
}

Statement* Parser::ParseWhileStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'while'.");
  Expression* condition = ParseExpression();
  Consume(RIGHT_PAREN, "Expect ')' after condition.");

  Statement* body = ParseStatement();

  return new While(condition, body);
}

Statement* Parser::ParseForStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'while'.");
  Statement* initializer = nullptr;

  // Parse the initializer.
  if (CheckTokens({SEMICOLON})) {
    initializer = nullptr;
  } else if (CheckTokens({VAR})) {
    initializer = ParseVariableDeclaration();
  } else {
    initializer = ParseExpressionStatement();
  }

  // Parse the condition.
  Expression* condition = nullptr;
  if (!CheckToken(SEMICOLON)) {
    condition = ParseExpression();
  }
  Consume(SEMICOLON, "Expect ';' after loop condition.");

  // Parse the increment.
  Expression* increment = nullptr;
  if (!CheckToken(RIGHT_PAREN)) {
    increment = ParseExpression();
  }
  Consume(RIGHT_PAREN, "Expect ')' after for clauses.");

  Statement* body = ParseStatement();

  // If there's an increment, move it into the for loops local scope and
  if (increment != nullptr) {
    body = new Block(
        std::vector<Statement*>({body, new ExpressionStatement(increment)}));
  }

  if (condition == nullptr) {
    condition = new Literal(true);
  }

  body = new While(condition, body);

  if (initializer != nullptr) {
    body = new Block(std::vector<Statement*>({initializer, body}));
  }

  return body;
}

// -------------------------------- EXPRESSIONS --------------------------------

/// @todo (C3NZ): I need to keep an eye on this just to make sure that the
/// initial pointer expression is being passed into the binary expression and
/// that the binary expression isn't holding a reference to itself after
/// assignment.
Expression* Parser::ParseEquality() {
  Expression* expression = ParseComparison();

  while (CheckTokens({BANG_EQUAL, EQUAL_EQUAL})) {
    Token expr_operator = Previous();
    Expression* right_side = ParseComparison();
    expression = new Binary(expression, expr_operator, right_side);
  }

  return expression;
}

Expression* Parser::ParseAssignment() {
  Expression* expression = ParseOr();

  if (CheckTokens({EQUAL})) {
    Token equals = Previous();
    Expression* value = ParseAssignment();

    if (Variable* var = dynamic_cast<Variable*>(expression)) {
      return new Assign(var->GetName(), value);
    }

    Error(equals, "Ivalid assignment target");
  }

  return expression;
}

Expression* Parser::ParseExpression() {
  return ParseAssignment();
}

/// @brief Parses primary expressions into literals.
Expression* Parser::ParsePrimary() {
    if (CheckTokens({FALSE})) {
      return new Literal(false);
    }

    if (CheckTokens({TRUE})) {
      return new Literal(true);
    }

    if (CheckTokens({NIL})) {
      return new Literal();
    }

    if (CheckTokens({NUMBER, STRING})) {
      Token token = Previous();

      if (token.Literal.type() == typeid(double)) {
        return new Literal(std::any_cast<double>(token.Literal));
      }

      return new Literal(std::any_cast<std::string&>(token.Literal));
    }

    if (CheckTokens({IDENTIFIER})) {
      return new Variable(Previous());
    }

    if (CheckTokens({LEFT_PAREN})) {
      Expression* expression = ParseExpression();
      Consume(RIGHT_PAREN, "Expect ')' after expression");
      return new Grouping(expression);
    }

    throw Error(Peek(), "Expect expression.");
}

/// Precedence will have ! parsed first and then - afterwards.
Expression* Parser::ParseUnary() {
  if (CheckTokens({BANG, MINUS})) {
    Token unary_operator = Previous();
    Expression* right_side = ParseUnary();
    return new Unary(unary_operator, right_side);
  }

  return ParseCall();
}

/// Checks for subtraction first and then addition after.
Expression* Parser::ParseTerm() {
  Expression* expression = ParseFactor();

  while (CheckTokens({MINUS, PLUS})) {
    Token expr_operator = Previous();
    Expression* right_side = ParseUnary();
    expression = new Binary(expression, expr_operator, right_side);
  }

  return expression;
}

Expression* Parser::ParseFactor() {
  Expression* expression = ParseUnary();

  while (CheckTokens({SLASH, STAR, MODULUS})) {
    Token expr_operator = Previous();
    Expression* right_side = ParseUnary();
    expression = new Binary(expression, expr_operator, right_side);
  }

  return expression;
}

/// This matches >, >=, <, <= and creates a Binary expression from the
/// result of the parse.
Expression* Parser::ParseComparison() {
  Expression* expression = ParseTerm();

  while (CheckTokens({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token expr_operator = Previous();
    Expression* right_side = ParseTerm();
    expression = new Binary(expression, expr_operator, right_side);
  }

  return expression;
}

Expression* Parser::ParseOr() {
  Expression* expression = ParseAnd();

  while (CheckTokens({OR})) {
    Token or_operator = Previous();
    Expression* right_operand = ParseAnd();
    expression = new Logical(expression, or_operator, right_operand);
  }

  return expression;
}

Expression* Parser::ParseAnd() {
  Expression* expression = ParseEquality();

  while (CheckTokens({AND})) {
    Token and_operator = Previous();
    Expression* right_operand = ParseEquality();
    expression = new Logical(expression, and_operator, right_operand);
  }

  return expression;
}

Expression* Parser::ParseCall() {
  Expression* expression = ParsePrimary();

  while (true) {
    if (CheckTokens({LEFT_PAREN})) {
      expression = FinishCall(expression);
    } else {
      break;
    }
  }

  return expression;
}

Expression* Parser::FinishCall(Expression* callee) {
  std::vector<Expression*> arguments;

  if (!CheckToken(RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255) {
        Error(Peek(), "Can't have more than 255 arguments.");
      }
      arguments.push_back(ParseExpression());
    } while (CheckTokens({COMMA}));
  }

  Token parent = Consume(RIGHT_PAREN, "Expect ')' after arguments.");

  return new Call(callee, parent, arguments);
}

}  // namespace lamscript
