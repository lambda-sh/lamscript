#include "gtest/gtest.h"

#include <Lamscript/parsing/Scanner.h>

TEST(Scanner, SuccessfullyParseTokens) {
  lamscript::parsing::Scanner scanner("print \"Hello, world!\"");
  scanner.ScanTokens();
}
