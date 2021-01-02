#include <Lamscript/runtime/Lamscript.h>

#include <any>
#include <memory>

#include <Lamscript/errors/RuntimeError.h>
#include <Lamscript/parsing/Parser.h>
#include <Lamscript/parsing/Resolver.h>
#include <Lamscript/parsing/Scanner.h>

namespace lamscript {
namespace runtime {

std::shared_ptr<Interpreter> Lamscript::interpreter_ = std::make_shared<
    Interpreter>();

bool Lamscript::had_error_ = false;

bool Lamscript::had_runtime_error_ = false;

/// @brief Run the given source.
ProgramResult Lamscript::Run(const std::string& source) {
  parsing::Scanner scanner = parsing::Scanner(source);
  std::vector<parsing::Token> tokens = scanner.ScanTokens();

  parsing::Parser parser = parsing::Parser(tokens);
  std::vector<std::unique_ptr<parsed::Statement>> statements = parser.Parse();

  if (had_error_) {
    had_error_ = false;
    return ProgramResult{ProgramStatus::FailedAtParser, 65};
  }

  parsing::Resolver resolver = parsing::Resolver(interpreter_);
  resolver.Resolve(statements);

  if (had_error_) {
    had_error_ = false;
    return ProgramResult{ProgramStatus::FailedAtResolver, 65};
  }

  interpreter_->Interpret(statements);

  if (had_runtime_error_) {
    return ProgramResult{ProgramStatus::FailedAtInterpeter, 70};
  }

  return ProgramResult{ProgramStatus::Success, 0};
}

/// @brief Run a given file.
///
/// This functions throws if it cannot successfully open the file.
ProgramResult Lamscript::RunFile(const std::string& file_path)  {
  std::ifstream source_file(file_path, std::ios::in | std::ios::binary);
  std::string source_code;

  if (source_file) {
    source_file.seekg(0, std::ios::end);
    source_code.resize(source_file.tellg());
    source_file.seekg(0, std::ios::beg);
    source_file.read(&source_code[0], source_code.size());
  } else {
    return ProgramResult{ProgramStatus::FailedAtReadingFile, 1};
  }

  return Run(source_code);
}

/// @brief Runs the prompt for the interpreter.
ProgramResult Lamscript::RunPrompt() {
  bool running = true;
  std::string source_line;

  while (running) {
    std::cout << "> ";
    std::getline(std::cin, source_line);

    if (!source_line.empty()) {
      Run(source_line);
    }
    source_line.clear();
  }
  return ProgramResult{ProgramStatus::Success, 0};
}

/// @brief Report an error
void Lamscript::Error(int line, const std::string& message) {
  Lamscript::Report(line, "", message);
}

void Lamscript::Error(parsing::Token token, const std::string& message) {
  if (token.Type == parsing::END_OF_FILE) {
    Report(token.Line, " at end", message);
  } else {
    Report(token.Line, " at '" + token.Lexeme + "'", message);
  }
}

void Lamscript::RuntimeError(lamscript::RuntimeError error) {
  std::cout
      << "[line "
      << error.GetToken().Line
      << "] RuntimeError"
      << ": "
      << error.what()
      << std::endl;

  Lamscript::had_runtime_error_ = true;
}

/// @brief Report function.
void Lamscript::Report(
    int line, const std::string& where, const std::string& message) {
  had_error_ = true;
  std::cout
      << "[line "
      << line
      << "] Error"
      << where
      << ": "
      << message
      << std::endl;
}

}  // namespace runtime
}  // namespace lamscript
