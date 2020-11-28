#ifndef SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_
#define SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_

#include <string>

#include <Lamscript/parsed/LamscriptCallable.h>

namespace lamscript {
namespace parsed {

class LamscriptInstance;

class LamscriptClass : LamscriptCallable {
 public:
  explicit LamscriptClass(const std::string& name) : name_(name) {}

  int Arity() const override { return 0; }

  std::any Call(
      runtime::Interpreter* interpreter,
      std::vector<std::any> arguments) override {
    std::shared_ptr<LamscriptInstance> instance = std::make_shared<
        LamscriptInstance>(this);
    return instance;
  }

  std::string ToString() const override { return name_; }
 private:
  std::string name_;
};


/// @brief Instance of a lamscript class.
class LamscriptInstance {
 public:
  explicit LamscriptInstance(
      LamscriptClass* class_def) : class_def_(class_def) {}

  std::any& GetField(const parsing::Token& name) {
    if (fields.contains(name.Lexeme)) {
      return fields.at(name.Lexeme);
    }

    throw new RuntimeError(name, "Undefined property '" + name.Lexeme + "'.");
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
