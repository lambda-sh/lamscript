#ifndef SRC_LAMSCRIPTEN_CORE_CHUNK_H_
#define SRC_LAMSCRIPTEN_CORE_CHUNK_H_

#include <algorithm>
#include <cstdint>

#include <Lamscripten/core/Memory.h>

namespace lamscripten::core {

///@brief Opcode types
enum class OpCode {
  Return = 0,
};

///@brief A dynamic array of opcodes
class Chunk {
 public:
  Chunk() : count_(0), capacity_(0), opcode_array_(nullptr) {}

  /// @brief Write bytes into the current chunk.
  void WriteByte(std::uint8_t byte) {
    if (capacity_ < count_ + 1) {
      size_t old_capacity = capacity_;
      capacity_ = (capacity_ < 8) ? 8 : capacity_ * 2;

      opcode_array_ = ReallocateArray<std::uint8_t>(
          opcode_array_, old_capacity, capacity_);
    }

    opcode_array_[count_] = byte;
    count_ += 1;
  }

 private:
  size_t count_;
  size_t capacity_;
  std::uint8_t* opcode_array_;
};

}  // namespace lamscripten::core

#endif  // SRC_LAMSCRIPTEN_CORE_CHUNK_H_