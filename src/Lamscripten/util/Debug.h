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
  std::cout << "\t" << opcode_index << ":";

  auto op_or_null = chunk.GetOpcodeAt(opcode_index);

  [[unlikely]] if (!op_or_null.has_value()) {
    std::cout << "INVALID OP INDEX @ " << opcode_index << std::endl;
    return -1;
  }

  auto op = core::OpCode(op_or_null.value());

  switch (op) {
    case core::OpCode::Return:
      std::cout << "OP_RETURN" << std::endl;
      return opcode_index + 1;
    case core::OpCode::Constant:
      uint8_t const_index = chunk.GetOpcodeAt(opcode_index + 1).value_or(0);
      std::cout
          << "OP_CONSTANT @ index "
          << const_index
          << " with a value of: "
          << chunk.GetConstantAt(const_index).value_or(0)
          << std::endl;
      return opcode_index + 2;
  }
}

inline void DisassembleChunk(
    const core::Chunk& chunk, std::string_view name) {
  std::cout << "== " << name << " ==" << std::endl;

  size_t chunk_index = 0;
  while (chunk_index < chunk.GetOpCodeCount()) {
    chunk_index = DisassembleInstruction(chunk, chunk_index);
  }
}

}  // namespace lamscripten::util

#endif  // SRC_LAMSCRIPTEN_UTIL_DEBUG_H_
