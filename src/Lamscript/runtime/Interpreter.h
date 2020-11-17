#ifndef SRC_LAMSCRIPT_RUNTIME_INTERPRETER_H_
#define SRC_LAMSCRIPT_RUNTIME_INTERPRETER_H_

#include <any>
#include <memory>
#include <typeinfo>

#include <Lamscript/Visitor.h>
#include <Lamscript/errors/RuntimeError.h>
#include <Lamscript/parsed/Expression.h>
#include <Lamscript/parsed/Statement.h>
#include <Lamscript/runtime/Environment.h>

namespace lamscript {
namespace runtime {

class Interpreter : ExpressionVisitor, StatementVisitor {
 public:
  Interpreter();
  // Implemented Expressions.

  std::any VisitAssignExpression(parsed::Assign* expression) override;
  std::any VisitLiteralExpression(parsed::Literal* expression) override;
  std::any VisitGroupingExpression(parsed::Grouping* expression) override;
  std::any VisitUnaryExpression(parsed::Unary* expression) override;
  std::any VisitBinaryExpression(parsed::Binary* expression) override;
  std::any VisitVariableExpression(parsed::Variable* expression) override;
  std::any VisitLogicalExpression(parsed::Logical* expression) override;
  std::any VisitCallExpression(parsed::Call* expression) override;
  std::any VisitLambdaExpression(parsed::LambdaExpression* expression) override;

  // Implemented Statements

  std::any VisitBlockStatement(parsed::Block* statement) override;
  std::any VisitExpressionStatement(
      parsed::ExpressionStatement* statement) override;
  std::any VisitPrintStatement(parsed::Print* statement) override;
  std::any VisitVariableStatement(
      parsed::VariableStatement* statement) override;
  std::any VisitIfStatement(parsed::If* statement) override;
  std::any VisitWhileStatement(parsed::While* statement) override;
  std::any VisitFunctionStatement(parsed::Function* statement) override;
  std::any VisitReturnStatement(parsed::Return* statement) override;

  /// @todo (C3NZ) Implement the rest of the visitor pattern.

  std::any VisitGetExpression(parsed::Get* expression) override {};
  std::any VisitSetExpression(parsed::Set* expression) override {};
  std::any VisitSuperExpression(parsed::Super* expression) override {};
  std::any VisitThisExpression(parsed::This* expression) override {};

  // Statements
  std::any VisitClassStatement(parsed::Class* statement) override {};

  // Primary external API

  void Interpret(
      const std::vector<std::unique_ptr<parsed::Statement>>& statements);
  void Execute(parsed::Statement* statement);
  void ExecuteBlock(
      const std::vector<std::unique_ptr<parsed::Statement>>& statements,
      std::shared_ptr<Environment> current_env);

  std::shared_ptr<Environment> GetGlobalEnvironment() { return globals_; }
  std::shared_ptr<Environment> GetCurrentEnvironment() { return environment_; }

 private:
  std::shared_ptr<Environment> globals_;
  std::shared_ptr<Environment> environment_;

  /// @brief Validates that a unary operand is indeed a number.
  void CheckNumberOperand(parsing::Token operator_used, std::any operand);
  /// @brief Validates that binary operands are indeed both numbers.
  void CheckNumberOperands(
      parsing::Token operator_used, std::any left_side, std::any right_side);
  bool IsTruthy(std::any object);
  /// @brief Evaluate a given expression.
  std::any Evaluate(parsed::Expression* expression);
  /// @brief Check to see if two values are equal.
  bool IsEqual(std::any left_side, std::any right_side);

  /// @brief Stringify any given interpreted object.
  std::string Stringify(std::any value);
};

}  // namespace runtime
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_RUNTIME_INTERPRETER_H_
