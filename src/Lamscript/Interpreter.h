#ifndef SRC_LAMSCRIPT_INTERPRETER_H_
#define SRC_LAMSCRIPT_INTERPRETER_H_

#include <any>
#include <typeinfo>

#include <Lamscript/Expression.h>
#include <Lamscript/RuntimeError.h>
#include <Lamscript/Visitor.h>

namespace lamscript {

class Interpreter : ExpressionVisitor {
 public:
  std::any VisitLiteralExpression(Literal* expression) override;
  std::any VisitGroupingExpression(Grouping* expression) override;
  std::any VisitUnaryExpression(Unary* expression) override;
  std::any VisitBinaryExpression(Binary* expression) override;

  /// @todo (C3NZ) Implement the rest of the visitor pattern.

  std::any VisitAssignExpression(Assign* expression) override {};
  std::any VisitCallExpression(Call* expression) override {};
  std::any VisitGetExpression(Get* expression) override {};
  std::any VisitLogicalExpression(Logical* expression) override {};
  std::any VisitSetExpression(Set* expression) override {};
  std::any VisitSuperExpression(Super* expression) override {};
  std::any VisitThisExpression(This* expression) override {};
  std::any VisitVariableExpression(Variable* expression) override {};

  void Interpret(Expression* expression);

 private:
  /// @brief Validates that a unary operand is indeed a number.
  void CheckNumberOperand(Token operator_used, std::any operand);
  /// @brief Validates that binary operands are indeed both numbers.
  void CheckNumberOperands(
      Token operator_used, std::any left_side, std::any right_side);
  bool IsTruthy(std::any object);
  /// @brief Evaluate a given expression.
  std::any Evaluate(Expression* expression);
  /// @brief Check to see if two values are equal.
  bool IsEqual(std::any left_side, std::any right_side);

  /// @brief Stringify any given interpreted object.
  std::string Stringify(std::any value);
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_INTERPRETER_H_
