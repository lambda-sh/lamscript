#ifndef SRC_LAMSCRIPT_TOKEN_H_
#define SRC_LAMSCRIPT_TOKEN_H_

#include <any>
#include <string>

#include <Lamscript/TokenType.h>

namespace lamscript {

struct Token {
  TokenType Type;
  std::string Lexeme;
  std::any Literal;
  int Line;


  /// @todo Fix this so that TokenTypes can easily be converted into strings
  /// later down the line.
  std::string ToString() {
    return ConvertTokenTypeToString(Type) + " " + Lexeme + " ";
  }
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_TOKEN_H_
