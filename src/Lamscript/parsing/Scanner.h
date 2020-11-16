#ifndef SRC_LAMSCRIPT_PARSING_SCANNER_H_
#define SRC_LAMSCRIPT_PARSING_SCANNER_H_

#include <bits/stdc++.h>
#include <cctype>
#include <string>
#include <vector>

#include <Lamscript/Lamscript.h>
#include <Lamscript/parsing/Token.h>
#include <Lamscript/parsing/TokenType.h>

namespace lamscript {
namespace parsing {

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

    tokens_.push_back({ END_OF_FILE, "", nullptr, line_ });
    return tokens_;
  }

 private:
  int start_, current_, line_;
  std::string source_;
  std::vector<Token> tokens_;

  static std::unordered_map<std::string, TokenType> keywords_;

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
  ///
  /// All literal allocations should be heap allocated, allowing for the Scanner
  /// to manage the lifetime of the literal.
  void AddToken(TokenType token_type, std::any literal) {
    std::string text = source_.substr(start_, current_ - start_);
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
    if (HasReachedEOF()) {
      return '\0';
    }

    return source_[current_];
  }

  char PeekNext() {
    int next_position = current_ + 1;
    if (next_position >= source_.length()) {
      return '\0';
    }

    return source_[next_position];
  }

  bool IsDigit(const char& c) {
    return c >= '0' && c <= '9';
  }

  bool IsAlpha(const char& c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')  || (c == '_');
  }

  bool IsAlphaNumeric(const char& c) {
    return IsAlpha(c) || IsDigit(c);
  }

  void ParseIdentifier() {
    while (IsAlphaNumeric(Peek())) {
      Advance();
    }

    // Fetch the identifier and then perform a lookup to see if the identifier
    // is a valid keyword.
    std::string identifier = source_.substr(start_, current_ - start_);
    auto lookup = keywords_.find(identifier);
    TokenType type = IDENTIFIER;

    if (lookup != keywords_.end()) {
      type = lookup->second;
    }
    AddToken(type);
  }

  /// @brief Parse a String literal.
  ///
  /// @todo (C3NZ): Is it possible to allocate a substring on the heap without
  /// having to create the substring and then store it in the heap allocated
  /// string?
  void ParseString() {
    while (Peek() != '"' && !HasReachedEOF()) {
      if (Peek() == '\n') {
        line_++;
      }
      Advance();
    }

    if (HasReachedEOF()) {
      Lamscript::Error(line_, "Unterminated string.");
      return;
    }

    Advance();

    std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
    AddToken(STRING, value);
  }

  /// @brief Parses a number
  void ParseNumber() {
    while (IsDigit(Peek())) {
      Advance();
    }

    // If theres currently a decimal place and the next value that is going to
    // be read into our scanner is a digit, then we want to continue to add the
    // value on to this digit.
    if (Peek() == '.' && IsDigit(PeekNext())) {
      Advance();
      while (IsDigit(Peek())) {
        Advance();
      }
    }

    /// @todo Do we need to do allocation for doubles on the heap like this?
    /// I've never seen this used elsewhere and the syntax seems a bit
    /// confusing.
    double d = std::stod(source_.substr(start_, current_ - start_));
    AddToken(NUMBER, d);
  }

  /// @brief Scan a single token at a time and determine the type of
  /// token parsing that should occur.
  void ScanToken() {
    const char& c = Advance();
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
      case '%': AddToken(MODULUS); break;
      case '!': AddToken(Match('=') ? BANG_EQUAL : BANG); break;
      case '=': AddToken(Match('=') ? EQUAL_EQUAL : EQUAL); break;
      case '<': AddToken(Match('=') ? LESS_EQUAL : LESS); break;
      case '>': AddToken(Match('=') ? GREATER_EQUAL : GREATER); break;
      case '/':
        // If the Next token is going to be another slash, then we ignore the
        // current line until a new line character has been found.
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
      case '"':  ParseString(); break;
      default:
        if (IsDigit(c)) {
          ParseNumber();
        } else if (IsAlpha(c)) {
          ParseIdentifier();
        } else {
          Lamscript::Error(line_, "Encountered an unexpected character.");
        }
        break;
    }
  }
};

// Keyword lookup.
std::unordered_map<std::string, TokenType> Scanner::keywords_ =
    std::unordered_map<std::string, TokenType>(
        {
            {"and", AND},
            {"class", CLASS},
            {"else", ELSE},
            {"false", FALSE},
            {"for", FOR},
            {"func", FUN},
            {"if", IF},
            {"nil", NIL},
            {"or", OR},
            {"print", PRINT},
            {"return", RETURN},
            {"super", SUPER},
            {"this", THIS},
            {"true", TRUE},
            {"var", VAR},
            {"while", WHILE}});

}  // namespace parsing
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSING_SCANNER_H_
