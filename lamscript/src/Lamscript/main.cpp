#include <iostream>
#include <ios>
#include <fstream>

namespace lamscript {

/// @brief Run a given line of source.
void Run(const std::string& source) {

}

/// @brief Run a given file.
///
/// This functions throws if it cannot successfully open the file.
void RunFile(const std::string& file_path)  {
  std::ifstream source_file(file_path, std::ios::in | std::ios::binary);
  std::string source_as_string;

  if (source_file) {
    source_file.seekg(0, std::ios::end);
    source_as_string.resize(source_file.tellg());
    source_file.seekg(0, std::ios::beg);
    source_file.read(&source_as_string[0], source_as_string.size());
  } else {
    throw "The Input file could not be read";
  }

  Run(source_as_string);
}

/// @brief Runs the prompt for the interpreter.
void RunPrompt() {
  bool running = true;

  while (running) {
    std::cout << "> " << std::endl;
    std::string source_line;
    std::cin >> source_line;

    if (!source_line.empty()) {
      Run(source_line);
    }
  }

}

}  // namespace lamscript

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cout << "Usage: lamscript [script]" << std::endl; 
    exit(64);
  } else if (argc == 2) {
    std::cout << "This would run the script: " << argv[1] << std::endl;
    lamscript::RunFile(argv[1]);
    exit(0);
  } else {
    lamscript::RunPrompt();
    exit(0);
  }
}
