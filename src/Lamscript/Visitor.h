#ifndef SRC_LAMSCRIPT_VISITOR_H_
#define SRC_LAMSCRIPT_VISITOR_H_

#include <Lamscript/Expression.h>
#include <Lamscript/Statement.h>

namespace lamscript {

/// @brief The visitor interface that allows for expressions
class ExpressionVisitor {
 public:
  virtual std::any VisitAssignExpression(Assign* expression) = 0;
  virtual std::any VisitBinaryExpression(Binary* expression) = 0;
  virtual std::any VisitCallExpression(Call* expression) = 0;
  virtual std::any VisitGetExpression(Get* expression) = 0;
  virtual std::any VisitGroupingExpression(Grouping* expression) = 0;
  virtual std::any VisitLiteralExpression(Literal* expression) = 0;
  virtual std::any VisitLogicalExpression(Logical* expression) = 0;
  virtual std::any VisitSetExpression(Set* expression) = 0;
  virtual std::any VisitSuperExpression(Super* expression) = 0;
  virtual std::any VisitThisExpression(This* expression) = 0;
  virtual std::any VisitUnaryExpression(Unary* expression) = 0;
  virtual std::any VisitVariableExpression(Variable* expression) = 0;
};

/// @brief The visitor interface for evaluating statements.
class StatementVisitor {
 public:
  virtual std::any VisitBlockStatement(Block* statement) = 0;
  virtual std::any VisitClassStatement(Class* statement) = 0;
  virtual std::any VisitExpressionStatement(ExpressionStatement* statement) = 0;
  virtual std::any VisitFunctionStatement(Function* statement) = 0;
  virtual std::any VisitIfStatement(If* statement) = 0;
  virtual std::any VisitPrintStatement(Print* statement) = 0;
  virtual std::any VisitReturnStatement(Return* statement) = 0;
  virtual std::any VisitVariableStatement(VariableStatement* statement) = 0;
  virtual std::any VisitWhileStatement(While* statement) = 0;
};


}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_VISITOR_H_
