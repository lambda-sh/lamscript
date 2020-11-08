#ifndef SRC_LAMSCRIPT_ENVIRONMENT_H_
#define SRC_LAMSCRIPT_ENVIRONMENT_H_

#include <any>
#include <string>
#include <unordered_map>

#include <Lamscript/Token.h>

namespace lamscript {

class Environment {
 public:
  /// @brief Defines a variable within the current environment.
  void SetVariable(const std::string& name, std::any value);
  std::any GetVariable(Token token);
 private:
  std::unordered_map<std::string, std::any> values_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_ENVIRONMENT_H_
