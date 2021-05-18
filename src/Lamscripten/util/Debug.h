#ifndef SRC_LAMSCRIPTEN_UTIL_DEBUG_H_
#define SRC_LAMSCRIPTEN_UTIL_DEBUG_H_

#include <string>
#include <string_view>
#include <iomanip>
#include <iostream>

#include <Lamscripten/core/Chunk.h>

namespace lamscripten::util {

[[nodiscard]] inline int DisassembleInstruction(
    const core::Chunk& chunk, size_t opcode_index) {
  std::cout << opcode_index << ":";

  auto op_or_null = chunk.GetOpcodeAt(opcode_index);

  [[unlikely]] if (!op_or_null.has_value()) {
    std::cout << "INVALID OP INDEX @ " << opcode_index << std::endl;
    return -1;
  }

  core::OpCode op = op_or_null.value();

  switch (op.GetType()) {
    case core::OpType::Return:
      std::cout << "OP_RETURN" << std::endl;
      break;
    case core::OpType::Constant:
      int const_index = op.GetBytes().GetAtIndex(0).value_or(100);
      std::cout
          << "OP_CONSTANT @ "
          << const_index
          << ":"
          << chunk.GetConstantAt(0).value_or(0)
          << std::endl;
      break;
  }
}

inline void DisassembleChunk(
    const core::Chunk& chunk, std::string_view name) {
  std::cout << "== " << name << " ==" << std::endl;
  for (
      size_t current_opcode = 0;
      current_opcode < chunk.GetOpCodeCount();
      current_opcode++) {
    DisassembleInstruction(chunk, current_opcode);
  }
}

}  // namespace lamscripten::util

#endif  // SRC_LAMSCRIPTEN_UTIL_DEBUG_H_
