#ifndef SRC_LAMSCRIPT_PARSING_RESOLVER_H_
#define SRC_LAMSCRIPT_PARSING_RESOLVER_H_

#include <any>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include <Lamscript/Visitor.h>
#include <Lamscript/parsed/Expression.h>
#include <Lamscript/parsed/Statement.h>
#include <Lamscript/parsing/Token.h>
#include <Lamscript/runtime/Interpreter.h>

namespace lamscript {
namespace parsing {

class Resolver : public ExpressionVisitor, StatementVisitor {
 public:
  explicit Resolver(std::shared_ptr<runtime::Interpreter> interpreter)
      : interpreter_(interpreter), scopes_() {}

  std::any VisitBlockStatement(parsed::Block* block) override;
  std::any VisitVariableStatement(parsed::VariableStatement* variable)
      override;

  /// @brief Forwards references to each statement into the visitor interface to
  /// ensure that variables are being binded and resolved properly.
  void Resolve(
      const std::vector<std::unique_ptr<parsed::Statement>>& statements);

 private:
  std::shared_ptr<runtime::Interpreter> interpreter_;
  std::stack<std::unordered_map<std::string, bool>> scopes_;

  /// @brief Creates a new scope to store variables and their usage in.
  void BeginScope();

  /// @brief Ends the current scope.
  void EndScope();

  /// @brief Resolves the current statement utilizing the visitor interface.
  void Resolve(parsed::Statement* statement);

  /// @brief Resolves the current expression utilizing the visitor interface.
  void Resolve(parsed::Expression* expression);


  /// Declare a variable.
  void Declare(Token name);

  /// Define a variable.
  void Define(Token name);
};

}  // namespace parsing
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSING_RESOLVER_H_
