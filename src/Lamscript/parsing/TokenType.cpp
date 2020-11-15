#include <Lamscript/parsing/TokenType.h>

#include <string>

namespace lamscript {
namespace parsing {


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
    case MODULUS:       return "%";
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
    case OR:            return "OR";
    case RETURN:        return "return";
    case SUPER:         return "super";
    case THIS:          return "this";
    case VAR:           return "var";
    case WHILE:         return "while";
    case PRINT:         return "print";
    case END_OF_FILE:   return "eof";
    default:            return "";
  }
}

}  // namespace parsing
}  // namespace lamscript
