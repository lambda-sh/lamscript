#ifndef SRC_LAMSCRIPT_TOKENTYPE_H_
#define SRC_LAMSCRIPT_TOKENTYPE_H_

#include <string>

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
  END_OF_FILE
};

/// @brief Convert TokenType enum values into their corresponding string
/// representations.
std::string ConvertTokenTypeToString(TokenType token_type) {
  switch (token_type) {
    case LEFT_PAREN:    return "(";
    case RIGHT_PAREN:   return ")";
    case LEFT_BRACE:    return "[";
    case RIGHT_BRACE:   return "]";
    case COMMA:         return ",";
    case DOT:           return ".";
    case MINUS:         return "-";
    case PLUS:          return "+";
    case SEMICOLON:     return ";";
    case SLASH:         return "/";
    case STAR:          return "*";
    case BANG:          return "!";
    case BANG_EQUAL:    return "!=";
    case EQUAL:         return "=";
    case EQUAL_EQUAL:   return "==";
    case GREATER:       return ">";
    case GREATER_EQUAL: return ">=";
    case LESS:          return "<";
    case LESS_EQUAL:    return "<=";
    case IDENTIFIER:    return "Identifier";
    case STRING:        return "String";
    case NUMBER:        return "Number";
    case AND:           return "&";
    case CLASS:         return "class";
    case ELSE:          return "else";
    case FALSE:         return "False";
    case TRUE:          return "true";
    case FUN:           return "fun";
    case FOR:           return "for";
    case IF:            return "if";
    case NIL:           return "nil";
    case OR:            return "print";
    case RETURN:        return "return";
    case SUPER:         return "super";
    case THIS:          return "this";
    case VAR:           return "var";
    case WHILE:         return "while";
    case END_OF_FILE:   return "eof";
    default:            return "";
  }
}

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_TOKENTYPE_H_
