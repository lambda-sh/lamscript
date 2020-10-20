#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include <Lamscript/AstPrinter.h>
#include <Lamscript/Expression.h>
#include <Lamscript/Lamscript.h>
#include <Lamscript/Token.h>
#include <Lamscript/TokenType.h>

int main(int argc, char** argv) {
  lamscript::Expression* expression = new lamscript::Binary(
      new lamscript::Unary(
          lamscript::Token{lamscript::MINUS, "-", nullptr, 1},
          new lamscript::Literal(123)),
      lamscript::Token{lamscript::STAR, "*", nullptr, 1},
      new lamscript::Grouping(
          new lamscript::Literal(45.67)));

  std::cout
      << std::any_cast<std::string>(
          (new lamscript::AstPrinter())->Print(expression));

  exit(0);

  if (argc > 2) {
    std::cout << "Usage: lamscript [script]" << std::endl;
    exit(64);
  } else if (argc == 2) {
    std::cout << "This would run the script: " << argv[1] << std::endl;
    lamscript::Lamscript::RunFile(argv[1]);
    exit(0);
  } else {
    lamscript::Lamscript::RunPrompt();
    exit(0);
  }
}
