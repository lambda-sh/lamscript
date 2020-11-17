#include <Lamscript/runtime/Interpreter.h>

#include <any>
#include <math.h>
#include <string>
#include <typeinfo>

#include <Lamscript/lib/Globals.h>
#include <Lamscript/parsed/LamscriptCallable.h>
#include <Lamscript/parsed/LamscriptFunction.h>
#include <Lamscript/parsed/Statement.h>
#include <Lamscript/runtime/Lamscript.h>

namespace lamscript {
namespace runtime {


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
Interpreter::Interpreter()
    : globals_(new Environment()), environment_(globals_) {
  globals_->SetVariable(
      parsing::Token{parsing::FUN, "clock", nullptr, 0},
      reinterpret_cast<parsed::LamscriptCallable*>(new lib::Clock()));
}


// --------------------------------- EXPRESSIONS -------------------------------

std::any Interpreter::VisitAssignExpression(parsed::Assign* expression) {
  std::any value = Evaluate(expression->GetValue());
  environment_->AssignVariable(expression->GetName(), value);
  return value;
}

std::any Interpreter::VisitLiteralExpression(parsed::Literal* expression) {
  return expression->GetValue();
}

std::any Interpreter::VisitGroupingExpression(parsed::Grouping* expression) {
  return Evaluate(expression->GetExpression());
}

std::any Interpreter::VisitVariableExpression(parsed::Variable* variable) {
  return environment_->GetVariable(variable->GetName());
}

std::any Interpreter::VisitUnaryExpression(parsed::Unary* expression) {
  std::any right_side = Evaluate(expression);

  switch (expression->GetUnaryOperator().Type) {
    case parsing::BANG:
      return !IsTruthy(right_side);
    case parsing::MINUS:
      CheckNumberOperand(expression->GetUnaryOperator(), right_side);
      return -(AnyAs<double>(right_side));
  }

  return nullptr;
}

std::any Interpreter::VisitBinaryExpression(parsed::Binary* expression) {
  std::any left_side = Evaluate(expression->GetLeftSide());
  std::any right_side = Evaluate(expression->GetRightSide());

  if (left_side.type() == Number || right_side.type() == Number) {
    CheckNumberOperands(expression->GetOperator(), left_side, right_side);
  }

  switch (expression->GetOperator().Type) {
    case parsing::MINUS:
    {
      return AnyAs<double>(left_side) - AnyAs<double>(right_side);
    }
    case parsing::PLUS:
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
    case parsing::SLASH:
    {
      double divisor = AnyAs<double>(right_side);
      if (divisor == 0) {
        throw RuntimeError(
            expression->GetOperator(), "Divide by 0 error.");
      }

      return AnyAs<double>(left_side) / divisor;
    }
    case parsing::STAR:
    {
      return AnyAs<double>(left_side) * AnyAs<double>(right_side);
    }
    case parsing::MODULUS:
    {
      return fmod(AnyAs<double>(left_side), AnyAs<double>(right_side));
    }
    case parsing::GREATER:
    {
      return AnyAs<double>(left_side) > AnyAs<double>(right_side);
    }
    case parsing::GREATER_EQUAL:
    {
      return AnyAs<double>(left_side) >= AnyAs<double>(right_side);
    }
    case parsing::LESS:
    {
      return AnyAs<double>(left_side) < AnyAs<double>(right_side);
    }
    case parsing::LESS_EQUAL:
    {
      return AnyAs<double>(left_side) <= AnyAs<double>(right_side);
    }
    case parsing::BANG_EQUAL:
    {
      return !IsEqual(left_side, right_side);
    }
    case parsing::EQUAL_EQUAL:
    {
      return IsEqual(left_side, right_side);
    }
  }

  return nullptr;
}

std::any Interpreter::VisitLogicalExpression(parsed::Logical* expression) {
  std::any left_side = Evaluate(expression->GetLeftOperand());
  bool left_is_truthy = IsTruthy(left_side);

  if (expression->GetLogicalOperator().Type == parsing::OR) {
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

std::any Interpreter::VisitCallExpression(parsed::Call* expression) {
  std::any callee = Evaluate(expression->GetCallee());
  std::vector<std::any> arguments;

  for (auto&& argument : expression->GetArguments()) {
    arguments.push_back(Evaluate(argument.get()));
  }

  try {
    parsed::LamscriptCallable* callable = AnyAs<
        parsed::LamscriptCallable*>(callee);

    if (callable->Arity() != arguments.size()) {
      throw RuntimeError(
          expression->GetParentheses(),
          "Expected " + std::to_string(callable->Arity())
              + " arguments but got " + std::to_string(arguments.size()) + ".");
    }

    return callable->Call(this, arguments);
  } catch (std::bad_any_cast error) {
    throw RuntimeError(
        expression->GetParentheses(),
        "Can only call functions and classes;");
  }
}

// --------------------------------- STATEMENTS --------------------------------

std::any Interpreter::VisitBlockStatement(parsed::Block* statement) {
  ExecuteBlock(
      statement->GetStatements(), std::make_shared<Environment>(environment_));
  return NULL;
}

std::any Interpreter::VisitPrintStatement(parsed::Print* statement) {
  std::any value = Evaluate(statement->GetExpression());
  std::cout << Stringify(value) << std::endl;
  return NULL;
}

std::any Interpreter::VisitExpressionStatement(
    parsed::ExpressionStatement* statement) {
  Evaluate(statement->GetExpression());
  return NULL;
}

std::any Interpreter::VisitVariableStatement(
    parsed::VariableStatement* statement) {
  std::any value;

  if (statement->GetInitializer() != nullptr) {
    value = Evaluate(statement->GetInitializer());
  }

  environment_->SetVariable(statement->GetName(), value);
  return NULL;
}

std::any Interpreter::VisitIfStatement(parsed::If* statement) {
  if (IsTruthy(Evaluate(statement->GetCondition()))) {
    Execute(statement->GetThenBranch());
  } else if (statement->GetElseBranch() != nullptr) {
    Execute(statement->GetElseBranch());
  }

  return NULL;
}

std::any Interpreter::VisitWhileStatement(parsed::While* statement) {
  while (IsTruthy(Evaluate(statement->GetCondition()))) {
    Execute(statement->GetBody());
  }

  return NULL;
}

std::any Interpreter::VisitFunctionStatement(parsed::Function* statement) {
  parsed::LamscriptCallable* func = new parsed::LamscriptFunction(
      statement, environment_);
  environment_->SetVariable(statement->GetName(), func);
  return NULL;
}

std::any Interpreter::VisitReturnStatement(parsed::Return* statement) {
  std::any value = nullptr;
  if (statement->GetValue() != nullptr) {
    value = Evaluate(statement->GetValue());
  }

  throw parsed::LamscriptReturnValue(value);
}

void Interpreter::Interpret(
    const std::vector<std::unique_ptr<parsed::Statement>>& statements) {
  try {
    for (auto&& statement : statements) {
      Execute(statement.get());
    }
  } catch (const RuntimeError& error) {
    Lamscript::RuntimeError(error);
  }
}

void Interpreter::Execute(parsed::Statement* statement) {
  statement->Accept(this);
}

void Interpreter::ExecuteBlock(
    const std::vector<std::unique_ptr<parsed::Statement>>& statements,
    std::shared_ptr<Environment> current_env) {
  std::shared_ptr<Environment> previous = environment_;

  // Forwards return value statements up the chain so that function calls that
  // recurse get their environment reset once the recursion starts unwinding.
  try {
    environment_ = current_env;

    for (auto&& statement : statements) {
      Execute(statement.get());
    }
  } catch(const RuntimeError& error) {
    Lamscript::RuntimeError(error);
  } catch (const parsed::LamscriptReturnValue& returned_value) {
    environment_ = previous;
    throw returned_value;
  }

  // Needed to ensure that if no error or return value happens, function calls
  environment_ = previous;
}

// ---------------------------------- PRIVATE ----------------------------------

void Interpreter::CheckNumberOperand(
    parsing::Token operator_used, std::any operand) {
  if (operand.type() != Number) {
    throw RuntimeError(operator_used, "Operand must be a number.");
  }
}

void Interpreter::CheckNumberOperands(
    parsing::Token operator_used, std::any left_side, std::any right_side) {
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

std::any Interpreter::Evaluate(parsed::Expression* expression) {
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

  if (object.type() == typeid(parsed::LamscriptCallable*)) {
    return AnyAs<parsed::LamscriptCallable*>(object)->ToString();
  }

  if (object.type() == typeid(parsed::LamscriptFunction*)) {
    return AnyAs<parsed::LamscriptFunction*>(object)->ToString();
  }

  return AnyAs<std::string>(object);
}

}  // namespace runtime
}  // namespace lamscript
