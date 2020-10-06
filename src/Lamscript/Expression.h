#ifndef SRC_LAMSCRIPT_EXPRESSION_H_
#define SRC_LAMSCRIPT_EXPRESSION_H_

#include <vector>

#include <Lamscript/Token.h>

namespace lamscript {

// Forward declaration for the Visitor interface.
template<typename ReturnType>
class Visitor;

/// @brief Base expression class used for allowing expressions to work with one
/// another
class Expression {};

/// @brief Binary expression handler.
class Binary : Expression {
 public:
  Binary(Expression* left, Token expression_operator, Expression* right)
      : left_(left), operator_(expression_operator), right_(right) {}


  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitBinaryExpression(this);
  }

 private:
  Expression* left_;
  Token operator_;
  Expression* right_;
};

class Assign : Expression {
 public:
  Assign(Token name, Expression* value) : name_(name), value_(value) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitAssignExpression(this);
  }

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

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitCallExpression(this);
  }

 private:
  Expression* callee_;
  Token parentheses_;
  std::vector<Expression*> arguments_;
};


class Get : Expression {
 public:
  Get(Expression* object, Token name) : object_(object), name_(name) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitGetExpression(this);
  }

 private:
  Expression* object_;
  Token name_;
};

class Grouping : Expression {
 public:
  explicit Grouping(Expression* expression) : expression_(expression) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitGroupingExpression(this);
  }

 private:
  Expression* expression_;
};

class Literal : Expression {
 public:
  explicit Literal(void* value) : value_(value) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitLiteralExpression(this);
  }

 private:
  void* value_;
};

class Logical : Expression {
 public:
  Logical(Expression* left, Token logical_operator, Expression* right)
      : left_(left), logical_operator_(logical_operator), right_(right) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitLogicalExpression(this);
  }

 private:
  Expression* left_;
  Token logical_operator_;
  Expression* right_;
};

class Set : Expression {
 public:
  Set(Expression* object, Token name, Expression* value)
      : object_(object), name_(name), value_(value) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitSetExpression(this);
  }

 private:
  Expression* object_;
  Token name_;
  Expression* value_;
};

class Super : Expression {
 public:
  Super(Token keyword, Token method) : keyword_(keyword), method_(method) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitSuperExpression(this);
  }

 private:
  Token keyword_;
  Token method_;
};

class This : Expression {
 public:
  explicit This(Token keyword) : keyword_(keyword) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitThisExpression(this);
  }

 private:
  Token keyword_;
};

class Unary : Expression {
 public:
  Unary(Token unary_operator, Expression* right)
      : unary_operator_(unary_operator), right_(right) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitUnaryExpression(this);
  }

 private:
  Token unary_operator_;
  Expression* right_;
};

class Variable : Expression {
 public:
  explicit Variable(Token name) : name_(name) {}

  template<typename ReturnType>
  ReturnType Accept(Visitor<ReturnType> visitor) {
    visitor.VisitVariableExpression(this);
  }

 private:
  Token name_;
};

template<typename ReturnType>
class Visitor {
 protected:
  friend class Expression;
  ReturnType VisitAssignExpression(Assign expression);
  ReturnType VisitBinaryExpression(Binary expression);
  ReturnType VisitCallExpression(Call expression);
  ReturnType VisitGetExpression(Get expression);
  ReturnType VisitGroupingExpression(Grouping expression);
  ReturnType VisitLiteralExpression(Literal expression);
  ReturnType VisitLogicalExpression(Logical expression);
  ReturnType VisitSetExpression(Set expression);
  ReturnType VisitSuperExpression(Super expression);
  ReturnType VisitThisExpression(This expression);
  ReturnType VisitUnaryExpression(Unary expression);
  ReturnType VisitVariableExpression(Variable expression);
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_EXPRESSION_H_
