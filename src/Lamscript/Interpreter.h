#ifndef SRC_LAMSCRIPT_INTERPRETER_H_
#define SRC_LAMSCRIPT_INTERPRETER_H_

#include <any>
#include <typeinfo>

#include <Lamscript/Expression.h>
#include <Lamscript/RuntimeError.h>

namespace lamscript {

class Interpreter : Visitor {
 public:
  std::any VisitLiteralExpression(Literal* expression) override {
    return expression->GetValue();
  }

  std::any VisitGroupingExpression(Grouping *expression) override {
    return Evaluate(expression->GetExpression());
  }

  std::any VisitUnaryExpression(Unary *expression) override {
    std::any right_side = Evaluate(expression);

    switch (expression->GetUnaryOperator().Type) {
      case BANG:
        return !IsTruthy(right_side);
      case MINUS:
        CheckNumberOperand(expression->GetUnaryOperator(), right_side);
        return -(std::any_cast<double>(right_side));
    }

    return nullptr;
  }

  std::any VisitBinaryExpression(Binary *expression) override {
    std::any left_side = Evaluate(expression->GetLeftSide());
    std::any right_side = Evaluate(expression->GetRightSide());

    switch (expression->GetOperator().Type) {
      case MINUS:
        return std::any_cast<double>(left_side) - std::any_cast<double>(
            right_side);
      case PLUS:
        if ((left_side.type() == typeid(double)) &&
            (right_side.type() == typeid(double))) {
         return std::any_cast<double>(left_side) + std::any_cast<double>(
            right_side);
        }

        if (left_side.type() == typeid(std::string)
            && right_side.type() == typeid(std::string)) {
          return std::any_cast<std::string>(left_side)
              + std::any_cast<std::string>(right_side);
        }
      case SLASH:
        return std::any_cast<double>(left_side) / std::any_cast<double>(
            right_side);
      case STAR:
        return std::any_cast<double>(left_side) * std::any_cast<double>(
            right_side);

      case GREATER:
        return std::any_cast<double>(left_side) > std::any_cast<double>(
            right_side);
      case GREATER_EQUAL:
        return std::any_cast<double>(left_side) >= std::any_cast<double>(
            right_side);
      case LESS:
        return std::any_cast<double>(left_side) < std::any_cast<double>(
            right_side);
      case LESS_EQUAL:
        return std::any_cast<double>(left_side) <= std::any_cast<double>(
            right_side);
      case BANG_EQUAL:
        return !IsEqual(left_side, right_side);
      case EQUAL_EQUAL:
        return IsEqual(left_side, right_side);
    }

    return nullptr;
  }

 private:
  void CheckNumberOperand(Token operator_used, std::any operand) {
    if (operand.type() != typeid(double)) {
      throw new RuntimeError(operator_used, "Operand must be a number.");
    }
  }

  bool IsTruthy(std::any object) {
    if (!object.has_value()) {
      return false;
    }

    if (object.type() == typeid(double)) {
      return static_cast<bool>(std::any_cast<double>(object));
    }

    return true;
  }
  std::any Evaluate(Expression* expression) {
    return expression->Accept(this);
  }

  /// @brief Check to see if two values are equal.
  bool IsEqual(std::any left_side, std::any right_side) {
    if (!left_side.has_value() && !right_side.has_value()) {
      return true;
    }

    if (!left_side.has_value()) {
      return false;
    }

    if (left_side.type() != right_side.type()) {
      return false;
    }

    if (left_side.type() == typeid(bool)) {
      return std::any_cast<bool>(left_side) == std::any_cast<bool>(right_side);
    }

    if (left_side.type() == typeid(double)) {
      return std::any_cast<double&>(left_side) == std::any_cast<double&>(
          right_side);
    }

    if (left_side.type() == typeid(std::string)) {
      return std::any_cast<std::string&>(left_side) == std::any_cast<
          std::string&>(right_side);
    }
  }
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_INTERPRETER_H_
