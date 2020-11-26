#ifndef SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_
#define SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_

#include <string>

#include <Lamscript/parsed/LamscriptCallable.h>

namespace lamscript {
namespace parsed {

class LamscriptClass : LamscriptCallable {
 public:
  explicit LamscriptClass(const std::string& name) : name_(name) {}

  int Arity() const override { return 0; }

  std::any Call(
      runtime::Interpreter* interpreter,
      std::vector<std::any> arguments) override {}

  std::string ToString() const override { return name_; }
 private:
  std::string name_;
};

}  // namespace parsed
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSED_LAMSCRIPTCLASS_H_
