#ifndef SRC_LAMSCRIPT_VISITOR_H_
#define SRC_LAMSCRIPT_VISITOR_H_

#include <Lamscript/Expression.h>

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
  virtual std::any VisitBlockStatement() = 0;
  virtual std::any VisitClassStatement() = 0;
  virtual std::any VisitExpressionStatement() = 0;
  virtual std::any VisitFunctionStatement() = 0;
  virtual std::any VisitIfStatement() = 0;
  virtual std::any VisitPrintStatement() = 0;
  virtual std::any VisitReturnStatement() = 0;
  virtual std::any VisitVarStatement() = 0;
  virtual std::any VisitWhileStatement() = 0;
};


}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_VISITOR_H_
