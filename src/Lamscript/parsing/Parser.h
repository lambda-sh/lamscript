#ifndef SRC_LAMSCRIPT_PARSING_PARSER_H_
#define SRC_LAMSCRIPT_PARSING_PARSER_H_

#include <initializer_list>
#include <memory>
#include <typeinfo>
#include <vector>

#include <Lamscript/errors/ParseError.h>
#include <Lamscript/parsed/Expression.h>
#include <Lamscript/parsed/Statement.h>
#include <Lamscript/parsing/Token.h>
#include <Lamscript/parsing/TokenType.h>

namespace lamscript {
namespace parsing {

/// @brief The Lamscript LL parser for converting Tokens into
/// Statements and Expressions. This evaluates Tokens presented by the scanner
/// and converts it into code that can be executed by the interpreter.
class Parser {
 public:
  explicit Parser(const std::vector<Token>& tokens)
      : tokens_(tokens), current_token_(0) {}

  /// @brief Begins parsing all tokens provided to the Parser.
  std::vector<std::unique_ptr<parsed::Statement>> Parse();

 private:
  std::vector<Token> tokens_;
  int current_token_;

  /// @brief Peek at the next token that we're going to parse.
  Token Peek();

  /// @brief Look at the previously parsed token.
  Token Previous();

  /// @brief Check to see if the end of the file has been reached.
  bool HasReachedEOF();

  /// @brief Advance to the next token if it exists, otherwise give the last
  /// that was parsed.
  Token Advance();

  /// @brief Checks if the current token matches the given token type.
  bool CheckToken(const TokenType& token_type);

  /// @brief validates that the current token matches any given set of types.
  bool CheckAndConsumeTokens(
      const std::initializer_list<TokenType>& token_types);

  /// @brief Upon an error occurring, we synchronize the parser to get to the
  /// next potentially valid expression/statement.
  void Synchronize();

  /// @brief Returns an error that propagates up through the stack for hh
  ParseError Error(Token token, const std::string& message);

  /// @brief Consumes a token if it matches the type of token being passed in.
  /// throws an error if the token doesn't match.
  Token Consume(TokenType type, const std::string& message);

  // ----------------------------- PARSE STATEMENTS ----------------------------

  /// @brief Attempts to parse a variable declaration statement. If a variable
  /// declaration isn't found, it will continue trying to parse other types of
  /// statements.
  std::unique_ptr<parsed::Statement> ParseDeclaration();

  /// @brief Parse Variable declaration and returns a VariableStatement when
  /// successfully done.
  std::unique_ptr<parsed::Statement> ParseVariableDeclaration();

  /// @brief Parses block statements produced from bracket {} tokens and returns
  /// the list of statements that belong to that scope.
  std::vector<std::unique_ptr<parsed::Statement>> ParseBlockStatements();

  /// @brief Generic statement parsing that parses for more generic statements.
  /// See the docs or cpp file for more information on precedence/order.
  std::unique_ptr<parsed::Statement> ParseStatement();

  /// @brief Parses the current print and the expression that is being printed.
  std::unique_ptr<parsed::Statement> ParsePrintStatement();

  /// @brief Parses the current expression statement. This is the simplest type
  /// of statement that can be produced.
  std::unique_ptr<parsed::Statement> ParseExpressionStatement();

  /// @brief Parses conditional if/then/else statements.
  std::unique_ptr<parsed::Statement> ParseIfStatement();

  /// @brief Parses while statements.
  std::unique_ptr<parsed::Statement> ParseWhileStatement();

  /// @brief Parses for statements.
  std::unique_ptr<parsed::Statement> ParseForStatement();

  /// @brief Parses function statements.
  std::unique_ptr<parsed::Statement> ParseFunction(const std::string& kind);

  std::unique_ptr<parsed::Statement> ParseReturnStatement();

  std::unique_ptr<parsed::Statement> ParseClassStatement();

  // ---------------------------- PARSE EXPRESSIONS ----------------------------

  /// @brief Parses an equality for as long as there are equal signs and
  /// continually chain the previous expression e.g. `true == false;`
  std::unique_ptr<parsed::Expression> ParseEquality();

  /// @brief Parses an assignment expression e.g. `x = 4;`
  std::unique_ptr<parsed::Expression> ParseAssignment();

  /// @brief Generically parses an expression. Starts off with assignment.
  std::unique_ptr<parsed::Expression> ParseExpression();

  /// @brief Parses primary expressions into literals. e.g. `"hello, world!"`
  std::unique_ptr<parsed::Expression> ParsePrimary();

  /// @brief Parse Unary Tokens. e.g. `-x || !x;`
  std::unique_ptr<parsed::Expression> ParseUnary();

  /// @brief Parse subtraction and addition terms. e.g. `x + 5;`
  std::unique_ptr<parsed::Expression> ParseTerm();

  /// @brief Parses division and multiplication. e.g. `x * 5;`
  std::unique_ptr<parsed::Expression> ParseFactor();

  /// @brief Parse the current comparison being made.
  std::unique_ptr<parsed::Expression> ParseComparison();

  /// @brief Parse Or comparisons. e.g. `10 or nil;`.
  std::unique_ptr<parsed::Expression> ParseOr();

  /// @brief Parse And comparisons. e.g. `10 and nil;`.
  std::unique_ptr<parsed::Expression> ParseAnd();

  /// @brief Parse a function or method call.
  std::unique_ptr<parsed::Expression> ParseCall();

  /// @brief Finishes up a function or method call.
  std::unique_ptr<parsed::Expression> FinishCall(
      std::unique_ptr<parsed::Expression> callee);
};

}  // namespace parsing
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSING_PARSER_H_
