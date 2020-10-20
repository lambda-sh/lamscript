#ifndef SRC_LAMSCRIPT_RUNTIMEERROR_H_
#define SRC_LAMSCRIPT_RUNTIMEERROR_H_

#include <stdexcept>
#include <string>

#include <Lamscript/Token.h>

namespace lamscript {

class RuntimeError : std::runtime_error {
 public:
  RuntimeError(Token token, const std::string& message)
    : std::runtime_error(message), token_(token) {}
 private:
  Token token_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_RUNTIMEERROR_H_
