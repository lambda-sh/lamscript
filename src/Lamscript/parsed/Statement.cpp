#include <Lamscript/parsed/Statement.h>

#include <Lamscript/Visitor.h>

namespace lamscript {
namespace parsed {

std::any Block::Accept(StatementVisitor* visitor) {
  return visitor->VisitBlockStatement(this);
}

std::any ExpressionStatement::Accept(StatementVisitor* visitor) {
  return visitor->VisitExpressionStatement(this);
}

std::any Function::Accept(StatementVisitor* visitor) {
  return visitor->VisitFunctionStatement(this);
}

std::any Class::Accept(StatementVisitor* visitor) {
  return visitor->VisitClassStatement(this);
}

std::any If::Accept(StatementVisitor* visitor) {
  return visitor->VisitIfStatement(this);
}

std::any Print::Accept(StatementVisitor* visitor) {
  return visitor->VisitPrintStatement(this);
}

std::any Return::Accept(StatementVisitor* visitor) {
  return visitor->VisitReturnStatement(this);
}

std::any VariableStatement::Accept(StatementVisitor* visitor) {
  return visitor->VisitVariableStatement(this);
}

std::any While::Accept(StatementVisitor* visitor) {
  return visitor->VisitWhileStatement(this);
}

}  // namespace parsed
}  // namespace lamscript
