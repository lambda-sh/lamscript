#ifndef SRC_LAMSCRIPT_PARSER_H_
#define SRC_LAMSCRIPT_PARSER_H_

#include <initializer_list>
#include <stdexcept>
#include <typeinfo>
#include <vector>

#include <Lamscript/Expression.h>
#include <Lamscript/Lamscript.h>
#include <Lamscript/Statement.h>
#include <Lamscript/Token.h>
#include <Lamscript/TokenType.h>

namespace lamscript {

/// @brief Custom Parse error for when the parser encounters an invalid
/// expression/statement.
class ParseError : std::runtime_error {
 public:
  explicit ParseError(const std::string& what_arg)
      : std::runtime_error(what_arg) {}
  explicit ParseError(const char* what_arg) : std::runtime_error(what_arg) {}
};

class Parser {
 public:
  explicit Parser(const std::vector<Token>& tokens)
      : tokens_(tokens), current_token_(0) {}

  /// @brief Begins parsing all tokens provided to the Parser.
  std::vector<Statement*> Parse() {
    std::vector<Statement*> statements;

    while (!HasReachedEOF()) {
      statements.push_back(ParseDeclaration());
    }

    return statements;
  }

 private:
  std::vector<Token> tokens_;
  int current_token_;

  /// @brief Peek at the next token that we're going to parse.
  Token Peek() {
    return tokens_.at(current_token_);
  }

  /// @brief Look at the previously parsed token.
  Token Previous() {
    return tokens_.at(current_token_ - 1);
  }

  /// @brief Check to see if the end of the file has been reached.
  bool HasReachedEOF() {
    if (current_token_ == 0) { return false; }
    return Peek().Type == END_OF_FILE;
  }

  /// @brief Advance to the next token if it exists, otherwise give the last
  /// that was parsed.
  Token Advance() {
    if (!HasReachedEOF()) {
      current_token_++;
    }

    return Previous();
  }

  /// @brief Checks if the current token matches the given token type.
  bool CheckToken(const TokenType& token_type) {
    if (HasReachedEOF()) {
      return false;
    }

    return Peek().Type == token_type;
  }

  /// @brief validates that the current token matches any given set of types.
  bool CheckTokens(const std::initializer_list<TokenType>& token_types) {
    for (TokenType token_type : token_types) {
      if (CheckToken(token_type)) {
        Advance();
        return true;
      }
    }
    return false;
  }


  // ----------------------------- PARSE STATEMENTS ----------------------------

  /// @brief Attempts to parse a variable declaration statement. If a variable
  /// declaration isn't found, it will continue trying to parse other types of
  /// statements.
  ///
  /// Synchronizes to the next valid statement when it runs into an invalid
  /// statement.
  Statement* ParseDeclaration() {
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

  Statement* ParseVariableDeclaration() {
    Token name = Consume(IDENTIFIER, "Expect a variable name.");
    Expression* initializer;

    // Variables can be optionally initialized.
    if (CheckTokens({EQUAL})) {
      initializer = ParseExpression();
    }

    Consume(SEMICOLON, "Expect ';' after variable declaration.");
    return new VariableStatement(name, initializer);
  }

  std::vector<Statement*> ParseBlockStatements() {
    std::vector<Statement*> statements;

    // Check for right side braces without consuming any tokens.
    while (!CheckToken(RIGHT_BRACE) && !HasReachedEOF()) {
      statements.push_back(ParseDeclaration());
    }

    Consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
  }

  Statement* ParseStatement() {
    if (CheckTokens({PRINT})) {
      return ParsePrintStatement();
    }

    if (CheckTokens({LEFT_BRACE})) {
      return new Block(ParseBlockStatements());
    }

    return ParseExpressionStatement();
  }

  Statement* ParsePrintStatement() {
    Expression* value = ParseExpression();
    Consume(SEMICOLON, "Expect ';' after value.");

    return new Print(value);
  }

  Statement* ParseExpressionStatement() {
    Expression* value = ParseExpression();
    Consume(SEMICOLON, "Expect ';' after value.");

    return new ExpressionStatement(value);
  }

  // ---------------------------- PARSE EXPRESSIONS ----------------------------

  /// @brief Parses an equality for as long as there are equal signs and
  /// continually chain the previous expression
  ///
  /// @todo (C3NZ): I need to keep an eye on this just to make sure that the
  /// initial pointer expression is being passed into the binary expression and
  /// that the binary expression isn't holding a reference to itself after
  /// assignment.
  Expression* ParseEquality() {
    Expression* expression = ParseComparison();

    while (CheckTokens({BANG_EQUAL, EQUAL_EQUAL})) {
      Token expr_operator = Previous();
      Expression* right_side = ParseComparison();
      expression = new Binary(expression, expr_operator, right_side);
    }

    return expression;
  }

  Expression* ParseAssignment() {
    Expression* expression = ParseEquality();

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

  Expression* ParseExpression() {
    return ParseAssignment();
  }

  /// @brief Parses primary expressions into literals.
  Expression* ParsePrimary() {
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

  /// @brief Parse Unary Tokens.
  ///
  /// Precedence will have ! parsed first and then - afterwards.
  Expression* ParseUnary() {
    if (CheckTokens({BANG, MINUS})) {
      Token unary_operator = Previous();
      Expression* right_side = ParseUnary();
      return new Unary(unary_operator, right_side);
    }

    return ParsePrimary();
  }

  /// @brief Check for subtraction and addition.
  ///
  /// Checks for subtraction first and then addition after.
  Expression* ParseTerm() {
    Expression* expression = ParseFactor();

    while (CheckTokens({MINUS, PLUS})) {
      Token expr_operator = Previous();
      Expression* right_side = ParseUnary();
      expression = new Binary(expression, expr_operator, right_side);
    }

    return expression;
  }

  Expression* ParseFactor() {
    Expression* expression = ParseUnary();

    while (CheckTokens({SLASH, STAR})) {
      Token expr_operator = Previous();
      Expression* right_side = ParseUnary();
      expression = new Binary(expression, expr_operator, right_side);
    }

    return expression;
  }

  /// @brief Parse the current comparison being made.
  ///
  /// This matches >, >=, <, <= and creates a Binary expression from the
  /// result of the parse.
  Expression* ParseComparison() {
    Expression* expression = ParseTerm();

    while (CheckTokens({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
      Token expr_operator = Previous();
      Expression* right_side = ParseTerm();
      expression = new Binary(expression, expr_operator, right_side);
    }

    return expression;
  }

  /// @brief Upon an error occurring, we synchronize the parser to get to the
  /// next potentially valid expression/statement.
  //
  /// By doing so, this should hopefully remove all potential ghost errrors
  /// that associated with the current malformed expression.
  void Synchronize() {
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

  /// @brief Returns an error that propagates up through the stack for hh
  ParseError Error(Token token, const std::string& message) {
    Lamscript::Error(token, message);
    return ParseError(message);
  }

  /// @brief Consumes a token if it matches the type of token being passed in.
  /// throws an error if the token doesn't match.
  Token Consume(TokenType type, const std::string& message) {
    if (CheckToken(type)) {
      return Advance();
    }

    throw message;
  }
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSER_H_
