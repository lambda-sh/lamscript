#include <gtest/gtest.h>

#include <Lamscript/runtime/Lamscript.h>

TEST(Examples, AnonymousClasses) {
  ::lamscript::runtime::Lamscript::RunFile("examples/anonymous.ls");
}

TEST(Examples, Blocks) {
  ::lamscript::runtime::Lamscript::RunFile("examples/blocks.ls");
}

TEST(Examples, Classes) {
  ::lamscript::runtime::Lamscript::RunFile("examples/class.ls");
}

TEST(Examples, Closures) {
  ::lamscript::runtime::Lamscript::RunFile("examples/closure.ls");
}

TEST(Examples, Conditionals) {
  ::lamscript::runtime::Lamscript::RunFile("examples/conditionals.ls");
}

TEST(Examples, Functions) {
  ::lamscript::runtime::Lamscript::RunFile("examples/func.ls");
}

TEST(Examples, Getters) {
  ::lamscript::runtime::Lamscript::RunFile("examples/getters.ls");
}

TEST(Examples, ClassInheritance) {
  ::lamscript::runtime::Lamscript::RunFile("examples/inheritance.ls");
}

TEST(Examples, LogicalOperators) {
  ::lamscript::runtime::Lamscript::RunFile("examples/logical.ls");
}

TEST(Examples, Loops) {
  ::lamscript::runtime::Lamscript::RunFile("examples/loops.ls");
}

TEST(Examples, MathOperators) {
  ::lamscript::runtime::Lamscript::RunFile("examples/math.ls");
}

TEST(Examples, PrintStatement) {
  ::lamscript::runtime::Lamscript::RunFile("examples/print.ls");
}

TEST(Examples, Recursion) {
  ::lamscript::runtime::Lamscript::RunFile("examples/recursion.ls");
}

TEST(Examples, SuperExpression) {
  ::lamscript::runtime::Lamscript::RunFile("examples/super.ls");
}

TEST(Examples, Variables) {
  ::lamscript::runtime::Lamscript::RunFile("examples/variable.ls");
}
