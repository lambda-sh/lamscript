#ifndef SRC_LAMSCRIPT_ERRORS_PARSEERROR_H_
#define SRC_LAMSCRIPT_ERRORS_PARSEERROR_H_

#include <stdexcept>

namespace lamscript {

/// @brief Custom Parse error for when the parser encounters an invalid
/// expression/statement.
class ParseError : std::runtime_error {
 public:
  explicit ParseError(const char* what_arg) : std::runtime_error(what_arg){}
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_ERRORS_PARSEERROR_H_
