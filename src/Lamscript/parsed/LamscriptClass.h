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
      std::shared_ptr<LamscriptClass> super_class,
      std::unordered_map<std::string, LamscriptFunction>&& methods)
              : name_(name), super_class_(super_class), methods_(methods) {}

  int Arity() const override {
    try {
      const LamscriptFunction& initializer = LookupMethod("constructor");
      return initializer.Arity();
    } catch (const std::out_of_range& err) {
      return 0;
    }
  }

  std::any Call(
      runtime::Interpreter* interpreter,
      std::vector<std::any> arguments) override {
    std::shared_ptr<LamscriptInstance> instance = std::make_shared<
        LamscriptInstance>(this);

    try {
      const LamscriptFunction& initializer = LookupMethod("constructor");
      initializer.Bind(instance)->Call(interpreter, arguments);
    } catch (const std::out_of_range& err) {}

    return instance;
  }

  /// @brief Looks up a method inside of the current class definition. If it
  /// doesn't exist, returns a nullptr.
  const LamscriptFunction& LookupMethod(const std::string& method_name) const {
    auto lookup = methods_.find(method_name);

    if (lookup != methods_.end()) {
      return lookup->second;
    }

    if (super_class_ != nullptr) {
      return super_class_->LookupMethod(method_name);
    }

    throw std::out_of_range("Lookup out of range.");
  }

  std::string ToString() const override { return name_; }

 private:
  std::string name_;
  std::shared_ptr<LamscriptClass> super_class_;
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
      const LamscriptFunction& method = class_def_->LookupMethod(name.Lexeme);
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
