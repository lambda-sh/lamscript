#ifndef SRC_LAMSCRIPT_PARSING_SCANNER_H_
#define SRC_LAMSCRIPT_PARSING_SCANNER_H_

#include <bits/stdc++.h>
#include <cctype>
#include <string>
#include <vector>

#include <Lamscript/parsing/Token.h>
#include <Lamscript/parsing/TokenType.h>
#include <Lamscript/runtime/Lamscript.h>

namespace lamscript {
namespace parsing {

/// @brief Lightweight scanner class.
class Scanner {
 public:
  explicit Scanner(std::string source)
      : source_(source), start_(0), current_(0), line_(1) {}

  /// @brief Scan in the tokens of the source that have been provided to the
  /// scanner.
  const std::vector<Token>& ScanTokens();

 private:
  int start_, current_, line_;
  std::string source_;
  std::vector<Token> tokens_;
  static std::unordered_map<std::string, TokenType> keywords_;

  /// @brief Checks to see if the scanner has reached the end of the file.
  bool HasReachedEOF();

  /// @brief Advance the scanner one character further.
  const char& Advance();

  /// @brief Add a single char token to the list of tokens.
  void AddToken(TokenType token_type) {
    AddToken(token_type, nullptr);
  }

  /// @brief Add a token with it's literal to the list of tokens.
  ///
  /// All literal allocations should be heap allocated, allowing for the Scanner
  /// to manage the lifetime of the literal.
  void AddToken(TokenType token_type, std::any literal);

  /// @brief Ensure that the expected character matches
  /// the character that the scanner is currently looking at.
  bool Match(char expected);

  /// @brief Peek into what the next token is going to be. Doesn't advance the
  /// scanners current position.
  char Peek();

  /// @brief Peek after the current token.
  char PeekNext();

  bool IsDigit(const char& c);
  bool IsAlpha(const char& c);
  bool IsAlphaNumeric(const char& c);

  void ParseIdentifier();
  void ParseString();
  void ParseNumber();

  void ScanToken();
};

}  // namespace parsing
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSING_SCANNER_H_
