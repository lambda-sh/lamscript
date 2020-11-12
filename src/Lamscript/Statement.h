#ifndef SRC_LAMSCRIPT_STATEMENT_H_
#define SRC_LAMSCRIPT_STATEMENT_H_

#include <any>
#include <vector>

#include <Lamscript/Expression.h>

namespace lamscript {

class StatementVisitor;

class Statement {
 public:
  virtual std::any Accept(StatementVisitor* visitor) = 0;
};

/// @brief Curly brace block statements for defining a local scope.
class Block : public Statement {
 public:
  explicit Block(const std::vector<Statement*>& statements)
      : statements_(statements) {}

  std::any Accept(StatementVisitor* visitor) override;

  const std::vector<Statement*>& GetStatements() { return statements_; }

 private:
  std::vector<Statement*> statements_;
};


class ExpressionStatement : public Statement {
 public:
  explicit ExpressionStatement(Expression* expression)
      : expression_(expression) {}

  std::any Accept(StatementVisitor* visitor) override;

  Expression* GetExpression() { return expression_; }
 private:
  Expression* expression_;
};

class Function : public Statement {
 public:
  Function(
      Token name,
      const std::vector<Token>& params,
      const std::vector<Statement*>& body)
          : name_(name), params_(params), body_(body) {}

  std::any Accept(StatementVisitor* visitor) override;

  const Token& GetName() const { return name_; }
  const std::vector<Token>& GetParams() const { return params_; }
  const std::vector<Statement*>& GetBody() const {return body_; }

 private:
  Token name_;
  std::vector<Token> params_;
  std::vector<Statement*> body_;
};

/// @brief Class definition statements.
class Class : public Statement {
 public:
  Class(Token name, Variable super_class, const std::vector<Function>& methods)
      : name_(name), super_class_(super_class), methods_(methods) {}

  std::any Accept(StatementVisitor* visitor) override;

 private:
  Token name_;
  Variable super_class_;
  std::vector<Function> methods_;
};

class If : public Statement {
 public:
  If(
      Expression* condition,
      Statement* then_branch,
      Statement* else_branch)
          : condition_(condition),
          then_branch_(then_branch),
          else_branch_(else_branch) {}

  std::any Accept(StatementVisitor* visitor) override;

  Expression* GetCondition() { return condition_; }
  Statement* GetThenBranch() { return then_branch_; }
  Statement* GetElseBranch() { return else_branch_; }

 private:
  Expression* condition_;
  Statement* then_branch_;
  Statement* else_branch_;
};

/// @brief Handles expression to be printed.
class Print : public Statement {
 public:
  explicit Print(Expression* expression) : expression_(expression) {}

  std::any Accept(StatementVisitor* visitor) override;

  Expression* GetExpression() { return expression_; }
 private:
  Expression* expression_;
};

class Return : public Statement {
 public:
  Return(Token keyword, Expression* value) : keyword_(keyword), value_(value) {}

  std::any Accept(StatementVisitor* visitor) override;

 private:
  Token keyword_;
  Expression* value_;
};

class VariableStatement : public Statement {
 public:
  VariableStatement(Token name, Expression* initializer)
      : name_(name), initializer_(initializer) {}

  std::any Accept(StatementVisitor* visitor) override;

  const Token& GetName() const { return name_; }
  Expression* GetInitializer() const { return initializer_; }

 private:
  Token name_;
  Expression* initializer_;
};

class While : public Statement {
 public:
  While(Expression* condition, Statement* body)
      : condition_(condition), body_(body) {}

  std::any Accept(StatementVisitor* visitor) override;

  Expression* GetCondition() { return condition_; }
  Statement* GetBody() { return body_; }

 private:
  Expression* condition_;
  Statement* body_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_STATEMENT_H_
