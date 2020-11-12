#ifndef SRC_LAMSCRIPT_FUNCTION_H_
#define SRC_LAMSCRIPT_FUNCTION_H_

#include <any>

#include <Lamscript/Callable.h>
#include <Lamscript/Environment.h>
#include <Lamscript/Statement.h>
#include <Lamscript/Interpreter.h>

namespace lamscript {

class LamscriptFunction : public LamscriptCallable {
 public:
  explicit LamscriptFunction(Function* declaration)
      : declaration_(declaration) {}

  int Arity() const override { return declaration_->GetParams().size(); }

  std::any Call(
      Interpreter* interpreter, std::vector<std::any> arguments) override {
    Environment* function_env = new Environment(
        interpreter->GetGlobalEnvironment());

    const std::vector<Token>& params = declaration_->GetParams();
    for (size_t i = 0; i < params.size(); i++) {
      function_env->SetVariable(params[i], arguments[i]);
    }

    interpreter->ExecuteBlock(declaration_->GetBody(), function_env);
    return NULL;
  }

  std::string ToString() const override {
    return "<fn " + declaration_->GetName().Lexeme + ">";
  }

 private:
  Function* declaration_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_FUNCTION_H_
