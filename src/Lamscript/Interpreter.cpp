#include <Lamscript/Interpreter.h>

#include <any>
#include <string>
#include <typeinfo>

#include <Lamscript/Lamscript.h>
#include <Lamscript/Statement.h>

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

// --------------------------------- EXPRESSIONS -------------------------------

std::any Interpreter::VisitAssignExpression(Assign* expression) {
  std::any value = Evaluate(expression->GetValue());
  environment_->AssignVariable(expression->GetName(), value);
  return value;
}

std::any Interpreter::VisitLiteralExpression(Literal* expression) {
  return expression->GetValue();
}

std::any Interpreter::VisitGroupingExpression(Grouping* expression) {
  return Evaluate(expression->GetExpression());
}

std::any Interpreter::VisitVariableExpression(Variable* variable) {
  return environment_->GetVariable(variable->GetName());
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

  if (left_side.type() == Number || right_side.type() == Number) {
    CheckNumberOperands(expression->GetOperator(), left_side, right_side);
  }

  switch (expression->GetOperator().Type) {
    case MINUS:
    {
      return AnyAs<double>(left_side) - AnyAs<double>(right_side);
    }
    case PLUS:
    {
      if (left_side.type() == Number && right_side.type() == Number) {
        return AnyAs<double>(left_side) + AnyAs<double>(right_side);
      }

      if (left_side.type() == String && right_side.type() == String) {
        return AnyAs<std::string>(left_side) + AnyAs<std::string>(right_side);
      }

      throw RuntimeError(
          expression->GetOperator(),
          "Operands must be two numbers or strings.");
    }
    case SLASH:
    {
      double divisor = AnyAs<double>(right_side);
      if (divisor == 0) {
        throw RuntimeError(
            expression->GetOperator(), "Divide by 0 error.");
      }

      return AnyAs<double>(left_side) / divisor;
    }
    case STAR:
    {
      return AnyAs<double>(left_side) * AnyAs<double>(right_side);
    }
    case GREATER:
    {
      return AnyAs<double>(left_side) > AnyAs<double>(right_side);
    }
    case GREATER_EQUAL:
    {
      return AnyAs<double>(left_side) >= AnyAs<double>(right_side);
    }
    case LESS:
    {
      return AnyAs<double>(left_side) < AnyAs<double>(right_side);
    }
    case LESS_EQUAL:
    {
      return AnyAs<double>(left_side) <= AnyAs<double>(right_side);
    }
    case BANG_EQUAL:
    {
      return !IsEqual(left_side, right_side);
    }
    case EQUAL_EQUAL:
    {
      return IsEqual(left_side, right_side);
    }
  }

  return nullptr;
}

std::any Interpreter::VisitLogicalExpression(Logical* expression) {
  std::any left_side = Evaluate(expression->GetLeftOperand());
  bool left_is_truthy = IsTruthy(left_side);

  if (expression->GetLogicalOperator().Type == OR) {
    if (left_is_truthy) {
      return left_side;
    }
  } else {
    if (!left_is_truthy) {
      return left_side;
    }
  }

  return Evaluate(expression->GetRightOperand());
}

// --------------------------------- STATEMENTS --------------------------------

std::any Interpreter::VisitBlockStatement(Block* statement) {
  ExecuteBlock(statement->GetStatements(), new Environment(environment_));
  return NULL;
}

std::any Interpreter::VisitPrintStatement(Print* statement) {
  std::any value = Evaluate(statement->GetExpression());
  std::cout << Stringify(value) << std::endl;
  return NULL;
}

std::any Interpreter::VisitExpressionStatement(ExpressionStatement* statement) {
  Evaluate(statement->GetExpression());
  return NULL;
}

std::any Interpreter::VisitVariableStatement(VariableStatement* statement) {
  std::any value;

  if (statement->GetInitializer() != nullptr) {
    value = Evaluate(statement->GetInitializer());
  }

  environment_->SetVariable(statement->GetName(), value);
  return NULL;
}

std::any Interpreter::VisitIfStatement(If* statement) {
  if (IsTruthy(Evaluate(statement->GetCondition()))) {
    Execute(statement->GetThenBranch());
  } else if (statement->GetElseBranch() != nullptr) {
    Execute(statement->GetElseBranch());
  }

  return NULL;
}

void Interpreter::Interpret(std::vector<Statement*> statements) {
  try {
    for (Statement* statement : statements) {
      Execute(statement);
    }
  } catch (RuntimeError error) {
    Lamscript::RuntimeError(error);
  }
}

void Interpreter::Execute(Statement* statement) {
  statement->Accept(this);
}

void Interpreter::ExecuteBlock(
    std::vector<Statement*> statements, Environment* current_env) {
  Environment* previous = environment_;

  try {
    environment_ = current_env;

    for (Statement* statement : statements) {
      Execute(statement);
    }
  } catch(...) {
    /// @todo Implement actual error handling inside of blocks.
    std::cout << "Some error happened in a local scope lol." << std::endl;
  }

  environment_ = previous;
}


// ---------------------------------- PRIVATE ----------------------------------

void Interpreter::CheckNumberOperand(Token operator_used, std::any operand) {
  if (operand.type() != Number) {
    throw RuntimeError(operator_used, "Operand must be a number.");
  }
}

void Interpreter::CheckNumberOperands(
    Token operator_used, std::any left_side, std::any right_side) {
  if (left_side.type() != Number || right_side.type() != Number) {
    throw RuntimeError(operator_used, "Operands must both be numbers.");
  }
}

bool Interpreter::IsTruthy(std::any object) {
  if (!object.has_value()) {
    return false;
  }
  if (object.type() == Boolean) {
    return AnyAs<bool&>(object);
  }

  if (object.type() == Number) {
    return AnyAs<double&>(object) != 0;
  }

  if (object.type() == String) {
    return !AnyAs<std::string&>(object).empty();
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

std::string Interpreter::Stringify(std::any object) {
  if (!object.has_value()) {
    return "nil";
  }

  if (object.type() == Number) {
    return std::to_string(AnyAs<double&>(object));
  }

  if (object.type() == Boolean) {
    return AnyAs<bool&>(object) ? "true" : "false";
  }

  return AnyAs<std::string>(object);
}

}  // namespace lamscript
