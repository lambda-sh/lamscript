#ifndef LAMSCRIPT_SRC_LAMSCRIPT_TOKENTYPE_H_
#define LAMSCRIPT_SRC_LAMSCRIPT_TOKENTYPE_H_
namespace lamscript {

/// @brief Tokens supported by the lamscript parser.
enum TokenType {
  // single character tokens
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS, PLUS,
  SEMICOLON, SLASH, STAR,

  // boolean operation tokens
  BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, 
  LESS_EQUAL,

  // Literals
  IDENTIFIER, STRING, NUMBER,

  // Keywords
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, 
  TRUE, VAR, WHILE,

  // etc
  EOF
};

}
#endif  // LAMSCRIPT_SRC_LAMSCRIPT_TOKENTYPE_H_
