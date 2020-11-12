#ifndef SRC_LAMSCRIPT_CALLABLE_H_
#define SRC_LAMSCRIPT_CALLABLE_H_

#include <any>
#include <vector>
#include <string>

#include <Lamscript/Interpreter.h>

namespace lamscript {

class LamscriptCallable {
 public:
  virtual int Arity() const = 0;
  virtual std::any Call(
      Interpreter* interpreter, std::vector<std::any> arguments) = 0;
  virtual std::string ToString() const = 0;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_CALLABLE_H_
