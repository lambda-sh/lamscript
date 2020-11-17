#ifndef SRC_LAMSCRIPT_PARSED_LAMSCRIPTCALLABLE_H_
#define SRC_LAMSCRIPT_PARSED_LAMSCRIPTCALLABLE_H_

#include <any>
#include <vector>
#include <string>

#include <Lamscript/runtime/Interpreter.h>

namespace lamscript {
namespace parsed {

class LamscriptCallable {
 public:
  virtual int Arity() const = 0;
  virtual std::any Call(
      runtime::Interpreter* interpreter, std::vector<std::any> arguments) = 0;
  virtual std::string ToString() const = 0;
};

}  // namespace parsed
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSED_LAMSCRIPTCALLABLE_H_
