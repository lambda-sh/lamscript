#ifndef SRC_LAMSCRIPT_ENVIRONMENT_H_
#define SRC_LAMSCRIPT_ENVIRONMENT_H_

#include <any>
#include <string>
#include <unordered_map>

#include <Lamscript/parsing/Token.h>

namespace lamscript {

class Environment {
 public:
  /// @brief Create a new environment with no parent (Usually the global
  /// environment).
  Environment() : parent_(nullptr) {}

  /// @brief Create an environment within a parent environment.
  explicit Environment(Environment* parent) : parent_(parent) {}

  /// @brief Defines a variable within the current environment.
  void SetVariable(const parsing::Token& name, std::any value);
  void AssignVariable(const parsing::Token& name, std::any value);
  std::any GetVariable(const parsing::Token& token);
 private:
  Environment* parent_;
  std::unordered_map<std::string, std::any> values_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_ENVIRONMENT_H_
