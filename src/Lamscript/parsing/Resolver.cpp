#include <Lamscript/parsing/Resolver.h>

#include <any>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include <Lamscript/parsed/Statement.h>

namespace lamscript {
namespace parsing {

// ---------------------------------- PUBLIC -----------------------------------

void Resolver::Resolve(
    const std::vector<std::unique_ptr<parsed::Statement>>& statements) {
  for (auto&& statement : statements) {
    Resolve(statement.get());
  }
}

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

// --------------------------------- PRIVATE -----------------------------------

void Resolver::BeginScope() {
  scopes_.push(std::unordered_map<std::string, bool>());
}

void Resolver::EndScope() {
  scopes_.pop();
}

void Resolver::Resolve(parsed::Statement* statement) {
  statement->Accept(this);
}

void Resolver::Resolve(parsed::Expression* expression) {
  expression->Accept(this);
}

void Resolver::Declare(Token name) {
  if (scopes_.empty()) {
    return;
  }

  std::unordered_map<std::string, bool>& scope = scopes_.top();
  scope[name.Lexeme] = false;
}

void Resolver::Define(Token name) {
  if (scopes_.empty()) {
    return;
  }

  std::unordered_map<std::string, bool>& scope = scopes_.top();
  scope[name.Lexeme] = true;
}

}  // namespace parsing
}  // namespace lamscript
