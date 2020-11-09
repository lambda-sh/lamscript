#include <Lamscript/Environment.h>

#include <Lamscript/RuntimeError.h>
#include <Lamscript/Token.h>

namespace lamscript {

void Environment::SetVariable(const Token& name, std::any value) {
  values_[name.Lexeme] = value;
}

void Environment::AssignVariable(const Token& name, std::any value) {
  if (values_.contains(name.Lexeme)) {
    values_[name.Lexeme] = value;
    return;
  }

  if (parent_) {
    parent_->AssignVariable(name, value);
  }

  throw RuntimeError(name, "Undefined Variable '" + name.Lexeme + "'.");
}

std::any Environment::GetVariable(const Token& name) {
  if (values_.contains(name.Lexeme)) {
    return values_[name.Lexeme];
  }

  if (parent_) {
    return parent_->GetVariable(name);
  }

  throw RuntimeError(name, "Undefined variable: '" + name.Lexeme + "'.");
}

}  // namespace lamscript
