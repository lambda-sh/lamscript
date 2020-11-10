#ifndef SRC_LAMSCRIPT_EXPRESSION_H_
#define SRC_LAMSCRIPT_EXPRESSION_H_

#include <any>
#include <vector>

#include <Lamscript/Token.h>

namespace lamscript {

/// @brief Base expression class used for allowing expressions to work with one
/// another
class ExpressionVisitor;

class Expression {
 public:
  virtual std::any Accept(ExpressionVisitor* visitor) = 0;
};

/// @brief Binary expression handler.
class Binary : public Expression {
 public:
  Binary(Expression* left, Token expression_operator, Expression* right)
      : left_(left), operator_(expression_operator), right_(right) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetLeftSide() const { return left_; }
  Expression* GetRightSide() const { return right_; }
  const Token& GetOperator() const { return operator_; }

 private:
  Expression* left_;
  Token operator_;
  Expression* right_;
};

class Assign : public Expression {
 public:
  Assign(Token name, Expression* value) : name_(name), value_(value) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetValue() const { return value_; }
  const Token& GetName() const { return name_; }

 private:
  Token name_;
  Expression* value_;
};

class Call : public Expression {
 public:
  Call(
      Expression* callee,
      Token parentheses,
      std::vector<Expression*> arguments)
          : callee_(callee), parentheses_(parentheses), arguments_(arguments) {}

  std::any Accept(ExpressionVisitor* visitor) override;

 private:
  Expression* callee_;
  Token parentheses_;
  std::vector<Expression*> arguments_;
};


class Get : public Expression {
 public:
  Get(Expression* object, Token name) : object_(object), name_(name) {}

  std::any Accept(ExpressionVisitor* visitor) override;

 private:
  Expression* object_;
  Token name_;
};

class Grouping : public Expression {
 public:
  explicit Grouping(Expression* expression) : expression_(expression) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetExpression() const { return expression_; }
 private:
  Expression* expression_;
};

class Literal : public Expression {
 public:
  Literal() {}
  explicit Literal(std::string literal) : value_(literal) {}
  explicit Literal(double literal) : value_(literal) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  std::any GetValue() { return value_; }
 private:
  std::any value_;
};

class Logical : public Expression {
 public:
  Logical(Expression* left, Token logical_operator, Expression* right)
      : left_(left), logical_operator_(logical_operator), right_(right) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetLeftOperand() { return left_; }
  const Token& GetLogicalOperator() { return logical_operator_; }
  Expression* GetRightOperand() { return right_; }

 private:
  Expression* left_;
  Token logical_operator_;
  Expression* right_;
};

class Set : public Expression {
 public:
  Set(Expression* object, Token name, Expression* value)
      : object_(object), name_(name), value_(value) {}

  std::any Accept(ExpressionVisitor* visitor) override;

 private:
  Expression* object_;
  Token name_;
  Expression* value_;
};

class Super : public Expression {
 public:
  Super(Token keyword, Token method) : keyword_(keyword), method_(method) {}

  std::any Accept(ExpressionVisitor* visitor) override;

 private:
  Token keyword_;
  Token method_;
};

class This : public Expression {
 public:
  explicit This(Token keyword) : keyword_(keyword) {}

  std::any Accept(ExpressionVisitor* visitor) override;

 private:
  Token keyword_;
};

class Unary : public Expression {
 public:
  Unary(Token unary_operator, Expression* right)
      : unary_operator_(unary_operator), right_(right) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetRightExpression() const { return right_; }
  const Token& GetUnaryOperator() const { return unary_operator_; }

 private:
  Token unary_operator_;
  Expression* right_;
};

class Variable : public Expression {
 public:
  explicit Variable(Token name) : name_(name) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  const Token& GetName() { return name_; }

 private:
  Token name_;
};


}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_EXPRESSION_H_
