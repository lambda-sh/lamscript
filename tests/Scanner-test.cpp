#include "gtest/gtest.h"

#include <Lamscript/parsing/Scanner.h>
#include <Lamscript/parsing/Token.h>

using lamscript::parsing::Scanner;
using lamscript::parsing::Token;

TEST(Scanner, SuccessfullyParseTokens) {
  Scanner scanner("print \"Hello, world!\"");
  scanner.ScanTokens();
}

TEST(Scanner, SuccessfullyParsePrint) {
  Scanner scanner("print \"Hello, world!\"");
  const std::vector<Token> tokens = scanner.ScanTokens();

  const Token& print = tokens[0];
  ASSERT_EQ(print.Lexeme, "print");

  const Token& literal = tokens[1];
  ASSERT_EQ(literal.Lexeme, "\"Hello, world!\"");
}
