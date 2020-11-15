#ifndef SRC_LAMSCRIPT_VISITOR_H_
#define SRC_LAMSCRIPT_VISITOR_H_

#include <Lamscript/parsable/Expression.h>
#include <Lamscript/parsable/Statement.h>

namespace lamscript {

/// @brief The visitor interface that allows for expressions
class ExpressionVisitor {
 public:
  virtual std::any VisitAssignExpression(parsable::Assign* expression) = 0;
  virtual std::any VisitBinaryExpression(parsable::Binary* expression) = 0;
  virtual std::any VisitCallExpression(parsable::Call* expression) = 0;
  virtual std::any VisitGetExpression(parsable::Get* expression) = 0;
  virtual std::any VisitGroupingExpression(parsable::Grouping* expression) = 0;
  virtual std::any VisitLiteralExpression(parsable::Literal* expression) = 0;
  virtual std::any VisitLogicalExpression(parsable::Logical* expression) = 0;
  virtual std::any VisitSetExpression(parsable::Set* expression) = 0;
  virtual std::any VisitSuperExpression(parsable::Super* expression) = 0;
  virtual std::any VisitThisExpression(parsable::This* expression) = 0;
  virtual std::any VisitUnaryExpression(parsable::Unary* expression) = 0;
  virtual std::any VisitVariableExpression(parsable::Variable* expression) = 0;
};

/// @brief The visitor interface for evaluating statements.
class StatementVisitor {
 public:
  virtual std::any VisitBlockStatement(parsable::Block* statement) = 0;
  virtual std::any VisitClassStatement(parsable::Class* statement) = 0;
  virtual std::any VisitExpressionStatement(
      parsable::ExpressionStatement* statement) = 0;
  virtual std::any VisitFunctionStatement(parsable::Function* statement) = 0;
  virtual std::any VisitIfStatement(parsable::If* statement) = 0;
  virtual std::any VisitPrintStatement(parsable::Print* statement) = 0;
  virtual std::any VisitReturnStatement(parsable::Return* statement) = 0;
  virtual std::any VisitVariableStatement(
      parsable::VariableStatement* statement) = 0;
  virtual std::any VisitWhileStatement(parsable::While* statement) = 0;
};


}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_VISITOR_H_
