#include <Lamscript/Environment.h>

#include <Lamscript/RuntimeError.h>
#include <Lamscript/Token.h>

namespace lamscript {

void Environment::SetVariable(std::string name, std::any value) {
  values_[name] = value;
}

std::any Environment::GetVariable(Token name) {
  if (values_.contains(name.Lexeme)) {
    return values_[name.Lexeme];
  }

  throw new RuntimeError(name, "Undefined variable: '" + name.Lexeme + "'." );
}

}  // namespace lamscript
