#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include <Lamscript/Lamscript.h>
#include <Lamscript/parsing/Token.h>
#include <Lamscript/parsing/TokenType.h>
#include <Lamscript/parsable/Expression.h>

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cout << "Usage: lamscript [script]" << std::endl;
    exit(64);
  } else if (argc == 2) {
    lamscript::Lamscript::RunFile(argv[1]);
    exit(0);
  } else {
    lamscript::Lamscript::RunPrompt();
    exit(0);
  }
}
