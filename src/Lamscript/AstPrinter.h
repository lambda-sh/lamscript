#ifndef SRC_LAMSCRIPT_ASTPRINTER_H_
#define SRC_LAMSCRIPT_ASTPRINTER_H_

#include <any>
#include <initializer_list>
#include <string>

#include <Lamscript/Expression.h>

namespace lamscript {

class AstPrinter : Visitor {
 public:
  AstPrinter() {}
  std::string Print(Expression* expression) {
    return std::any_cast<std::string>(expression->Accept(this));
  }

  std::any VisitAssignExpression(Assign* expression) override {}

  std::any VisitBinaryExpression(Binary* expression) override {
    return Parenthesize(
        expression->GetOperator().Lexeme,
        {expression->GetLeftSide(), expression->GetRightSide()});
  }

  std::any VisitCallExpression(Call* expression) override {}
  std::any VisitGetExpression(Get* expression) override {}

  std::any VisitGroupingExpression(Grouping* expression) override {
    return Parenthesize("group", {expression->GetExpression()});
  }

  std::any VisitLiteralExpression(Literal* expression) override {
    const std::any& literal = expression->GetValue();

    if (!literal.has_value()) {
      return "nil";
    }

    std::string type(literal.type().name());
    if (type.compare("double") == 0) {
      return std::to_string(std::any_cast<double>(literal));
    }

    return std::any_cast<std::string>(literal);
  }

  std::any VisitLogicalExpression(Logical* expression) override {}
  std::any VisitSetExpression(Set* expression) override {}
  std::any VisitSuperExpression(Super* expression) override {}
  std::any VisitThisExpression(This* expression) override {}

  std::any VisitUnaryExpression(Unary* expression) override {
    return Parenthesize(
        expression->GetUnaryOperator().Lexeme,
        {expression->GetRightExpression()});
  }

  std::any VisitVariableExpression(Variable* expression) override {}

 private:
  std::vector<std::string> results_;
  std::string Parenthesize(
      std::string name, std::initializer_list<Expression*> expressions) {
    std::string builder;

    builder.append("(").append(name);
    for (Expression* expr : expressions) {
      builder.append(" ");
      builder.append(std::any_cast<std::string>(expr->Accept(this)));
    }

    return builder;
  }
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_ASTPRINTER_H_
