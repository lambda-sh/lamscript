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
      std::unordered_map<std::string, LamscriptFunction>&& methods)
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
  LamscriptFunction& LookupMethod(const std::string& method_name) {
    std::cout << "looking up method by name: " << method_name << std::endl;
    for (auto& method : methods_) {
      std::cout << method.first << std::endl;
    }
    return methods_.at(method_name);
  }

  std::string ToString() const override { return name_; }

 private:
  std::string name_;
  std::unordered_map<std::string, parsed::LamscriptFunction> methods_;
};


/// @brief Instance of a lamscript class.
class LamscriptInstance :
    public std::enable_shared_from_this<LamscriptInstance> {
 public:
  explicit LamscriptInstance(
      LamscriptClass* class_def) : class_def_(class_def) {}

  std::any GetField(const parsing::Token& name) {
    if (fields_.contains(name.Lexeme)) {
      return fields_.at(name.Lexeme);
    }

    // Binds the function to the current instance, allowing the use of `this`
    // to correctly be resolved.
    try {
      LamscriptFunction& method = class_def_->LookupMethod(name.Lexeme);
      return method.Bind(shared_from_this());
    } catch (const std::out_of_range& err) {
      throw RuntimeError(name, "Undefined property '" + name.Lexeme + "'.");
    }
  }

  void SetField(const parsing::Token& name, std::any value) {
    fields_[name.Lexeme] = value;
  }

  std::string ToString() const { return class_def_->ToString() + " Instance"; }

 private:
  LamscriptClass* class_def_;
  std::unordered_map<std::string, std::any> fields_;
};

}  // namespace parsed
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_
