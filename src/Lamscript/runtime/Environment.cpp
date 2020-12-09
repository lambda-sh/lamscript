#include <Lamscript/runtime/Environment.h>

#include <iostream>

#include <Lamscript/errors/RuntimeError.h>
#include <Lamscript/parsing/Token.h>

namespace lamscript {
namespace runtime {

namespace {

typedef std::unordered_map<std::string, std::any>::iterator EnvSearchResult;

}  // namespace

void Environment::SetVariable(const parsing::Token& name, std::any value) {
  values_[name.Lexeme] = value;
}

void Environment::AssignVariable(const parsing::Token& name, std::any value) {
  EnvSearchResult lookup = values_.find(name.Lexeme);

  if (lookup != values_.end()) {
    lookup->second = value;
    return;
  }

  if (parent_ != nullptr) {
    parent_->AssignVariable(name, value);
    return;
  }

  throw RuntimeError(name, "Undefined Variable '" + name.Lexeme + "'.");
}

void Environment::AssignVariableAtScope(
    size_t distance, const parsing::Token &name, std::any value) {
  ScopeAt(distance)->AssignVariable(name, value);
}

std::any Environment::GetVariable(const parsing::Token& name) {
  EnvSearchResult lookup = values_.find(name.Lexeme);

  if (lookup != values_.end()) {
    return lookup->second;
  }

  if (parent_ != nullptr) {
    return parent_->GetVariable(name);
  }

  throw RuntimeError(name, "Undefined variable: '" + name.Lexeme + "'.");
}

std::any Environment::GetVariableAtScope(
    size_t distance, const parsing::Token& name) {
  return ScopeAt(distance)->GetVariable(name);
}

// ---------------------------------- PRIVATE ----------------------------------

Environment* Environment::ScopeAt(const size_t& distance) {
  Environment* current = this;
  for (size_t current_pos = 0; current_pos < distance; current_pos++) {
    current = current->parent_.get();
  }
  return current;
}

}  // namespace runtime
}  // namespace lamscript
