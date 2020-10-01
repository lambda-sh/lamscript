#ifndef SRC_LAMSCRIPT_SCANNER_H_
#define SRC_LAMSCRIPT_SCANNER_H_

#include <string>
#include <vector>

#include <Lamscript/Lamscript.h>
#include <Lamscript/Token.h>
#include <Lamscript/TokenType.h>

namespace lamscript {


/// @brief Lightweight scanner class.
class Scanner {
 public:
  explicit Scanner(std::string source)
      : source_(source), start_(0), current_(0), line_(1) {}

  /// @brief Scan in the tokens of the source that have been provided to the
  /// scanner.
  const std::vector<Token>& ScanTokens() {
    while (!HasReachedEOF()) {
      start_ = current_;
      ScanToken();
    }

    tokens_.push_back({END_OF_FILE, "", nullptr, line_ });
    return tokens_;
  }

 private:
  int start_, current_, line_;
  std::string source_;
  std::vector<Token> tokens_;

  /// @brief Checks to see if the scanner has reached the end of the file.
  bool HasReachedEOF() { return current_ >= source_.length(); }

  /// @brief Advance the scanner one character further.
  const char& Advance() {
    current_++;
    return source_[current_ - 1];
  }

  /// @brief Add a single char token to the list of tokens.
  void AddToken(TokenType token_type) {
    AddToken(token_type, nullptr);
  }

  /// @brief Add a token with it's literal to the list of tokens.
  void AddToken(TokenType token_type, void* literal) {
    std::string text = source_.substr(start_, current_);
    tokens_.push_back({ token_type, text, literal, line_ });
  }

  /// @brief Ensure that the expected character matches
  /// the character that the scanner is currently looking at.
  bool Match(char expected) {
    if (HasReachedEOF()) return false;
    if (source_[current_] != expected) return false;

    current_++;
    return true;
  }

  /// @brief Peek into what the next token is going to be. Doesn't advance the
  /// scanners current position.
  char Peek() {
    if (HasReachedEOF()) return '\0';
    return source_[current_];
  }

  void ScanToken() {
    char c = Advance();
    switch (c) {
      case '(': AddToken(LEFT_PAREN); break;
      case ')': AddToken(RIGHT_PAREN); break;
      case '{': AddToken(LEFT_BRACE); break;
      case '}': AddToken(RIGHT_BRACE); break;
      case ',': AddToken(COMMA); break;
      case '.': AddToken(DOT); break;
      case '-': AddToken(MINUS); break;
      case '+': AddToken(PLUS); break;
      case ';': AddToken(SEMICOLON); break;
      case '*': AddToken(STAR); break;
      case '!': AddToken(Match('=') ? BANG_EQUAL : BANG); break;
      case '=': AddToken(Match('=') ? EQUAL_EQUAL : EQUAL); break;
      case '<': AddToken(Match('=') ? LESS_EQUAL : LESS); break;
      case '>': AddToken(Match('=') ? GREATER_EQUAL : GREATER); break;
      case '/':
        /// If the Next token is going to be another slash, then we ignore the
        /// current line until a new line character has been found.
        if (Match('/')) {
          while (Peek() != '\n' && !HasReachedEOF()) {
            Advance();
          }
        } else {
          AddToken(SLASH);
        }
        break;
      case ' ':  break;
      case '\r': break;
      case '\t': break;
      case '\n': line_++; break;
      default:
        Lamscript::Error(line_, "Encountered an unexpected character.");
        break;
    }
  }
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_SCANNER_H_
