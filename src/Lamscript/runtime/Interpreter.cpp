#include <Lamscript/runtime/Interpreter.h>

#include <any>
#include <math.h>
#include <string>
#include <typeinfo>

#include <Lamscript/lib/Globals.h>
#include <Lamscript/parsed/LamscriptCallable.h>
#include <Lamscript/parsed/LamscriptClass.h>
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

typedef std::shared_ptr<parsed::LamscriptInstance> SharedLamscriptInstance;
typedef std::shared_ptr<parsed::LamscriptCallable> SharedLamscriptCallable;
typedef std::shared_ptr<parsed::LamscriptClass> SharedLamscriptClass;

const std::type_info& LS_TYPE_BOOLEAN = typeid(bool);
const std::type_info& LS_TYPE_NUMBER = typeid(double);
const std::type_info& LS_TYPE_STRING = typeid(std::string);
const std::type_info& LS_TYPE_INSTANCE = typeid(SharedLamscriptInstance);
const std::type_info& LS_TYPE_CALLABLE = typeid(SharedLamscriptCallable);
const std::type_info& LS_TYPE_CLASS = typeid(SharedLamscriptClass);

}  // namespace

// ---------------------------------- PUBLIC -----------------------------------

Interpreter::Interpreter()
    : globals_(new Environment()), environment_(globals_) {
  globals_->SetVariable(
      parsing::Token{parsing::FUN, "clock", nullptr, 0},
      SharedLamscriptCallable(new lib::Clock()));
}

// --------------------------------- EXPRESSIONS -------------------------------

std::any Interpreter::VisitAssignExpression(parsed::Assign* expression) {
  std::any value = Evaluate(expression->GetValue());

  try {
    size_t& distance = locals_.at(expression);
    environment_->AssignVariableAtScope(distance, expression->GetName(), value);
  } catch (const std::out_of_range& error) {
    globals_->AssignVariable(expression->GetName(), value);
  }

  return value;
}

std::any Interpreter::VisitLiteralExpression(parsed::Literal* expression) {
  return expression->GetValue();
}

std::any Interpreter::VisitGroupingExpression(parsed::Grouping* expression) {
  return Evaluate(expression->GetExpression());
}

std::any Interpreter::VisitVariableExpression(parsed::Variable* variable) {
  return LookupVariable(variable->GetName(), variable);
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

  if (left_side.type() == LS_TYPE_NUMBER
      || right_side.type() == LS_TYPE_NUMBER) {
    CheckNumberOperands(expression->GetOperator(), left_side, right_side);
  }

  const std::type_info& left_type = left_side.type();
  const std::type_info& right_type = right_side.type();

  switch (expression->GetOperator().Type) {
    case parsing::MINUS:
    {
      return AnyAs<double>(left_side) - AnyAs<double>(right_side);
    }
    case parsing::PLUS:
    {
      if (left_type == LS_TYPE_NUMBER && right_type == LS_TYPE_NUMBER) {
        return AnyAs<double>(left_side) + AnyAs<double>(right_side);
      }

      if (left_type == LS_TYPE_STRING && right_type == LS_TYPE_STRING) {
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
    SharedLamscriptCallable callable = AnyAs<SharedLamscriptCallable>(callee);

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

std::any Interpreter::VisitLambdaExpression(
    parsed::LambdaExpression* expression) {

  parsed::Function* func(
      static_cast<parsed::Function*>(expression->GetFunctionStatement()));

  Execute(func);

  return environment_->GetVariable(func->GetName());
}

std::any Interpreter::VisitGetExpression(parsed::Get* getter) {
  std::any object = Evaluate(getter->GetObject().get());

  if (object.type() == LS_TYPE_CALLABLE) {
    auto class_def = static_cast<parsed::LamscriptClass*>(
        AnyAs<SharedLamscriptCallable>(object).get());

    try {
      auto func = class_def->LookupMethod(getter->GetName().Lexeme);

      if (!func.IsStatic()) {
        throw RuntimeError(
            getter->GetName(),
            "Only instances can access non-static class methods.");
      }

      if (func.IsGetter()) {
        return func.Call(this, {{}});
      }

      return func.Bind(nullptr);
    } catch (std::out_of_range& err) {
      throw RuntimeError(
          getter->GetName(), "No static function found on class.");
    }
  }

  if (object.type() != LS_TYPE_INSTANCE) {
    throw RuntimeError(
        getter->GetName(), "Only instances have properties.");
  }

  std::any instance_field = AnyAs<SharedLamscriptInstance>(
      object)->GetField(getter->GetName());

  if (instance_field.type() == LS_TYPE_CALLABLE) {
    auto func = static_cast<parsed::LamscriptFunction*>(
        AnyAs<SharedLamscriptCallable>(instance_field).get());

    if (func->IsGetter()) {
      return func->Call(this, {{}});
    }
  }

  return instance_field;
}

std::any Interpreter::VisitSetExpression(parsed::Set* setter) {
  std::any object = Evaluate(setter->GetObject().get());

  if (object.type() != LS_TYPE_INSTANCE) {
    throw RuntimeError(setter->GetName(), "Only instances have fields.");
  }

  std::any value = Evaluate(setter->GetValue());
  AnyAs<SharedLamscriptInstance>(object)->SetField(setter->GetName(), value);

  return value;
}

std::any Interpreter::VisitThisExpression(parsed::This* this_expr) {
  return LookupVariable(this_expr->GetKeyword(), this_expr);
}

// --------------------------------- STATEMENTS --------------------------------

std::any Interpreter::VisitBlockStatement(parsed::Block* statement) {
  ExecuteBlock(
      statement->GetStatements(), std::make_shared<Environment>(environment_));
  return nullptr;
}

std::any Interpreter::VisitPrintStatement(parsed::Print* statement) {
  std::any value = Evaluate(statement->GetExpression());
  std::cout << Stringify(value) << std::endl;
  return nullptr;
}

std::any Interpreter::VisitExpressionStatement(
    parsed::ExpressionStatement* statement) {
  Evaluate(statement->GetExpression());
  return nullptr;
}

std::any Interpreter::VisitVariableStatement(
    parsed::VariableStatement* statement) {
  std::any value;

  if (statement->GetInitializer() != nullptr) {
    value = Evaluate(statement->GetInitializer());
  }

  environment_->SetVariable(statement->GetName(), value);
  return nullptr;
}

std::any Interpreter::VisitIfStatement(parsed::If* statement) {
  if (IsTruthy(Evaluate(statement->GetCondition()))) {
    Execute(statement->GetThenBranch());
  } else if (statement->GetElseBranch() != nullptr) {
    Execute(statement->GetElseBranch());
  }

  return nullptr;
}

std::any Interpreter::VisitWhileStatement(parsed::While* statement) {
  while (IsTruthy(Evaluate(statement->GetCondition()))) {
    Execute(statement->GetBody());
  }

  return nullptr;
}

std::any Interpreter::VisitFunctionStatement(parsed::Function* statement) {
  std::shared_ptr<parsed::LamscriptCallable> func(
      new parsed::LamscriptFunction(
          std::shared_ptr<parsed::Function>(statement), environment_, false));
  environment_->SetVariable(statement->GetName(), func);
  return nullptr;
}

std::any Interpreter::VisitReturnStatement(parsed::Return* statement) {
  std::any value = nullptr;
  if (statement->GetValue() != nullptr) {
    value = Evaluate(statement->GetValue());
  }

  throw parsed::LamscriptReturnValue(value);
}


std::any Interpreter::VisitClassStatement(parsed::Class* class_def) {
  std::unordered_map<
      std::string, parsed::LamscriptFunction> methods;

  std::any super_class;
  SharedLamscriptClass super_class_def = nullptr;

  if (class_def->GetSuperClass() != nullptr) {
    super_class = Evaluate(class_def->GetSuperClass());

    if (super_class.type() == LS_TYPE_CALLABLE) {
      SharedLamscriptCallable callable = AnyAs<
          SharedLamscriptCallable>(super_class);

          if (auto x = dynamic_cast<parsed::LamscriptClass*>(callable.get())) {
            super_class_def.reset(x);
          }
    } else {
      throw RuntimeError(
          class_def->GetName(), "Superclass must be a class.");
    }
  }

  for (auto& method : class_def->GetMethods()) {
    parsed::LamscriptFunction func(
        method,
        environment_,
        method->GetName().Lexeme.compare("constructor") == 0);
    methods.insert(std::make_pair(method->GetName().Lexeme, func));
  }

  std::shared_ptr<parsed::LamscriptCallable> lam_class(
      new parsed::LamscriptClass(
          class_def->GetName().Lexeme,
          super_class_def,
          std::move(methods)));

  environment_->SetVariable(class_def->GetName(), lam_class);
  return nullptr;
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

void Interpreter::Resolve(parsed::Expression* expression, size_t distance) {
  locals_[expression] = distance;
}

// ---------------------------------- PRIVATE ----------------------------------

void Interpreter::CheckNumberOperand(
    parsing::Token operator_used, std::any operand) {
  if (operand.type() != LS_TYPE_NUMBER) {
    throw RuntimeError(operator_used, "Operand must be a number.");
  }
}

void Interpreter::CheckNumberOperands(
    parsing::Token operator_used, std::any left_side, std::any right_side) {
  if (left_side.type() != LS_TYPE_NUMBER
      || right_side.type() != LS_TYPE_NUMBER) {
    throw RuntimeError(operator_used, "Operands must both be numbers.");
  }
}

bool Interpreter::IsTruthy(std::any object) {
  if (!object.has_value()) {
    return false;
  }

  const std::type_info& type = object.type();

  if (type == LS_TYPE_BOOLEAN) {
    return AnyAs<bool&>(object);
  }

  if (type == LS_TYPE_NUMBER) {
    return AnyAs<double&>(object) != 0;
  }

  if (type == LS_TYPE_STRING) {
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

  const std::type_info& left_type = left_side.type();
  const std::type_info& right_type = right_side.type();


  if (left_type != right_type) {
    return false;
  }

  if (left_type == LS_TYPE_BOOLEAN) {
    return AnyAs<bool>(left_side) == AnyAs<bool>(right_side);
  }

  if (left_type == LS_TYPE_NUMBER) {
    return AnyAs<double&>(left_side) == AnyAs<double&>(right_side);
  }

  if (left_type == LS_TYPE_STRING) {
    return AnyAs<std::string&>(left_side) == AnyAs<std::string&>(right_side);
  }

  return false;
}

std::string Interpreter::Stringify(std::any object) {
  if (!object.has_value()) {
    return "nil";
  }

  const std::type_info& type = object.type();

  if (type == LS_TYPE_BOOLEAN) {
    return AnyAs<bool&>(object) ? "true" : "false";
  }

  if (type == LS_TYPE_NUMBER) {
    return std::to_string(AnyAs<double&>(object));
  }

  if (type == LS_TYPE_CALLABLE) {
    return AnyAs<SharedLamscriptCallable>(object)->ToString();
  }

  return AnyAs<std::string>(object);
}

std::any Interpreter::LookupVariable(
    const parsing::Token& name, parsed::Expression* expression) {
  try {
    size_t& distance = locals_.at(expression);
    return environment_->GetVariableAtScope(distance, name);
  } catch (const std::out_of_range& error) {
    return globals_->GetVariable(name);
  }
}

}  // namespace runtime
}  // namespace lamscript
