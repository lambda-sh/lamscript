#include "gtest/gtest.h"

#include <Lamscript/parsing/Scanner.h>
#include <Lamscript/parsing/Token.h>

using lamscript::parsing::Scanner;
using lamscript::parsing::Token;
using lamscript::parsing::TokenType;

TEST(Scanner, SuccessfullyScanAnything) {
  Scanner scanner("print \"Hello, world!\"");
  scanner.ScanTokens();
}

TEST(Scanner, SuccessfullyScanNumber) {
  Scanner scanner("34;");
  const std::vector<Token> tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 3);

  const Token& number = tokens[0];
  ASSERT_EQ(number.Lexeme, "34");

  const Token& semicolon = tokens[1];
  ASSERT_EQ(semicolon.Lexeme, ";");

  const Token& eof = tokens[2];
  ASSERT_EQ(eof.Type, TokenType::END_OF_FILE);
}

TEST(Scanner, SuccessfullyScanPrint) {
  Scanner scanner("print \"Hello, world!\";");
  const std::vector<Token> tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 4);

  const Token& print = tokens[0];
  ASSERT_EQ(print.Lexeme, "print");

  const Token& literal = tokens[1];
  ASSERT_EQ(literal.Lexeme, "\"Hello, world!\"");

  const Token& semicolon = tokens[2];
  ASSERT_EQ(semicolon.Lexeme, ";");

  const Token& eof = tokens[3];
  ASSERT_EQ(eof.Type, TokenType::END_OF_FILE);
}
