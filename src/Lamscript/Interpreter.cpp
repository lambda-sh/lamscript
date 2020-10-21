#include <Lamscript/Interpreter.h>

#include <any>
#include <typeinfo>

namespace lamscript {

// ---------------------------------- INTERNAL ---------------------------------

namespace {
  template<class ReturnType>
  ReturnType AnyAs(std::any any_object) {
    return std::any_cast<ReturnType>(any_object);
  }

  const std::type_info& Boolean = typeid(bool);
  const std::type_info& Number = typeid(double);
  const std::type_info& String = typeid(std::string);

}  // namespace

// ---------------------------------- PUBLIC -----------------------------------

std::any Interpreter::VisitLiteralExpression(Literal* expression) {
  return expression->GetValue();
}

std::any Interpreter::VisitGroupingExpression(Grouping* expression) {
  return Evaluate(expression->GetExpression());
}


std::any Interpreter::VisitUnaryExpression(Unary* expression) {
  std::any right_side = Evaluate(expression);

  switch (expression->GetUnaryOperator().Type) {
    case BANG:
      return !IsTruthy(right_side);
    case MINUS:
      CheckNumberOperand(expression->GetUnaryOperator(), right_side);
      return -(AnyAs<double>(right_side));
  }

  return nullptr;
}

std::any Interpreter::VisitBinaryExpression(Binary* expression) {
    std::any left_side = Evaluate(expression->GetLeftSide());
    std::any right_side = Evaluate(expression->GetRightSide());

    if (left_side.type() == Number) {
      CheckNumberOperands(expression->GetOperator(), left_side, right_side);
    }

    switch (expression->GetOperator().Type) {
      case MINUS:
        return AnyAs<double>(left_side) - AnyAs<double>(right_side);
      case PLUS:
        if (left_side.type() == Number && right_side.type() == Number) {
          return AnyAs<double>(left_side) + AnyAs<double>(right_side);
        }

        if (left_side.type() == String && right_side.type() == String) {
          return AnyAs<std::string>(left_side) + AnyAs<std::string>(right_side);
        }

        throw new RuntimeError(
            expression->GetOperator(),
            "Operands must be two numbers or strings.");
      case SLASH:
        return AnyAs<double>(left_side) / AnyAs<double>(right_side);
      case STAR:
        return AnyAs<double>(left_side) * AnyAs<double>(right_side);
      case GREATER:
        return AnyAs<double>(left_side) > AnyAs<double>(right_side);
      case GREATER_EQUAL:
        return AnyAs<double>(left_side) >= AnyAs<double>(right_side);
      case LESS:
        return AnyAs<double>(left_side) < AnyAs<double>(right_side);
      case LESS_EQUAL:
        return AnyAs<double>(left_side) <= AnyAs<double>(right_side);
      case BANG_EQUAL:
        return !IsEqual(left_side, right_side);
      case EQUAL_EQUAL:
        return IsEqual(left_side, right_side);
    }

    return nullptr;
  }


// ---------------------------------- PRIVATE ----------------------------------

void Interpreter::CheckNumberOperand(Token operator_used, std::any operand) {
  if (operand.type() != Number) {
    throw new RuntimeError(operator_used, "Operand must be a number.");
  }
}

void Interpreter::CheckNumberOperands(
    Token operator_used, std::any left_side, std::any right_side) {
  if (left_side.type() != Number || right_side.type() != Number) {
    throw new RuntimeError(operator_used, "Operands must both be numbers.");
  }
}

bool Interpreter::IsTruthy(std::any object) {
  if (!object.has_value()) {
    return false;
  }

  if (object.type() == Number) {
    return AnyAs<bool>(object);
  }

  return true;
}

std::any Interpreter::Evaluate(Expression* expression) {
  return expression->Accept(this);
}

/// Order of checks follow as:
/// 1. Are both values null?
/// 2. Is only the left side null?
/// 3. Are both values of the same type?
/// 4. Boolean comparison.
/// 5. Number comparison.
/// 6. String comparison.
bool Interpreter::IsEqual(std::any left_side, std::any right_side) {
  if (!left_side.has_value() && !right_side.has_value()) {
    return true;
  }

  if (!left_side.has_value()) {
    return false;
  }

  if (left_side.type() != right_side.type()) {
    return false;
  }

  if (left_side.type() == Boolean) {
    return AnyAs<bool>(left_side) == AnyAs<bool>(right_side);
  }

  if (left_side.type() == Number) {
    return AnyAs<double&>(left_side) == AnyAs<double&>(right_side);
  }

  if (left_side.type() == String) {
    return AnyAs<std::string&>(left_side) == AnyAs<std::string&>(right_side);
  }

  return false;
}

}  // namespace lamscript
