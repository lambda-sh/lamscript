#ifndef SRC_LAMSCRIPT_RUNTIME_LAMSCRIPT_H_
#define SRC_LAMSCRIPT_RUNTIME_LAMSCRIPT_H_

#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include <Lamscript/errors/RuntimeError.h>
#include <Lamscript/parsing/Token.h>
#include <Lamscript/runtime/Interpreter.h>

namespace lamscript {
namespace runtime {

/// @brief The status of a programs execution.
enum class ProgramStatus {
  Success,
  FailedAtReadingFile,
  FailedAtScanner,
  FailedAtParser,
  FailedAtResolver,
  FailedAtInterpeter
};

/// @brief Stores the result of any given programs execution result.
struct ProgramResult{
  ProgramStatus Status;
  int ReturnCode;
  std::string Message;
};

class Lamscript {
 public:
  static ProgramResult Run(const std::string& source);
  static ProgramResult RunFile(const std::string& file_path);
  static ProgramResult RunPrompt();
  static void Error(int line, const std::string& message);
  static void Error(parsing::Token token, const std::string& message);
  static void RuntimeError(lamscript::RuntimeError error);
  static void Report(
      int line, const std::string& where, const std::string& message);
 private:
  static std::shared_ptr<Interpreter> interpreter_;
  static bool had_error_, had_runtime_error_;
};

}  // namespace runtime
}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_RUNTIME_LAMSCRIPT_H_
