#include <Lamscript/Environment.h>

#include <Lamscript/RuntimeError.h>
#include <Lamscript/parsing/Token.h>

namespace lamscript {

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

}  // namespace lamscript
