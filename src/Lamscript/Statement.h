#ifndef SRC_LAMSCRIPT_STATEMENT_H_
#define SRC_LAMSCRIPT_STATEMENT_H_

#include <any>

namespace lamscript {

class StatementVisitor;

class Statement {
  virtual std::any Accept(StatementVisitor) = 0;
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_STATEMENT_H_
