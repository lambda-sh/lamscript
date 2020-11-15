#include <Lamscript/parsed/Expression.h>

#include <any>

#include <Lamscript/Visitor.h>

namespace lamscript {
namespace parsed {

std::any Binary::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitBinaryExpression(this);
}

std::any Assign::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitAssignExpression(this);
}

std::any Call::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitCallExpression(this);
}

std::any Get::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitGetExpression(this);
}

std::any Grouping::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitGroupingExpression(this);
}

std::any Literal::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitLiteralExpression(this);
}

std::any Logical::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitLogicalExpression(this);
}

std::any Set::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitSetExpression(this);
}

std::any Super::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitSuperExpression(this);
}

std::any This::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitThisExpression(this);
}

std::any Unary::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitUnaryExpression(this);
}

std::any Variable::Accept(ExpressionVisitor* visitor) {
  return visitor->VisitVariableExpression(this);
}

}  // namespace parsed
}  // namespace lamscript
