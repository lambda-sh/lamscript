#ifndef LAMSCRIPT_SRC_LAMSCRIPT_LAMSCRIPT_H_
#define LAMSCRIPT_SRC_LAMSCRIPT_ERRORS_H_

#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include <Lamscript/scanner.h>

namespace lamscript {

class Lamscript {
 public:
  static void Run(const std::string& source);
  static void RunFile(const std::string& file_path);
  static void RunPrompt();
  static void Error(int line, const std::string& message);
  static void Report(
      int line, const std::string& where, const std::string& message);
 private:
  static bool had_error_;
};



}  // namespace lamscript

#endif  // LAMSCRIPT_SRC_LAMSCRIPT_LAMSCRIPT_H_
