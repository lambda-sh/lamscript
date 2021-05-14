#ifndef SRC_LAMSCRIPTEN_CORE_CHUNK_H_
#define SRC_LAMSCRIPTEN_CORE_CHUNK_H_

#include <algorithm>
#include <cstdint>

#include <Lamscripten/core/Memory.h>

namespace lamscripten::core {

/// @brief Opcode types
enum class OpCode {
  Return = 0,
};

/// @brief A dynamic array of opcodes
class Chunk {
 public:
  Chunk() : count_(0), capacity_(0), opcode_array_(nullptr) {}

  /// @brief Write bytes into the current chunk.
  void WriteOpCode(OpCode code) {
    if (capacity_ < count_ + 1) {
      size_t old_capacity = capacity_;
      capacity_ = (capacity_ < 8) ? 8 : capacity_ * 2;

      opcode_array_ = ReallocateArray<OpCode>(
          opcode_array_, old_capacity, capacity_);
    }

    opcode_array_[count_] = code;
    count_ += 1;
  }

  [[nodiscard]] size_t GetCount() const {
    return count_;
  }

  [[nodiscard]] OpCode GetOpcodeAt(size_t index) const {
    return opcode_array_[index];
  }

  [[nodiscard]] auto begin() const {
    return opcode_array_;
  }

  [[nodiscard]] auto end() const {
    return opcode_array_ + capacity_;
  }

 private:
  size_t count_;
  size_t capacity_;
  OpCode* opcode_array_;
};

}  // namespace lamscripten::core

#endif  // SRC_LAMSCRIPTEN_CORE_CHUNK_H_
