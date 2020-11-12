#ifndef SRC_LAMSCRIPT_GLOBALS_H_
#define SRC_LAMSCRIPT_GLOBALS_H_

#include <any>
#include <chrono>
#include <vector>

#include <Lamscript/Callable.h>

namespace lamscript {

class Clock : public LamscriptCallable {
 public:
  int Arity() const override { return 0; }
  std::any Call(
      Interpreter* interpreter, std::vector<std::any> arguments) override {
    return static_cast<double>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
  }
  std::string ToString() const override { return "<native fn>"; }
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_GLOBALS_H_
