#include <Lamscript/runtime/Environment.h>

#include <Lamscript/errors/RuntimeError.h>
#include <Lamscript/parsing/Token.h>

namespace lamscript {
namespace runtime {

void Environment::SetVariable(const parsing::Token& name, std::any value) {
  values_[name.Lexeme] = value;
}

void Environment::AssignVariable(const parsing::Token& name, std::any value) {
  if (values_.contains(name.Lexeme)) {
    values_[name.Lexeme] = value;
    return;
  }

  if (parent_ != nullptr) {
    return parent_->AssignVariable(name, value);
  }

  throw RuntimeError(name, "Undefined Variable '" + name.Lexeme + "'.");
}

std::any Environment::GetVariable(const parsing::Token& name) {
  if (values_.contains(name.Lexeme)) {
    return values_[name.Lexeme];
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

Environment* Environment::ScopeAt(size_t distance) {
  Environment* current = this;
  for (size_t current_pos = 0; current_pos < distance; distance++) {
    current = current->parent_.get();
  }
  return current;
}

}  // namespace runtime
}  // namespace lamscript
