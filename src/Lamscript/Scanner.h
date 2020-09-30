#ifndef SRC_LAMSCRIPT_SCANNER_H_
#define SRC_LAMSCRIPT_SCANNER_H_

#include <string>
#include <vector>

#include <Lamscript/Token.h>

namespace lamscript {


/// @brief Lightweight scanner class.
class Scanner {
 public:
  explicit Scanner(std::string source) : source_(source) {}
  std::vector<Token> ScanTokens() { return std::vector<Token>(); }
 private:
  std::string source_;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_SCANNER_H_
