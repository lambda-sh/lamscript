#ifndef SRC_LAMSCRIPT_PARSABLE_LAMSCRIPTCALLABLE_H_
#define SRC_LAMSCRIPT_PARSABLE_LAMSCRIPTCALLABLE_H_

#include <any>
#include <vector>
#include <string>

#include <Lamscript/Interpreter.h>

namespace lamscript {
namespace parsable {

class LamscriptCallable {
 public:
  virtual int Arity() const = 0;
  virtual std::any Call(
      Interpreter* interpreter, std::vector<std::any> arguments) = 0;
  virtual std::string ToString() const = 0;
};

}  // namespace parsable
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSABLE_LAMSCRIPTCALLABLE_H_