#ifndef SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_
#define SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_

#include <string>

#include <Lamscript/parsed/LamscriptCallable.h>
#include <Lamscript/parsed/LamscriptFunction.h>

namespace lamscript {
namespace parsed {

class LamscriptInstance;

class LamscriptClass : public LamscriptCallable {
 public:
  LamscriptClass(
      const std::string& name,
      std::unordered_map<std::string, parsed::LamscriptFunction*> methods)
              : name_(name), methods_(methods) {}

  int Arity() const override { return 0; }

  std::any Call(
      runtime::Interpreter* interpreter,
      std::vector<std::any> arguments) override {
    std::shared_ptr<LamscriptInstance> instance = std::make_shared<
        LamscriptInstance>(this);
    return instance;
  }

  /// @brief Looks up a method inside of the current class definition. If it
  /// doesn't exist, returns a nullptr.
  parsed::LamscriptFunction* LookupMethod(std::string method_name) {
    if (methods_.contains(method_name)) {
      return methods_.at(method_name);
    }

    return nullptr;
  }

  std::string ToString() const override { return name_; }

 private:
  std::string name_;
  std::unordered_map<std::string, parsed::LamscriptFunction*> methods_;
};


/// @brief Instance of a lamscript class.
class LamscriptInstance {
 public:
  explicit LamscriptInstance(
      LamscriptClass* class_def) : class_def_(class_def) {}

  std::any GetField(const parsing::Token& name) {
    if (fields.contains(name.Lexeme)) {
      return fields.at(name.Lexeme);
    }

    LamscriptCallable* method = class_def_->LookupMethod(name.Lexeme);
    if (method != nullptr) {
      return method;
    }

    throw RuntimeError(name, "Undefined property '" + name.Lexeme + "'.");
  }

  void SetField(const parsing::Token& name, std::any value) {
    fields[name.Lexeme] = value;
  }

  std::string ToString() const { return class_def_->ToString() + " Instance"; }

 private:
  LamscriptClass* class_def_;
  std::unordered_map<std::string, std::any> fields;
};

}  // namespace parsed
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_
