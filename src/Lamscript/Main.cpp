#include <iostream>

#include <Lamscript/runtime/Lamscript.h>

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cout << "Usage: lamscript [script]" << std::endl;
    exit(64);
  } else if (argc == 2) {
    lamscript::runtime::Lamscript::RunFile(argv[1]);
    exit(0);
  } else {
    lamscript::runtime::Lamscript::RunPrompt();
    exit(0);
  }
}
