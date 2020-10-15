#ifndef SRC_LAMSCRIPT_PARSER_H_
#define SRC_LAMSCRIPT_PARSER_H_

#include <initializer_list>
#include <typeinfo>
#include <vector>

#include <Lamscript/Expression.h>
#include <Lamscript/Token.h>
#include <Lamscript/TokenType.h>

namespace lamscript {

class Parser {
 public:
  explicit Parser(std::vector<Token>& tokens) : tokens_(tokens) {}

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
    return Peek().Type == EOF;
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
  bool CheckToken(TokenType token_type) {
    if (HasReachedEOF()) {
      return false;
    }

    return Peek().Type == token_type;
  }

  /// @brief validates that the current token matches any given set of types.
  bool CheckTokens(std::initializer_list<TokenType> token_types) {
    for (TokenType token_type : token_types) {
      if (CheckToken(token_type)) {
        Advance();
        return true;
      }
    }
    return false;
  }

  Expression* ParseExpression() {
    return ParseEquality();
  }

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

  Expression* ParsePrimary() {
      if (CheckToken(FALSE)) {
        return new Literal(false);
      }

      if (CheckToken(TRUE)) {
        return new Literal(true);
      }

      if (CheckToken(NIL)) {
        return new Literal();
      }

      if (CheckTokens({NUMBER, STRING})) {
        Token token = Previous();

        if (token.Literal.type() == typeid(double)) {
          return new Literal(std::any_cast<double>(token.Literal));
        }

        return new Literal(std::any_cast<std::string>(token.Literal));
      }

      if (CheckToken(LEFT_PAREN)) {
        Expression* expression = ParseExpression();
        Consume(RIGHT_PAREN, "Expect ')' after expression");
        return new Grouping(expression);
      }
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
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSER_H_
