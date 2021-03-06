#ifndef SRC_LAMSCRIPT_PARSED_EXPRESSION_H_
#define SRC_LAMSCRIPT_PARSED_EXPRESSION_H_

#include <any>
#include <memory>
#include <vector>

#include <Lamscript/parsing/Token.h>

namespace lamscript {

class ExpressionVisitor;

namespace parsed {

class Expression {
 public:
  virtual std::any Accept(ExpressionVisitor* visitor) = 0;
//   virtual ~Expression();
};

/// @brief Binary expression handler.
class Binary : public Expression {
 public:
  Binary(
      std::unique_ptr<Expression> left,
      parsing::Token expression_operator,
      std::unique_ptr<Expression> right)
          : left_(std::move(left)),
          operator_(expression_operator),
          right_(std::move(right)) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetLeftSide() const { return left_.get(); }
  Expression* GetRightSide() const { return right_.get(); }
  const parsing::Token& GetOperator() const { return operator_; }

 private:
  std::unique_ptr<Expression> left_;
  parsing::Token operator_;
  std::unique_ptr<Expression> right_;
};

class Assign : public Expression {
 public:
  Assign(
      parsing::Token name, std::unique_ptr<Expression> value)
          : name_(name), value_(std::move(value)) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetValue() const { return value_.get(); }
  const parsing::Token& GetName() const { return name_; }

 private:
  parsing::Token name_;
  std::unique_ptr<Expression> value_;
};

class Call : public Expression {
 public:
  Call(
      std::unique_ptr<Expression> callee,
      parsing::Token parentheses,
      std::vector<std::unique_ptr<Expression>>&& arguments)
          : callee_(std::move(callee)),
          parentheses_(parentheses),
          arguments_(std::move(arguments)) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetCallee() { return callee_.get(); }
  const parsing::Token& GetParentheses() { return parentheses_; }
  const std::vector<std::unique_ptr<Expression>>& GetArguments() {
      return arguments_; }

 private:
  std::unique_ptr<Expression> callee_;
  parsing::Token parentheses_;
  std::vector<std::unique_ptr<Expression>> arguments_;
};


class Get : public Expression {
 public:
  Get(
      std::shared_ptr<Expression> object,
      parsing::Token name)
          : object_(std::move(object)), name_(name) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  std::shared_ptr<Expression> GetObject() const { return object_; }
  const parsing::Token& GetName() const { return name_; }

 private:
  std::shared_ptr<Expression> object_;
  parsing::Token name_;
};

class Grouping : public Expression {
 public:
  explicit Grouping(
      std::unique_ptr<Expression> expression)
          : expression_(std::move(expression)) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetExpression() const { return expression_.get(); }
 private:
  std::unique_ptr<Expression> expression_;
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
  Logical(
      std::unique_ptr<Expression> left,
      parsing::Token logical_operator,
      std::unique_ptr<Expression> right)
        : left_(std::move(left)),
        logical_operator_(logical_operator),
        right_(std::move(right)) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetLeftOperand() { return left_.get(); }
  const parsing::Token& GetLogicalOperator() { return logical_operator_; }
  Expression* GetRightOperand() { return right_.get(); }

 private:
  std::unique_ptr<Expression> left_;
  parsing::Token logical_operator_;
  std::unique_ptr<Expression> right_;
};

class Set : public Expression {
 public:
  Set(
      std::shared_ptr<Expression> object,
      parsing::Token name,
      std::unique_ptr<Expression> value)
          : object_(object), name_(name), value_(std::move(value)) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  std::shared_ptr<Expression> GetObject() { return object_; }
  Expression* GetValue() { return value_.get(); }
  const parsing::Token& GetName() const { return name_; }

 private:
  std::shared_ptr<Expression> object_;
  parsing::Token name_;
  std::unique_ptr<Expression> value_;
};

class Super : public Expression {
 public:
  Super(parsing::Token keyword, parsing::Token method)
      : keyword_(keyword), method_(method) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  const parsing::Token& GetKeyword() const { return keyword_; }
  const parsing::Token& GetMethod() const { return method_; }

 private:
  parsing::Token keyword_;
  parsing::Token method_;
};

class This : public Expression {
 public:
  explicit This(parsing::Token keyword) : keyword_(keyword) {}

  std::any Accept(ExpressionVisitor* visitor) override;
  const parsing::Token& GetKeyword() const { return keyword_; }

 private:
  parsing::Token keyword_;
};

class Unary : public Expression {
 public:
  Unary(
      parsing::Token unary_operator,
      std::unique_ptr<Expression> right)
          : unary_operator_(unary_operator), right_(std::move(right)) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Expression* GetRightExpression() const { return right_.get(); }
  const parsing::Token& GetUnaryOperator() const { return unary_operator_; }

 private:
  parsing::Token unary_operator_;
  std::unique_ptr<Expression> right_;
};

class Variable : public Expression {
 public:
  explicit Variable(parsing::Token name) : name_(name) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  const parsing::Token& GetName() { return name_; }

 private:
  parsing::Token name_;
};

class Statement;

class LambdaExpression : public Expression {
 public:
  explicit LambdaExpression(std::unique_ptr<Statement> lambda_function)
      : lambda_function_(std::move(lambda_function)) {}

  std::any Accept(ExpressionVisitor* visitor) override;

  Statement* GetFunctionStatement() { return lambda_function_.get(); }
 private:
  std::unique_ptr<Statement> lambda_function_;
};

}  // namespace parsed
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSED_EXPRESSION_H_
