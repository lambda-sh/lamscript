#ifdef LAMSCRIPT_BUILD_AS_EXECUTABLE

#include <iostream>

#include <Lamscript/runtime/Lamscript.h>

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cout << "Usage: lamscript [script]" << std::endl;
    exit(64);
  } else if (argc == 2) {
    lamscript::runtime::ProgramResult result =
        lamscript::runtime::Lamscript::RunFile(argv[1]);
    exit(result.ReturnCode);
  } else {
    lamscript::runtime::Lamscript::RunPrompt();
    exit(0);
  }
}

#endif
