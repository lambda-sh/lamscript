#ifndef SRC_LAMSCRIPT_PARSING_TOKENTYPE_H_
#define SRC_LAMSCRIPT_PARSING_TOKENTYPE_H_

#include <string>

namespace lamscript {
namespace parsing {


/// @brief Tokens supported by the lamscript parser.
enum TokenType {
  // single character tokens
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS, PLUS,
  SEMICOLON, SLASH, STAR, MODULUS,

  // boolean operation tokens
  BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS,
  LESS_EQUAL,

  // Literals
  IDENTIFIER, STRING, NUMBER,

  // Keywords
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS,
  TRUE, VAR, WHILE,

  // etc
  END_OF_FILE
};

std::string ConvertTokenTypeToString(TokenType token_type);

}  // namespace parsing
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSING_TOKENTYPE_H_
