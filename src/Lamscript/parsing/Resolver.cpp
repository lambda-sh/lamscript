#include <Lamscript/parsing/Resolver.h>

#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Lamscript/parsed/Expression.h>
#include <Lamscript/parsed/Statement.h>
#include <Lamscript/runtime/Lamscript.h>

namespace lamscript {
namespace parsing {

namespace {

typedef std::unordered_map<std::string, VariableMetadata> Scope;

}  // namespace

// ---------------------------------- PUBLIC -----------------------------------

void Resolver::Resolve(
    const std::vector<std::unique_ptr<parsed::Statement>>& statements) {
  for (auto&& statement : statements) {
    Resolve(statement.get());
  }
}

// ------------------------------- EXPRESSIONS ---------------------------------

std::any Resolver::VisitVariableExpression(parsed::Variable* variable) {
  if (!scope_stack_.empty()) {
    Scope& scope = scope_stack_.back();
    const std::string& variable_name = variable->GetName().Lexeme;
    auto lookup = scope.find(variable_name);

    if (lookup != scope.end()) {
      if (lookup->second.Defined == false) {
          runtime::Lamscript::Error(
              variable->GetName(),
              "Can't read local variable in it's own initializer.");
      }
      scope[variable_name].Used = true;
    }
  }

  ResolveLocalVariable(variable, variable->GetName());
  return nullptr;
}

std::any Resolver::VisitAssignExpression(parsed::Assign* assignment) {
  Resolve(assignment->GetValue());
  ResolveLocalVariable(assignment, assignment->GetName());
  return nullptr;
}

std::any Resolver::VisitBinaryExpression(parsed::Binary* binary) {
  Resolve(binary->GetLeftSide());
  Resolve(binary->GetRightSide());
  return nullptr;
}


std::any Resolver::VisitCallExpression(parsed::Call* call) {
  Resolve(call->GetCallee());

  for (auto&& argument : call->GetArguments()) {
    Resolve(argument.get());
  }

  return nullptr;
}

std::any Resolver::VisitGroupingExpression(parsed::Grouping* grouping) {
  Resolve(grouping->GetExpression());
  return nullptr;
}

std::any Resolver::VisitLiteralExpression(parsed::Literal* literal) {
  return nullptr;
}

std::any Resolver::VisitLogicalExpression(parsed::Logical* logical) {
  Resolve(logical->GetLeftOperand());
  Resolve(logical->GetRightOperand());
  return nullptr;
}

std::any Resolver::VisitUnaryExpression(parsed::Unary* unary) {
  Resolve(unary->GetRightExpression());
  return nullptr;
}


std::any Resolver::VisitGetExpression(parsed::Get* getter) {
  Resolve(getter->GetObject().get());
  return nullptr;
}

std::any Resolver::VisitSetExpression(parsed::Set* setter) {
  Resolve(setter->GetValue());
  Resolve(setter->GetObject().get());
  return nullptr;
}

std::any Resolver::VisitSuperExpression(parsed::Super* super) {
  if (current_class_ == ClassType::None) {
    runtime::Lamscript::Error(
        super->GetKeyword(), "Can't use 'super' outside of a class.");
  }

  if (current_class_ != ClassType::SuperClass) {
    runtime::Lamscript::Error(
        super->GetKeyword(),
        "Can't use 'super' in a class with no super class.");
  }

  ResolveLocalVariable(super, super->GetKeyword());
  return nullptr;
}

std::any Resolver::VisitLambdaExpression(parsed::LambdaExpression* lambda) {
  Resolve(lambda->GetFunctionStatement());
  return nullptr;
}

std::any Resolver::VisitThisExpression(parsed::This* this_expr) {
  if (current_class_ == ClassType::None) {
    runtime::Lamscript::Error(
        this_expr->GetKeyword(), "Cannot use this outside of a class.");
  }

  if (current_function_ == FunctionType::Static) {
    runtime::Lamscript::Error(
        this_expr->GetKeyword(),
        "Cannot use this inside of a static function ");
  }

  ResolveLocalVariable(this_expr, this_expr->GetKeyword());
  return nullptr;
}

// -------------------------------- STATEMENTS ---------------------------------

std::any Resolver::VisitBlockStatement(parsed::Block* block) {
  BeginScope();
  Resolve(block->GetStatements());
  EndScope();

  return nullptr;
}

std::any Resolver::VisitVariableStatement(parsed::VariableStatement* variable) {
  Declare(variable->GetName());

  if (variable->GetInitializer() != nullptr) {
    Resolve(variable->GetInitializer());
  }

  Define(variable->GetName());
  return nullptr;
}

std::any Resolver::VisitFunctionStatement(parsed::Function* func) {
  Declare(func->GetName());
  Define(func->GetName());

  ResolveFunction(func, FunctionType::Function);
  return nullptr;
}

std::any Resolver::VisitExpressionStatement(
    parsed::ExpressionStatement* expression) {
  Resolve(expression->GetExpression());
  return nullptr;
}

/// This will resolve all parts of the if statement, regardless of what gets
/// executed or not
std::any Resolver::VisitIfStatement(parsed::If* if_statement) {
  Resolve(if_statement->GetCondition());
  Resolve(if_statement->GetThenBranch());

  if (if_statement->GetElseBranch() != nullptr) {
    Resolve(if_statement->GetElseBranch());
  }

  return nullptr;
}

std::any Resolver::VisitPrintStatement(parsed::Print* print) {
  Resolve(print->GetExpression());
  return nullptr;
}

std::any Resolver::VisitReturnStatement(parsed::Return* return_statement) {
  if (current_function_ == FunctionType::None) {
    runtime::Lamscript::Error(
        return_statement->GetKeyword(), "Can't return from top-level code.");
  }

  if (return_statement->GetValue() != nullptr) {
    if (current_function_ == FunctionType::Initializer) {
      runtime::Lamscript::Error(
          return_statement->GetKeyword(),
          "Cannot return a value form an initializer.");
    }
    Resolve(return_statement->GetValue());
  }

  return nullptr;
}

std::any Resolver::VisitWhileStatement(parsed::While* while_statement) {
  Resolve(while_statement->GetCondition());
  Resolve(while_statement->GetBody());
  return nullptr;
}


std::any Resolver::VisitClassStatement(parsed::Class* class_def) {
  ClassType enclosing_class = current_class_;
  current_class_ = ClassType::Class;

  Declare(class_def->GetName());
  Define(class_def->GetName());

  // Validate super class first.
  if (class_def->GetSuperClass() != nullptr) {
    std::string super_class_name  = static_cast<parsed::Variable*>(
        class_def->GetSuperClass())->GetName().Lexeme;

    if (super_class_name.compare(class_def->GetName().Lexeme) == 0) {
      runtime::Lamscript::Error(
          class_def->GetName(), "A class can't inherit from itself.");
    }

    current_class_ = ClassType::SuperClass;
    Resolve(class_def->GetSuperClass());

    BeginScope();
    Scope& scope = scope_stack_.back();
    scope["super"] = VariableMetadata{true, true, class_def->GetName().Line};
  }

  BeginScope();
  Scope& scope = scope_stack_.back();
  scope["this"] = VariableMetadata{true, true, class_def->GetName().Line};

  for (auto& method : class_def->GetMethods()) {
    FunctionType method_type = FunctionType::Method;

    if (method->IsStatic()) {
      method_type = FunctionType::Static;
    }

    if (method->GetName().Lexeme.compare("constructor") == 0) {
      if (method->IsStatic()) {
        runtime::Lamscript::Error(
            parsing::Token{STATIC, "static", nullptr, method->GetName().Line},
            "static cannot come before the initializer.");
      } else {
        method_type = FunctionType::Initializer;
      }
    }

    ResolveFunction(method.get(), method_type);
  }

  EndScope();

  if (class_def->GetSuperClass() != nullptr) {
    EndScope();
  }

  current_class_ = enclosing_class;
  return nullptr;
}

// --------------------------------- PRIVATE -----------------------------------

void Resolver::BeginScope() {
  scope_stack_.push_back(Scope());
}

/// Ensures that variables have to be used inside of their local scopes.
void Resolver::EndScope() {
  for (auto it : scope_stack_.back()) {
    if (it.second.Used == false) {
      runtime::Lamscript::Error(
          parsing::Token{IDENTIFIER, it.first, nullptr, it.second.Line},
          "Defined a local variable but that isn't used.");
    }
  }

  scope_stack_.pop_back();
}

void Resolver::Resolve(parsed::Statement* statement) {
  statement->Accept(this);
}

void Resolver::Resolve(parsed::Expression* expression) {
  expression->Accept(this);
}

void Resolver::Declare(Token name) {
  if (scope_stack_.empty()) {
    return;
  }

  Scope& scope = scope_stack_.back();

  if (scope.contains(name.Lexeme)) {
    runtime::Lamscript::Error(
        name, "There is already a variable that exists within this scope.");
  }

  scope[name.Lexeme] = VariableMetadata{false, false, name.Line};
}

void Resolver::Define(Token name) {
  if (scope_stack_.empty()) {
    return;
  }

  Scope& scope = scope_stack_.back();
  scope[name.Lexeme].Defined = true;
}

void Resolver::ResolveLocalVariable(
    parsed::Expression* expression, const Token& variable_name) {
  for (int pos = scope_stack_.size() - 1; pos >= 0; pos--) {
    if (scope_stack_[pos].contains(variable_name.Lexeme)) {
      interpreter_->Resolve(expression, scope_stack_.size() - 1 - pos);
      scope_stack_[pos][variable_name.Lexeme].Used = true;
      return;
    }
  }
}

void Resolver::ResolveFunction(parsed::Function* func, FunctionType type) {
  FunctionType enclosing_function = current_function_;
  current_function_ = type;

  BeginScope();

  for (const Token& param : func->GetParams()) {
    Declare(param);
    Define(param);
  }

  Resolve(func->GetBody());
  EndScope();

  current_function_ = enclosing_function;
}

}  // namespace parsing
}  // namespace lamscript
