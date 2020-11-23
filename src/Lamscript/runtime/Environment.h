#ifndef SRC_LAMSCRIPT_RUNTIME_ENVIRONMENT_H_
#define SRC_LAMSCRIPT_RUNTIME_ENVIRONMENT_H_

#include <any>
#include <memory>
#include <string>
#include <unordered_map>

#include <Lamscript/parsing/Token.h>

namespace lamscript {
namespace runtime {

/// @brief Allows for the storage of variables in memory.
class Environment {
 public:
  /// @brief Create a new environment with no parent (Usually the global
  /// environment).
  Environment() : parent_(nullptr) {}

  /// @brief Create an environment within a parent environment.
  explicit Environment(std::shared_ptr<Environment> parent) : parent_(parent) {}

  /// @brief Defines a variable within the current environment.
  void SetVariable(const parsing::Token& name, std::any value);

  /// @brief Assigns a variable within the current environment.
  void AssignVariable(const parsing::Token& name, std::any value);

  /// @brief Assigns a variable at a further up scope.
  void AssignVariableAtScope(
      size_t distance, const parsing::Token& name, std::any value);

  /// @brief Gets a variable within the current environment.
  std::any GetVariable(const parsing::Token& token);

  /// brief Gets a variable at scope.
  std::any GetVariableAtScope(size_t distance, const parsing::Token& name);

 private:
  std::shared_ptr<Environment> parent_;
  std::unordered_map<std::string, std::any> values_;

  Environment* ScopeAt(size_t distance);
};

}  // namespace runtime
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_RUNTIME_ENVIRONMENT_H_
