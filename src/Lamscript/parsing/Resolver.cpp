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
    std::unordered_map<std::string, bool>& scope = scope_stack_.back();
    const std::string& variable_name = variable->GetName().Lexeme;
    auto lookup = scope.find(variable_name);

    if (lookup != scope.end() && lookup->second == false) {
      runtime::Lamscript::Error(
          variable->GetName(),
          "Can't read local variable in it's own initializer.");
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

  ResolveFunction(func, FunctionType::kFunction);
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
  if (current_function_ == FunctionType::kNone) {
    runtime::Lamscript::Error(
        return_statement->GetKeyword(), "Can't return from top-level code.");
  }

  if (return_statement->GetValue() != nullptr) {
    Resolve(return_statement->GetValue());
  }

  return nullptr;
}

std::any Resolver::VisitWhileStatement(parsed::While* while_statement) {
  Resolve(while_statement->GetCondition());
  Resolve(while_statement->GetBody());
  return nullptr;
}

// --------------------------------- PRIVATE -----------------------------------

void Resolver::BeginScope() {
  scope_stack_.push_back(std::unordered_map<std::string, bool>());
}

void Resolver::EndScope() {
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

  std::unordered_map<std::string, bool> &scope = scope_stack_.back();

  if (scope.contains(name.Lexeme)) {
    runtime::Lamscript::Error(
        name, "There is already a variable that exists within this scope.");
  }

  scope[name.Lexeme] = false;
}

void Resolver::Define(Token name) {
  if (scope_stack_.empty()) {
    return;
  }

  std::unordered_map<std::string, bool> &scope = scope_stack_.back();
  scope[name.Lexeme] = true;
}

void Resolver::ResolveLocalVariable(
    parsed::Expression* expression, const Token& variable_name) {
  for (int pos = scope_stack_.size() - 1; pos >= 0; pos--) {
    if (scope_stack_[pos].contains(variable_name.Lexeme)) {
      interpreter_->Resolve(expression, scope_stack_.size() - 1 - pos);
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
