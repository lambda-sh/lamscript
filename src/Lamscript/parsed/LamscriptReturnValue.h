#ifndef SRC_LAMSCRIPT_PARSED_LAMSCRIPTRETURNVALUE_H_
#define SRC_LAMSCRIPT_PARSED_LAMSCRIPTRETURNVALUE_H_

#include <any>
#include <stdexcept>

namespace lamscript {
namespace parsed {

/// @brief Used for obtaining the return value from the function.
///
/// In order to bubble values back up the stack, we use std::runtime_error as
/// the base class for the return value container and then throw the
/// LamscriptReturnValue back up to where the function was initially called
/// from, allowing the value to be used from outside of the function without
/// having to explicitly return it.
class LamscriptReturnValue : std::runtime_error {
 public:
  explicit LamscriptReturnValue(std::any value)
      : std::runtime_error(""), value_(value) {}

  std::any GetReturnedValue() const { return value_; }
 private:
  std::any value_;
};

}  // namespace parsed
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSED_LAMSCRIPTRETURNVALUE_H_
