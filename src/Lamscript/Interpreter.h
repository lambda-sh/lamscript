#ifndef SRC_LAMSCRIPT_INTERPRETER_H_
#define SRC_LAMSCRIPT_INTERPRETER_H_

#include <any>
#include <typeinfo>

#include <Lamscript/Expression.h>
#include <Lamscript/Lamscript.h>
#include <Lamscript/RuntimeError.h>

namespace lamscript {

class Interpreter : Visitor {
 public:
  std::any VisitLiteralExpression(Literal* expresesion) override;
  std::any VisitGroupingExpression(Grouping* expression) override;
  std::any VisitUnaryExpression(Unary* expression) override;
  std::any VisitBinaryExpression(Binary* expression) override;

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
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_INTERPRETER_H_
