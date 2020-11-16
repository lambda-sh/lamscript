#ifndef SRC_LAMSCRIPT_PARSED_LAMSCRIPTFUNCTION_H_
#define SRC_LAMSCRIPT_PARSED_LAMSCRIPTFUNCTION_H_

#include <any>

#include <Lamscript/Environment.h>
#include <Lamscript/Interpreter.h>
#include <Lamscript/parsed/LamscriptCallable.h>
#include <Lamscript/parsed/LamscriptReturnValue.h>
#include <Lamscript/parsed/Statement.h>
#include <iostream>

namespace lamscript {
namespace parsed {

class LamscriptFunction : public LamscriptCallable {
 public:
  explicit LamscriptFunction(Function* declaration)
      : declaration_(declaration) {}

  int Arity() const override { return declaration_->GetParams().size(); }

  std::any Call(
      Interpreter* interpreter, std::vector<std::any> arguments) override {
    Environment* function_env = new Environment(
        interpreter->GetCurrentEnvironment());

    const std::vector<parsing::Token>& params = declaration_->GetParams();
    for (size_t i = 0; i < params.size(); i++) {
      function_env->SetVariable(params[i], arguments[i]);
    }

    try {
      interpreter->ExecuteBlock(declaration_->GetBody(), function_env);
    } catch (const LamscriptReturnValue& value_container) {
      return value_container.GetReturnedValue();
    }

    return nullptr;
  }

  std::string ToString() const override {
    return "<fn " + declaration_->GetName().Lexeme + ">";
  }

 private:
  Function* declaration_;
};

}  // namespace parsed
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSED_LAMSCRIPTFUNCTION_H_
