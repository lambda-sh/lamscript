#include <iostream>
#include <ios>
#include <fstream>
#include <vector>

#include <Lamscript/Lamscript.h>

int main(int argc, char** argv) {
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
