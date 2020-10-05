#ifndef SRC_LAMSCRIPT_EXPRESSION_H_
#define SRC_LAMSCRIPT_EXPRESSION_H_

#include <vector>Jj

#include <Lamscript/Token.h>

namespace lamscript {

/// @brief Base expression class used for allowing expressions to work with one
/// another
class Expression {};

/// @brief Binary expression handler.
class Binary : Expression {
 public:
  Binary(Expression* left, Token expression_operator, Expression* right)
      : left_(left), operator_(expression_operator), right_(right) {}

 private:
  Expression* left_;
  Token operator_;
  Expression* right_;
};

class Assign : Expression {
 public:
  Assign(Token name, Expression* value) : name_(name), value_(value) {}

 private:
  Token name_;
  Expression* value_;
};

class Call : Expression {
 public:
  Call(
      Expression* callee,
      Token parentheses,
      std::vector<Expression*> arguments)
          : callee_(callee), parentheses_(parentheses), arguments_(arguments) {}

 private:
  Expression* callee_;
  Token parentheses_;
  std::vector<Expression*> arguments_;
};


class Get : Expression {
 public:
  Get(Expression* object, Token name) : object_(object), name_(name) {}

 private:
  Expression* object_;
  Token name_;
};

class Grouping : Expression {
 public:
  explicit Grouping(Expression* expression) : expression_(expression) {}

 private:
  Expression* expression_;
};

class Literal : Expression {
 public:
  explicit Literal(void* value) : value_(value) {}

 private:
  void* value_;
};

class Logical : Expression {
 public:
  Logical(Expression* left, Token logical_operator, Expression* right)
      : left_(left), logical_operator_(logical_operator), right_(right) {}

 private:
  Expression* left_;
  Token logical_operator_;
  Expression* right_;
};

class Set : Expression {
 public:
  Set(Expression* object, Token name, Expression* value)
      : object_(object), name_(name), value_(value) {}

 private:
  Expression* object_;
  Token name_;
  Expression* value_;
};

class Super : Expression {
 public:
  Super(Token keyword, Token method) : keyword_(keyword), method_(method) {}

 private:
  Token keyword_;
  Token method_;
};

class This : Expression {
 public:
  explicit This(Token keyword) : keyword_(keyword) {}

 private:
  Token keyword_;
};

class Unary : Expression {
 public:
  Unary(Token unary_operator, Expression* right)
      : unary_operator_(unary_operator), right_(right) {}

 private:
  Token unary_operator_;
  Expression* right_;
};

class Variable : Expression {
 public:
  explicit Variable(Token name) : name_(name) {}

 private:
  Token name_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_EXPRESSION_H_
