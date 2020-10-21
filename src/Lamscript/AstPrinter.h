#ifndef SRC_LAMSCRIPT_ASTPRINTER_H_
#define SRC_LAMSCRIPT_ASTPRINTER_H_

#include <any>
#include <initializer_list>
#include <string>
#include <iostream>

#include <Lamscript/Expression.h>
#include <Lamscript/Visitor.h>

namespace lamscript {

class AstPrinter : Visitor {
 public:
  AstPrinter() {}
  std::any Print(Expression* expression) {
    return expression->Accept(this);
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
    std::any literal = expression->GetValue();
    if (!literal.has_value()) {
      return std::string("nil");
    }

    std::string type(literal.type().name());
    if (type.compare("d") == 0) {
      return std::to_string(std::any_cast<double>(literal));
    }

    return literal;
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
  std::any Parenthesize(
      std::string name, std::initializer_list<Expression*> expressions) {
    std::string builder;

    builder.append("(").append(name);
    try {
      for (Expression* expr : expressions) {
        builder.append(" ");
        builder.append(std::any_cast<std::string>(expr->Accept(this)));
      }
    } catch (std::bad_any_cast e) {
      std::cout << e.what();
    }
    builder.append(")");

    return builder;
  }
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_ASTPRINTER_H_
