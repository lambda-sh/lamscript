#ifndef SRC_LAMSCRIPTEN_CORE_CHUNK_H_
#define SRC_LAMSCRIPTEN_CORE_CHUNK_H_

#include <algorithm>
#include <cstdint>

#include <Lamscripten/core/Memory.h>
#include <Lamscripten/core/Types.h>

namespace lamscripten::core {

/// @brief Opcode types
enum class OpCode {
  InvalidOpLookup,
  Return,
  Constant,
};

/// @brief A dynamic array of opcodes
class Chunk {
 public:
  Chunk() : count_(0), capacity_(0), opcode_array_(), constants_() {}

  /// @brief Write bytes into the current chunk.
  [[nodiscard]] size_t WriteOpCode(OpCode code) {
    size_t index = opcode_array_.Push(code);
    return index;
  }

  [[nodsicard]] size_t WriteConstant(double value) {
    size_t index = constants_.Push(value);
    return index;
  }

  [[nodiscard]] size_t GetOpCodeCount() const {
    return opcode_array_.GetCount();
  }

  [[nodiscard]] OpCode GetOpcodeAt(size_t index) const {
    return opcode_array_.GetAtIndex(index).value_or(OpCode::InvalidOpLookup);
  }

  [[nodiscard]] auto begin() const {
    return opcode_array_.begin();
  }

  [[nodiscard]] auto end() const {
    return opcode_array_.end();
  }

 private:
  size_t count_;
  size_t capacity_;
  Array<OpCode> opcode_array_;
  Array<double> constants_;
};

}  // namespace lamscripten::core

#endif  // SRC_LAMSCRIPTEN_CORE_CHUNK_H_
