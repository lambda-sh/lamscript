#include <Lamscript/Statement.h>

#include <Lamscript/Visitor.h>

namespace lamscript {

std::any Block::Accept(StatementVisitor* visitor) {
  return visitor->VisitBlockStatement();
}

std::any ExpressionStatement::Accept(StatementVisitor* visitor) {
  return visitor->VisitExpressionStatement();
}

std::any Function::Accept(StatementVisitor* visitor) {
  return visitor->VisitFunctionStatement();
}

std::any Class::Accept(StatementVisitor* visitor) {
  return visitor->VisitClassStatement();
}

std::any If::Accept(StatementVisitor* visitor) {
  return visitor->VisitIfStatement();
}

std::any Print::Accept(StatementVisitor* visitor) {
  return visitor->VisitPrintStatement();
}

std::any Return::Accept(StatementVisitor* visitor) {
  return visitor->VisitReturnStatement();
}

std::any VariableStatement::Accept(StatementVisitor* visitor) {
  return visitor->VisitVarStatement();
}

std::any While::Accept(StatementVisitor* visitor) {
  return visitor->VisitWhileStatement();
}

}  // namespace lamscript
