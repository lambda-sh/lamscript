#include <Lamscript/Expression.h>

#include <any>

#include <Lamscript/Visitor.h>

namespace lamscript {

std::any Binary::Accept(Visitor* visitor) {
  return visitor->VisitBinaryExpression(this);
}

std::any Assign::Accept(Visitor* visitor) {
  return visitor->VisitAssignExpression(this);
}

std::any Call::Accept(Visitor* visitor) {
  return visitor->VisitCallExpression(this);
}

std::any Get::Accept(Visitor* visitor) {
  return visitor->VisitGetExpression(this);
}

std::any Grouping::Accept(Visitor* visitor) {
  return visitor->VisitGroupingExpression(this);
}

std::any Literal::Accept(Visitor* visitor) {
  return visitor->VisitLiteralExpression(this);
}

std::any Logical::Accept(Visitor* visitor) {
  return visitor->VisitLogicalExpression(this);
}

std::any Set::Accept(Visitor* visitor) {
  return visitor->VisitSetExpression(this);
}

std::any Super::Accept(Visitor* visitor) {
  return visitor->VisitSuperExpression(this);
}

std::any This::Accept(Visitor* visitor) {
  return visitor->VisitThisExpression(this);
}

std::any Unary::Accept(Visitor* visitor) {
  return visitor->VisitUnaryExpression(this);
}

std::any Variable::Accept(Visitor* visitor) {
  return visitor->VisitVariableExpression(this);
}

}  // namespace lamscript
