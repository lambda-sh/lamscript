#ifndef SRC_LAMSCRIPTEN_UTIL_DEBUG_H_
#define SRC_LAMSCRIPTEN_UTIL_DEBUG_H_

#include <string>
#include <string_view>
#include <iostream>

#include <Lamscripten/core/Chunk.h>

namespace lamscripten::util {

[[nodiscard]] inline int DisassembleInstruction(
    const core::Chunk& chunk, size_t opcode_index) {
  std::cout << opcode_index << ":";
  core::OpCode code = chunk.GetOpcodeAt(opcode_index);

  switch (code) {
    case core::OpCode::Return:
      std::cout << "OP_RETURN" << std::endl;
      break;
    case core::OpCode::InvalidOpLookup:
      std::cout << "INVALID_OP_LOOKUP " << std::endl;
      break;
    case core::OpCode::Constant:
      std::cout << "" << std::endl;
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
