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

/// @brief Resolves variables and expressions prior to interpreting them.
class Resolver : public ExpressionVisitor, StatementVisitor {
 public:
  explicit Resolver(std::shared_ptr<runtime::Interpreter> interpreter)
      : interpreter_(interpreter), scope_stack_() {}

  std::any VisitVariableExpression(parsed::Variable* variable) override;

  /// @brief Resolves the expression for the assigned value and then resolves
  /// the variable that's being assigned to.
  std::any VisitAssignExpression(parsed::Assign* assignment) override;

  /// @brief Resolves both of the expressions.
  std::any VisitBinaryExpression(parsed::Binary* binary) override;

  /// @brief Resolves the callee and all of the arguments passed into it.
  std::any VisitCallExpression(parsed::Call* call) override;

  /// @brief Resolves the expression contained within the grouping.
  std::any VisitGroupingExpression(parsed::Grouping* grouping) override;

  /// @brief no-op considering that literals don't resolve into variables.
  std::any VisitLiteralExpression(parsed::Literal* literal) override;

  /// @brief Visits both the left and the right operands.
  std::any VisitLogicalExpression(parsed::Logical* logical) override;

  /// @brief Visit the right side of the unary expression.
  std::any VisitUnaryExpression(parsed::Unary* unary) override;

  /// @brief Resolves all variables declared within block statements.
  std::any VisitBlockStatement(parsed::Block* block) override;

  /// @brief Declares, initializes (if possible), and defines the variable in
  /// the current scope.
  std::any VisitVariableStatement(parsed::VariableStatement* variable)
      override;

  /// @brief Resolves the function eagerly, allowing it to recursively call
  /// itself.
  std::any VisitFunctionStatement(parsed::Function* func) override;

  /// @brief Resolves the expression associated with the expression statement.
  std::any VisitExpressionStatement(parsed::ExpressionStatement* expression)
      override;

  /// @brief Resolves the condition, then branch, and then else branch if
  /// applicable.
  std::any VisitIfStatement(parsed::If* if_statement) override;

  /// @brief Resolves the expression being used inside of the print statement.
  std::any VisitPrintStatement(parsed::Print* print) override;

  /// @brief Resolves the expression returned by the return statement if it
  /// isn't null (explicitly or implicitly void/nil).
  std::any VisitReturnStatement(parsed::Return* return_statement) override;

  /// @brief Resolves both the condition and the body.
  std::any VisitWhileStatement(parsed::While* while_statement) override;


  /// @brief Forwards references to each statement into the visitor interface
  /// to ensure that variables are being binded and resolved properly.
  void Resolve(
      const std::vector<std::unique_ptr<parsed::Statement>> &statements);


  /// @todo Implement the rest of these expression/statement visitation methods.
  std::any VisitGetExpression(parsed::Get* expression) override {}
  std::any VisitSetExpression(parsed::Set* expression) override {}
  std::any VisitSuperExpression(parsed::Super* expression) override {}
  std::any VisitThisExpression(parsed::This* expression) override {}
  std::any VisitLambdaExpression(
      parsed::LambdaExpression* expression) override {}

  std::any VisitClassStatement(parsed::Class* statement) override {}

 private:
  std::shared_ptr<runtime::Interpreter> interpreter_;
  std::vector<std::unordered_map<std::string, bool>> scope_stack_;

  /// @brief Creates a new scope to store variables and their usage in.
  void BeginScope();

  /// @brief Ends the current scope.
  void EndScope();

  /// @brief Resolves the current statement utilizing the visitor interface.
  void Resolve(parsed::Statement* statement);

  /// @brief Resolves the current expression utilizing the visitor interface.
  void Resolve(parsed::Expression* expression);

  /// @brief Declares a variable and marks it as unitialized.
  void Declare(Token name);

  /// @brief Define a variables a variable and marks it initialized.
  void Define(Token name);

  /// @brief Resolves local variables in the scope that they're being defined
  /// in.
  void ResolveLocalVariable(
      parsed::Expression* expression, const Token& variable_name);

  /// @brief Creates the function scope and binds the function parameters and
  /// body to the proper variables.
  void ResolveFunction(parsed::Function* func);
};

}  // namespace parsing
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSING_RESOLVER_H_
