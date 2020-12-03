#ifndef SRC_LAMSCRIPT_PARSED_LAMSCRIPTFUNCTION_H_
#define SRC_LAMSCRIPT_PARSED_LAMSCRIPTFUNCTION_H_

#include <any>
#include <memory>
#include <iostream>

#include <Lamscript/parsed/LamscriptCallable.h>
#include <Lamscript/parsed/LamscriptReturnValue.h>
#include <Lamscript/parsed/Statement.h>
#include <Lamscript/runtime/Environment.h>
#include <Lamscript/runtime/Interpreter.h>

namespace lamscript {
namespace parsed {

class LamscriptInstance;

class LamscriptFunction : public LamscriptCallable {
 public:
  LamscriptFunction(
      std::shared_ptr<Function> declaration,
      std::shared_ptr<runtime::Environment> closure)
          : declaration_(declaration), closure_(closure) {}

  /// @brief Enables functions to bind to whatever instance they desire,
  /// allowing `this` expressions to be resolved to their correct scope.
  std::shared_ptr<LamscriptCallable> Bind(
      std::shared_ptr<LamscriptInstance> instance) {
    std::shared_ptr<runtime::Environment> function_env =
        std::make_shared<runtime::Environment>(closure_);

    function_env->SetVariable(
        parsing::Token{parsing::THIS, "this", nullptr, 0}, instance);

    std::shared_ptr<LamscriptCallable> callable;
    callable.reset(new LamscriptFunction(declaration_, function_env));
    return callable;
  }

  int Arity() const override { return declaration_->GetParams().size(); }

  std::any Call(
      runtime::Interpreter* interpreter,
      std::vector<std::any> arguments) override {
    std::shared_ptr<runtime::Environment> function_env =
        std::make_shared<runtime::Environment>(closure_);

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
  std::shared_ptr<Function> declaration_;
  std::shared_ptr<runtime::Environment> closure_;
};

}  // namespace parsed
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSED_LAMSCRIPTFUNCTION_H_
