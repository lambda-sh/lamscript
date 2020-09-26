#ifndef LAMSCRIPT_SRC_LAMSCRIPT_SCANNER_H_
#define LAMSCRIPT_SRC_LAMSCRIPT_SCANNER_H_

#include <string>
#include <vector>

namespace lamscript {

struct Token {};

/// @brief Lightweight scanner class.
class Scanner {
 public:
  Scanner(std::string source) : source_(source) {}
  std::vector<Token> ScanTokens() { return std::vector<Token>(); }
 private:
  std::string source_;
};

}  // namespace lamscript
#endif
