#include <Lamscript/Expression.h>

namespace lamscript {

void Binary::Accept(Visitor* visitor) {
  visitor->VisitBinaryExpression(this);
}

void Assign::Accept(Visitor* visitor) {
  visitor->VisitAssignExpression(this);
}

void Call::Accept(Visitor* visitor) {
  visitor->VisitCallExpression(this);
}

void Get::Accept(Visitor* visitor) {
  visitor->VisitGetExpression(this);
}

void Grouping::Accept(Visitor* visitor) {
  visitor->VisitGroupingExpression(this);
}

void Literal::Accept(Visitor* visitor) {
  visitor->VisitLiteralExpression(this);
}

void Logical::Accept(Visitor* visitor) {
  visitor->VisitLogicalExpression(this);
}

void Set::Accept(Visitor* visitor) {
  visitor->VisitSetExpression(this);
}

void Super::Accept(Visitor* visitor) {
  visitor->VisitSuperExpression(this);
}

void This::Accept(Visitor* visitor) {
  visitor->VisitThisExpression(this);
}

void Unary::Accept(Visitor* visitor) {
  visitor->VisitUnaryExpression(this);
}

void Variable::Accept(Visitor* visitor) {
  visitor->VisitVariableExpression(this);
}

}  // namespace lamscript
