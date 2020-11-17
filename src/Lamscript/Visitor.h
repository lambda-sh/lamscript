#ifndef SRC_LAMSCRIPT_VISITOR_H_
#define SRC_LAMSCRIPT_VISITOR_H_

#include <Lamscript/parsed/Expression.h>
#include <Lamscript/parsed/Statement.h>

namespace lamscript {

/// @brief The visitor interface that allows for expressions
class ExpressionVisitor {
 public:
  virtual std::any VisitAssignExpression(parsed::Assign* expression) = 0;
  virtual std::any VisitBinaryExpression(parsed::Binary* expression) = 0;
  virtual std::any VisitCallExpression(parsed::Call* expression) = 0;
  virtual std::any VisitGetExpression(parsed::Get* expression) = 0;
  virtual std::any VisitGroupingExpression(parsed::Grouping* expression) = 0;
  virtual std::any VisitLiteralExpression(parsed::Literal* expression) = 0;
  virtual std::any VisitLogicalExpression(parsed::Logical* expression) = 0;
  virtual std::any VisitSetExpression(parsed::Set* expression) = 0;
  virtual std::any VisitSuperExpression(parsed::Super* expression) = 0;
  virtual std::any VisitThisExpression(parsed::This* expression) = 0;
  virtual std::any VisitUnaryExpression(parsed::Unary* expression) = 0;
  virtual std::any VisitVariableExpression(parsed::Variable* expression) = 0;
  virtual std::any VisitLambdaExpression(
      parsed::LambdaExpression* expression) = 0;
};

/// @brief The visitor interface for evaluating statements.
class StatementVisitor {
 public:
  virtual std::any VisitBlockStatement(parsed::Block* statement) = 0;
  virtual std::any VisitClassStatement(parsed::Class* statement) = 0;
  virtual std::any VisitExpressionStatement(
      parsed::ExpressionStatement* statement) = 0;
  virtual std::any VisitFunctionStatement(parsed::Function* statement) = 0;
  virtual std::any VisitIfStatement(parsed::If* statement) = 0;
  virtual std::any VisitPrintStatement(parsed::Print* statement) = 0;
  virtual std::any VisitReturnStatement(parsed::Return* statement) = 0;
  virtual std::any VisitVariableStatement(
      parsed::VariableStatement* statement) = 0;
  virtual std::any VisitWhileStatement(parsed::While* statement) = 0;
};


}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_VISITOR_H_
