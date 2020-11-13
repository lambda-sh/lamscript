#ifndef SRC_LAMSCRIPT_STATEMENT_H_
#define SRC_LAMSCRIPT_STATEMENT_H_

#include <any>
#include <memory>
#include <vector>

#include <Lamscript/Expression.h>

namespace lamscript {

class StatementVisitor;

class Statement {
 public:
  virtual std::any Accept(StatementVisitor* visitor) = 0;
  // virtual ~Statement();
};

/// @brief Curly brace block statements for defining a local scope.
class Block : public Statement {
 public:
  explicit Block(std::vector<std::unique_ptr<Statement>>&& statements)
      : statements_(std::move(statements)) {}

  std::any Accept(StatementVisitor* visitor) override;

  const std::vector<std::unique_ptr<Statement>>& GetStatements() const {
    return statements_;
  }

 private:
  std::vector<std::unique_ptr<Statement>> statements_;
};


class ExpressionStatement : public Statement {
 public:
  explicit ExpressionStatement(std::unique_ptr<Expression> expression)
      : expression_(std::move(expression)) {}

  std::any Accept(StatementVisitor* visitor) override;

  Expression* GetExpression() { return expression_.get(); }
 private:
  std::unique_ptr<Expression> expression_;
};

class Function : public Statement {
 public:
  Function(
      Token name,
      const std::vector<Token>& params,
      std::vector<std::unique_ptr<Statement>>&& body)
          : name_(name), params_(params), body_(std::move(body)) {}

  std::any Accept(StatementVisitor* visitor) override;

  const Token& GetName() const { return name_; }
  const std::vector<Token>& GetParams() const { return params_; }
  const std::vector<std::unique_ptr<Statement>>& GetBody() const {
      return body_; }

 private:
  Token name_;
  std::vector<Token> params_;
  std::vector<std::unique_ptr<Statement>> body_;
};

/// @brief Class definition statements.
class Class : public Statement {
 public:
  Class(Token name, Variable super_class, std::vector<Function>&& methods)
      : name_(name), super_class_(super_class), methods_(std::move(methods)) {}

  std::any Accept(StatementVisitor* visitor) override;

 private:
  Token name_;
  Variable super_class_;
  std::vector<Function> methods_;
};

class If : public Statement {
 public:
  If(
      std::unique_ptr<Expression> condition,
      std::unique_ptr<Statement> then_branch,
      std::unique_ptr<Statement> else_branch)
          : condition_(std::move(condition)),
          then_branch_(std::move(then_branch)),
          else_branch_(std::move(else_branch)) {}

  std::any Accept(StatementVisitor* visitor) override;

  Expression* GetCondition() { return condition_.get(); }
  Statement* GetThenBranch() { return then_branch_.get(); }
  Statement* GetElseBranch() { return else_branch_.get(); }

 private:
  std::unique_ptr<Expression> condition_;
  std::unique_ptr<Statement> then_branch_;
  std::unique_ptr<Statement> else_branch_;
};

/// @brief Handles expression to be printed.
class Print : public Statement {
 public:
  explicit Print(std::unique_ptr<Expression> expression)
    : expression_(std::move(expression)) {}

  std::any Accept(StatementVisitor* visitor) override;

  Expression* GetExpression() { return expression_.get(); }
 private:
  std::unique_ptr<Expression> expression_;
};

class Return : public Statement {
 public:
  Return(Token keyword, std::unique_ptr<Expression> value)
    : keyword_(std::move(keyword)), value_(std::move(value)) {}

  std::any Accept(StatementVisitor* visitor) override;

 private:
  Token keyword_;
  std::unique_ptr<Expression> value_;
};

class VariableStatement : public Statement {
 public:
  VariableStatement(Token name, std::unique_ptr<Expression> initializer)
      : name_(name), initializer_(std::move(initializer)) {}

  std::any Accept(StatementVisitor* visitor) override;

  const Token& GetName() const { return name_; }
  Expression* GetInitializer() const { return initializer_.get(); }

 private:
  Token name_;
  std::unique_ptr<Expression> initializer_;
};

class While : public Statement {
 public:
  While(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body)
      : condition_(std::move(condition)), body_(std::move(body)) {}

  std::any Accept(StatementVisitor* visitor) override;

  Expression* GetCondition() { return condition_.get(); }
  Statement* GetBody() { return body_.get(); }

 private:
  std::unique_ptr<Expression> condition_;
  std::unique_ptr<Statement> body_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_STATEMENT_H_
