#include <gtest/gtest.h>

#include <Lamscript/runtime/Lamscript.h>

using ::lamscript::runtime::ProgramStatus;
using ::lamscript::runtime::ProgramResult;
using ::lamscript::runtime::Lamscript;

TEST(Examples, AnonymousClasses) {
  ProgramResult result = Lamscript::RunFile("examples/anonymous.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Blocks) {
  ProgramResult result = Lamscript::RunFile("examples/blocks.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Classes) {
  ProgramResult result = Lamscript::RunFile("examples/class.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Closures) {
  ProgramResult result = Lamscript::RunFile("examples/closure.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Conditionals) {
  ProgramResult result = Lamscript::RunFile("examples/conditionals.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Functions) {
  ProgramResult result = Lamscript::RunFile("examples/func.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Getters) {
  ProgramResult result = Lamscript::RunFile("examples/getters.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, ClassInheritance) {
  ProgramResult result = Lamscript::RunFile("examples/inheritance.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, LogicalOperators) {
  ProgramResult result = Lamscript::RunFile("examples/logical.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Loops) {
  ProgramResult result = Lamscript::RunFile("examples/loops.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, MathOperators) {
  ProgramResult result = Lamscript::RunFile("examples/math.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, PrintStatement) {
  ProgramResult result = Lamscript::RunFile("examples/print.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Recursion) {
  ProgramResult result = Lamscript::RunFile("examples/recursion.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, SuperExpression) {
  ProgramResult result = Lamscript::RunFile("examples/super.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}

TEST(Examples, Variables) {
  ProgramResult result = Lamscript::RunFile("examples/variable.ls");
  ASSERT_EQ(result.Status, ProgramStatus::Success);
  EXPECT_EQ(result.ReturnCode, 0);
}
