#include <Lamscript/parsing/Parser.h>

#include <initializer_list>
#include <typeinfo>
#include <vector>
#include <memory>

#include <Lamscript/errors/ParseError.h>
#include <Lamscript/parsed/Expression.h>
#include <Lamscript/parsed/Statement.h>
#include <Lamscript/parsing/Token.h>
#include <Lamscript/parsing/TokenType.h>
#include <Lamscript/runtime/Lamscript.h>

namespace lamscript {
namespace parsing {

namespace {
  typedef std::unique_ptr<parsed::Expression> UniqueExpression;
  typedef std::unique_ptr<parsed::Statement> UniqueStatement;
  typedef std::shared_ptr<parsed::Function> SharedFunction;

}  // namespace

// ---------------------------------- PUBLIC -----------------------------------

std::vector<UniqueStatement> Parser::Parse() {
  std::vector<UniqueStatement> statements;

  while (!HasReachedEOF()) {
    statements.emplace_back(std::move(ParseDeclaration()));
  }

  return statements;
}

std::string GenerateRandomString(size_t length) {
  auto RandomCharacter = []() -> char {
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
  };

  std::string random_string(length, 0);
  std::generate_n(random_string.begin(), length, RandomCharacter);
  return random_string;
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
  runtime::Lamscript::Error(token, message);
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
UniqueStatement Parser::ParseDeclaration() {
  try {
    if (CheckAndConsumeTokens({CLASS})) {
      return std::move(ParseClassStatement());
    }

    if (CheckAndConsumeTokens({FUN})) {
        return std::move(ParseFunction("function"));
    }

    if (CheckAndConsumeTokens({VAR})) {
        return std::move(ParseVariableDeclaration());
    }

    return std::move(ParseStatement());
  } catch (const ParseError& error) {
    Synchronize();
    return nullptr;
  }
}

UniqueStatement Parser::ParseVariableDeclaration() {
  Token name = Consume(IDENTIFIER, "Expect a variable name.");
  UniqueExpression initializer = nullptr;

  // Variables can be optionally initialized.
  if (CheckAndConsumeTokens({EQUAL})) {
    initializer = ParseExpression();
  }

  Consume(SEMICOLON, "Expect ';' after variable declaration.");

  UniqueStatement statement;
  statement.reset(
      new parsed::VariableStatement(name, std::move(initializer)));
  return std::move(statement);
}

std::vector<
    std::unique_ptr<parsed::Statement>> Parser::ParseBlockStatements() {
  std::vector<std::unique_ptr<parsed::Statement>> statements;

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
UniqueStatement Parser::ParseStatement() {
  if (CheckAndConsumeTokens({FOR})) {
    return std::move(ParseForStatement());
  }

  if (CheckAndConsumeTokens({IF})) {
    return std::move(ParseIfStatement());
  }

  if (CheckAndConsumeTokens({PRINT})) {
    return std::move(ParsePrintStatement());
  }

  if (CheckAndConsumeTokens({RETURN})) {
    return std::move(ParseReturnStatement());
  }

  if (CheckAndConsumeTokens({WHILE})) {
    return std::move(ParseWhileStatement());
  }

  if (CheckAndConsumeTokens({LEFT_BRACE})) {
    UniqueStatement block;
    block.reset(new parsed::Block(std::move(ParseBlockStatements())));
    return std::move(block);
  }

  return std::move(ParseExpressionStatement());
}

UniqueStatement Parser::ParsePrintStatement() {
  UniqueExpression value = ParseExpression();
  Consume(SEMICOLON, "Expect ';' after value.");

  UniqueStatement print;
  print.reset(new parsed::Print(std::move(value)));
  return std::move(print);
}

UniqueStatement Parser::ParseExpressionStatement() {
  UniqueExpression value = ParseExpression();
  Consume(SEMICOLON, "Expect ';' after value.");


  UniqueStatement expr_statement;
  expr_statement.reset(new parsed::ExpressionStatement(std::move(value)));
  return std::move(expr_statement);
}

UniqueStatement Parser::ParseIfStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'if'.");
  UniqueExpression condition = ParseExpression();
  Consume(RIGHT_PAREN, "Expect ')' after if condition.");

  UniqueStatement then_branch = ParseStatement();
  UniqueStatement else_branch = nullptr;

  if (CheckAndConsumeTokens({ELSE})) {
    else_branch = ParseStatement();
  }

  UniqueStatement if_statement;
  if_statement.reset(
      new parsed::If(
        std::move(condition), std::move(then_branch), std::move(else_branch)));
  return std::move(if_statement);
}

UniqueStatement Parser::ParseWhileStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'while'.");
  UniqueExpression condition = ParseExpression();
  Consume(RIGHT_PAREN, "Expect ')' after condition.");

  UniqueStatement body = ParseStatement();
  UniqueStatement while_statement;

  while_statement.reset(
      new parsed::While(std::move(condition), std::move(body)));

  return std::move(while_statement);
}

UniqueStatement Parser::ParseForStatement() {
  Consume(LEFT_PAREN, "Expect '(' after 'while'.");
  UniqueStatement initializer = nullptr;

  // Parse the initializer.
  if (CheckAndConsumeTokens({SEMICOLON})) {
    initializer = nullptr;
  } else if (CheckAndConsumeTokens({VAR})) {
    initializer = ParseVariableDeclaration();
  } else {
    initializer = ParseExpressionStatement();
  }

  // Parse the condition.
  UniqueExpression condition = nullptr;
  if (!CheckToken(SEMICOLON)) {
    condition = ParseExpression();
  }
  Consume(SEMICOLON, "Expect ';' after loop condition.");

  // Parse the increment.
  UniqueExpression increment = nullptr;
  if (!CheckToken(RIGHT_PAREN)) {
    increment = ParseExpression();
  }
  Consume(RIGHT_PAREN, "Expect ')' after for clauses.");

  UniqueStatement body = ParseStatement();

  // If there's an increment, move it into the for loops local scope and
  if (increment != nullptr) {
    UniqueStatement expression;
    expression.reset(new parsed::ExpressionStatement(std::move(increment)));

    std::vector<UniqueStatement> statements;
    statements.emplace_back(std::move(body));
    statements.emplace_back(std::move(expression));

    body.reset(new parsed::Block(std::move(statements)));
  }

  if (condition == nullptr) {
    condition.reset(new parsed::Literal(true));
  }

  body.reset(new parsed::While(std::move(condition), std::move(body)));

  if (initializer != nullptr) {
    std::vector<UniqueStatement> statements;
    statements.emplace_back(std::move(initializer));
    statements.emplace_back(std::move(body));
    body.reset(new parsed::Block(std::move(statements)));
  }

  return std::move(body);
}

UniqueStatement Parser::ParseFunction(const std::string& kind) {
  Token name{FUN, "lambda" + GenerateRandomString(8), nullptr,  Peek().Line };
  if (kind.compare("function") == 0 || kind.compare("method") == 0) {
    name = Consume(IDENTIFIER, "Expect " + kind + " name.");
  }

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
  std::vector<std::unique_ptr<parsed::Statement>> body =
      ParseBlockStatements();

  UniqueStatement func_statement;
  func_statement.reset(new parsed::Function(name, parameters, std::move(body)));
  return std::move(func_statement);
}

UniqueStatement Parser::ParseReturnStatement() {
  Token keyword = Previous();
  UniqueExpression value = nullptr;

  if (!CheckToken(SEMICOLON)) {
    value = std::move(ParseExpression());
  }

  Consume(SEMICOLON, "Expect ';' after return value.");

  UniqueStatement return_statement;
  return_statement.reset(new parsed::Return(keyword, std::move(value)));
  return std::move(return_statement);
}

UniqueStatement Parser::ParseClassStatement() {
  Token name = Consume(IDENTIFIER, "Expect a class name.");
  Consume(LEFT_BRACE, "Expect '{' before class body.");

  std::vector<SharedFunction> methods;

  while (!CheckToken(RIGHT_BRACE) && !HasReachedEOF()) {
    SharedFunction class_method;
    class_method.reset(
        static_cast<parsed::Function*>(ParseFunction("method").release()));
    methods.emplace_back(class_method);
  }

  Consume(RIGHT_BRACE, "Expect '}' after class body.");

  UniqueStatement class_def;
  class_def.reset(new parsed::Class(name, nullptr, methods));
  return std::move(class_def);
}

// -------------------------------- EXPRESSIONS --------------------------------

/// @todo (C3NZ): I need to keep an eye on this just to make sure that the
/// initial pointer expression is being passed into the binary expression and
/// that the binary expression isn't holding a reference to itself after
/// assignmen
UniqueExpression Parser::ParseEquality() {
  UniqueExpression expression = ParseComparison();

  while (CheckAndConsumeTokens({BANG_EQUAL, EQUAL_EQUAL})) {
    Token expr_operator = Previous();
    UniqueExpression right_side = ParseComparison();
    expression.reset(
        new parsed::Binary(
            std::move(expression), expr_operator, std::move(right_side)));
  }

  return std::move(expression);
}

UniqueExpression Parser::ParseAssignment() {
  UniqueExpression expression = ParseOr();

  if (CheckAndConsumeTokens({EQUAL})) {
    Token equals = Previous();
    UniqueExpression value = ParseAssignment();

    if (parsed::Variable* var = dynamic_cast<parsed::Variable*>(
          expression.get())) {
      expression.reset(new parsed::Assign(var->GetName(), std::move(value)));
      return std::move(expression);
    } else if (
        parsed::Get* get = dynamic_cast<parsed::Get*>(expression.get())) {
      expression.reset(
          new parsed::Set(
              get->GetObject(), get->GetName(), std::move(value)));
    } else {
      Error(equals, "Invalid assignment target");
    }
  }

  return std::move(expression);
}

UniqueExpression Parser::ParseExpression() {
  return std::move(ParseAssignment());
}

/// @brief Parses primary expressions into literals.
UniqueExpression Parser::ParsePrimary() {
  UniqueExpression expression;

  if (CheckAndConsumeTokens({FALSE})) {
    expression.reset(new parsed::Literal(false));
    return std::move(expression);
  }

  if (CheckAndConsumeTokens({TRUE})) {
    expression.reset(new parsed::Literal(true));
    return std::move(expression);
  }

  if (CheckAndConsumeTokens({NIL})) {
    expression.reset(new parsed::Literal());
    return std::move(expression);
  }

  if (CheckAndConsumeTokens({NUMBER, STRING})) {
    Token token = Previous();

    if (token.Literal.type() == typeid(double)) {
      expression.reset(new parsed::Literal(
            std::any_cast<double>(token.Literal)));
      return std::move(expression);
    }

    expression.reset(new parsed::Literal(
          std::any_cast<std::string&>(token.Literal)));
    return std::move(expression);
  }

  if (CheckAndConsumeTokens({THIS})) {
    expression.reset(new parsed::This(Previous()));
    return std::move(expression);
  }

  if (CheckAndConsumeTokens({IDENTIFIER})) {
    expression.reset(new parsed::Variable(Previous()));
    return std::move(expression);
  }

  if (CheckAndConsumeTokens({FUN})) {
    UniqueStatement lambda_func = std::move(
        ParseFunction("lambda"));
    expression.reset(new parsed::LambdaExpression(std::move(lambda_func)));
    return std::move(expression);
  }

  if (CheckAndConsumeTokens({LEFT_PAREN})) {
    UniqueExpression grouping = ParseExpression();
    Consume(RIGHT_PAREN, "Expect ')' after expression");
    grouping.reset(new parsed::Grouping(std::move(grouping)));
    return std::move(grouping);
  }

  throw Error(Peek(), "Expect expression.");
}

/// Precedence will have ! parsed first and then - afterwards.
UniqueExpression Parser::ParseUnary() {
  if (CheckAndConsumeTokens({BANG, MINUS})) {
    Token unary_operator = Previous();
    UniqueExpression right_side = ParseUnary();
    right_side.reset(
        new parsed::Unary(unary_operator, std::move(right_side)));
    return std::move(right_side);
  }

  return std::move(ParseCall());
}

/// Checks for subtraction first and then addition after.
UniqueExpression Parser::ParseTerm() {
  UniqueExpression expression = ParseFactor();

  while (CheckAndConsumeTokens({MINUS, PLUS})) {
    Token expr_operator = Previous();
    UniqueExpression right_side = ParseUnary();
    expression.reset(
        new parsed::Binary(
            std::move(expression), expr_operator, std::move(right_side)));
  }

  return std::move(expression);
}

UniqueExpression Parser::ParseFactor() {
  UniqueExpression expression = ParseUnary();

  while (CheckAndConsumeTokens({SLASH, STAR, MODULUS})) {
    Token expr_operator = Previous();
    UniqueExpression right_side = ParseUnary();
    expression.reset(
        new parsed::Binary(
            std::move(expression), expr_operator, std::move(right_side)));
  }

  return std::move(expression);
}

/// This matches >, >=, <, <= and creates a Binary expression from the
/// result of the parse.
UniqueExpression Parser::ParseComparison() {
  UniqueExpression expression = ParseTerm();

  while (CheckAndConsumeTokens({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token expr_operator = Previous();
    UniqueExpression right_side = ParseTerm();
    expression.reset(
        new parsed::Binary(
            std::move(expression), expr_operator, std::move(right_side)));
  }

  return std::move(expression);
}

UniqueExpression Parser::ParseOr() {
  UniqueExpression expression = ParseAnd();

  while (CheckAndConsumeTokens({OR})) {
    Token or_operator = Previous();
    UniqueExpression right_operand = ParseAnd();
    expression.reset(
        new parsed::Logical(
            std::move(expression), or_operator, std::move(right_operand)));
  }

  return std::move(expression);
}

UniqueExpression Parser::ParseAnd() {
  UniqueExpression expression = ParseEquality();

  while (CheckAndConsumeTokens({AND})) {
    Token and_operator = Previous();
    UniqueExpression right_operand = ParseEquality();
    expression.reset(
        new parsed::Logical(
            std::move(expression), and_operator, std::move(right_operand)));
  }

  return std::move(expression);
}

UniqueExpression Parser::ParseCall() {
  UniqueExpression expression = ParsePrimary();

  while (true) {
    if (CheckAndConsumeTokens({LEFT_PAREN})) {
      expression = FinishCall(std::move(expression));
    } else if (CheckAndConsumeTokens({DOT})) {
      Token name = Consume(IDENTIFIER, "Expect property name after '.'.");
      expression.reset(new parsed::Get(std::move(expression), name));
    } else {
      break;
    }
  }

  return std::move(expression);
}

UniqueExpression Parser::FinishCall(UniqueExpression callee) {
  std::vector<UniqueExpression> arguments;

  if (!CheckToken(RIGHT_PAREN)) {
    do {
      if (arguments.size() >= 255) {
        Error(Peek(), "Can't have more than 255 arguments.");
      }

      arguments.emplace_back(std::move(ParseExpression()));
    } while (CheckAndConsumeTokens({COMMA}));
  }

  Token parent = Consume(RIGHT_PAREN, "Expect ')' after arguments.");

  UniqueExpression call;
  call.reset(
      new parsed::Call(std::move(callee), parent, std::move(arguments)));
  return std::move(call);
}

}  // namespace parsing
}  // namespace lamscript
