#include "gtest/gtest.h"

#include <Lamscript/parsing/Scanner.h>
#include <Lamscript/parsing/Token.h>

using lamscript::parsing::Scanner;
using lamscript::parsing::Token;
using lamscript::parsing::TokenType;

TEST(Scanner, SuccessfullyParseTokens) {
  Scanner scanner("print \"Hello, world!\"");
  scanner.ScanTokens();
}

TEST(Scanner, SuccessfullyParsePrint) {
  Scanner scanner("print \"Hello, world!\"");
  const std::vector<Token> tokens = scanner.ScanTokens();

  ASSERT_EQ(tokens.size(), 3);

  const Token& print = tokens[0];
  ASSERT_EQ(print.Lexeme, "print");

  const Token& literal = tokens[1];
  ASSERT_EQ(literal.Lexeme, "\"Hello, world!\"");

  const Token& eof = tokens[2];
  ASSERT_EQ(eof.Type, TokenType::END_OF_FILE);
}

TEST(Scanner, SuccessfullyParse) {}
