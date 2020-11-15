#ifndef SRC_LAMSCRIPT_RUNTIMEERROR_H_
#define SRC_LAMSCRIPT_RUNTIMEERROR_H_

#include <stdexcept>
#include <string>

#include <Lamscript/parsing/Token.h>

namespace lamscript {

/// @brief Custom runtime error to handle invalid expressions.
class RuntimeError : public std::runtime_error {
 public:
  RuntimeError(Token token, const std::string& message)
    : std::runtime_error(message), token_(token) {}
  const Token& GetToken() const { return token_; }
 private:
  Token token_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_RUNTIMEERROR_H_
