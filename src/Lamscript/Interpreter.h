#ifndef SRC_LAMSCRIPT_INTERPRETER_H_
#define SRC_LAMSCRIPT_INTERPRETER_H_

#include <any>
#include <memory>
#include <typeinfo>

#include <Lamscript/Environment.h>
#include <Lamscript/RuntimeError.h>
#include <Lamscript/Visitor.h>
#include <Lamscript/parsable/Expression.h>
#include <Lamscript/parsable/Statement.h>

namespace lamscript {

class Interpreter : ExpressionVisitor, StatementVisitor {
 public:
  Interpreter();
  // Implemented Expressions.

  std::any VisitAssignExpression(parsable::Assign* expression) override;
  std::any VisitLiteralExpression(parsable::Literal* expression) override;
  std::any VisitGroupingExpression(parsable::Grouping* expression) override;
  std::any VisitUnaryExpression(parsable::Unary* expression) override;
  std::any VisitBinaryExpression(parsable::Binary* expression) override;
  std::any VisitVariableExpression(parsable::Variable* expression) override;
  std::any VisitLogicalExpression(parsable::Logical* expression) override;
  std::any VisitCallExpression(parsable::Call* expression) override;

  // Implemented Statements

  std::any VisitBlockStatement(parsable::Block* statement) override;
  std::any VisitExpressionStatement(
      parsable::ExpressionStatement* statement) override;
  std::any VisitPrintStatement(parsable::Print* statement) override;
  std::any VisitVariableStatement(
      parsable::VariableStatement* statement) override;
  std::any VisitIfStatement(parsable::If* statement) override;
  std::any VisitWhileStatement(parsable::While* statement) override;
  std::any VisitFunctionStatement(parsable::Function* statement) override;

  /// @todo (C3NZ) Implement the rest of the visitor pattern.

  std::any VisitGetExpression(parsable::Get* expression) override {};
  std::any VisitSetExpression(parsable::Set* expression) override {};
  std::any VisitSuperExpression(parsable::Super* expression) override {};
  std::any VisitThisExpression(parsable::This* expression) override {};

  // Statements
  std::any VisitClassStatement(parsable::Class* statement) override {};
  std::any VisitReturnStatement(parsable::Return* statement) override {};

  // Primary external API

  void Interpret(
      const std::vector<std::unique_ptr<parsable::Statement>>& statements);
  void Execute(parsable::Statement* statement);
  void ExecuteBlock(
      const std::vector<std::unique_ptr<parsable::Statement>>& statements,
      Environment* current_env);

  Environment* GetGlobalEnvironment() const { return globals_; }

 private:
  Environment* globals_;
  Environment* environment_;

  /// @brief Validates that a unary operand is indeed a number.
  void CheckNumberOperand(parsing::Token operator_used, std::any operand);
  /// @brief Validates that binary operands are indeed both numbers.
  void CheckNumberOperands(
      parsing::Token operator_used, std::any left_side, std::any right_side);
  bool IsTruthy(std::any object);
  /// @brief Evaluate a given expression.
  std::any Evaluate(parsable::Expression* expression);
  /// @brief Check to see if two values are equal.
  bool IsEqual(std::any left_side, std::any right_side);

  /// @brief Stringify any given interpreted object.
  std::string Stringify(std::any value);
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_INTERPRETER_H_
