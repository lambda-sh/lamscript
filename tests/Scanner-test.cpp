#include "gtest/gtest.h"

#include <Lamscript/parsing/Scanner.h>
#include <Lamscript/parsing/Token.h>

using ::lamscript::parsing::Scanner;
using ::lamscript::parsing::Token;
using ::lamscript::parsing::TokenType;

TEST(Scanner, ScanAnything) {
  Scanner scanner("print \"Hello, world!\"");
  scanner.ScanTokens();
}

TEST(Scanner, ScanNumberLiteral) {
  Scanner scanner("34;");
  const std::vector<Token> tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 3);

  const Token& number = tokens[0];
  EXPECT_EQ(number.Type, TokenType::NUMBER);
  EXPECT_EQ(number.Lexeme, "34");

  const Token& semicolon = tokens[1];
  EXPECT_EQ(semicolon.Type, TokenType::SEMICOLON);
  EXPECT_EQ(semicolon.Lexeme, ";");

  const Token& eof = tokens[2];
  EXPECT_EQ(eof.Type, TokenType::END_OF_FILE);
  EXPECT_EQ(eof.Lexeme, "");
}

TEST(Scanner, ScanBoolean) {
  Scanner scanner("true and false;");
  const std::vector<Token> tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 5);

  const Token& true_token = tokens[0];
  EXPECT_EQ(true_token.Type, TokenType::TRUE);
  EXPECT_EQ(true_token.Lexeme, "true");

  const Token& and_token = tokens[1];
  EXPECT_EQ(and_token.Type, TokenType::AND);
  EXPECT_EQ(and_token.Lexeme, "and");

  const Token& false_token = tokens[2];
  EXPECT_EQ(false_token.Type, TokenType::FALSE);
  EXPECT_EQ(false_token.Lexeme, "false");

  const Token& semicolon = tokens[3];
  EXPECT_EQ(semicolon.Type, TokenType::SEMICOLON);
  EXPECT_EQ(semicolon.Lexeme, ";");

  const Token& eof = tokens[4];
  EXPECT_EQ(eof.Type, TokenType::END_OF_FILE);
}

TEST(Scanner, ScanPrint) {
  Scanner scanner("print \"Hello, world!\";");
  const std::vector<Token> tokens = scanner.ScanTokens();
  ASSERT_EQ(tokens.size(), 4);

  const Token& print = tokens[0];
  EXPECT_EQ(print.Type, TokenType::PRINT);
  EXPECT_EQ(print.Lexeme, "print");

  const Token& literal = tokens[1];
  EXPECT_EQ(literal.Type, TokenType::STRING);
  EXPECT_EQ(literal.Lexeme, "\"Hello, world!\"");

  const Token& semicolon = tokens[2];
  EXPECT_EQ(semicolon.Type, TokenType::SEMICOLON);
  EXPECT_EQ(semicolon.Lexeme, ";");

  const Token& eof = tokens[3];
  EXPECT_EQ(eof.Type, TokenType::END_OF_FILE);
}
