#ifndef SRC_LAMSCRIPTEN_CORE_CHUNK_H_
#define SRC_LAMSCRIPTEN_CORE_CHUNK_H_

#include <algorithm>
#include <cstdint>
#include <optional>

#include <Lamscripten/core/Memory.h>
#include <Lamscripten/core/Types.h>

namespace lamscripten::core {

/// @brief Opcode types
enum class OpCode : std::uint16_t {
  NoOp,
  Return,
  Constant,
  NextLine
};

/// @brief A dynamic array of opcodes
class Chunk {
 public:
  Chunk() : opcodes_(), constants_() {}

  /// @brief Writes an OpCode into the chunk.
  [[nodiscard]] size_t WriteOpCode(OpCode code) {
    size_t index = opcodes_.PushMemory(static_cast<uint16_t>(code));
    return index;
  }

  /// @brief Returns the start index of where the bytes are written to within
  /// the chunk.
  [[nodiscard]] size_t WriteBytes(std::initializer_list<uint16_t> bytes) {
    size_t start_index = opcodes_.GetCount() - 1;

    for (auto byte : bytes) {
      size_t _ = opcodes_.PushCopy(byte);
    }

    return start_index;
  }

  [[nodsicard]] size_t AddConstant(double value) {
    size_t index = constants_.PushCopy(value);
    return index;
  }

  [[nodiscard]] size_t GetOpCodeCount() const {
    return opcodes_.GetCount();
  }

  [[nodiscard]] DynamicArray<uint16_t> GetBytesFromLine(size_t line) {
    DynamicArray<uint16_t> bytes;
    size_t line_count = 0;

    for (auto& byte : opcodes_) {
      if (line == line_count) {
        bytes.PushCopy(byte);
      }
      if (line > line_count) {
        return bytes;
      }

      if (static_cast<OpCode>(byte) == OpCode::NextLine) {
        line_count += 1;
      }
    }

    return bytes;
  }

  [[nodiscard]] std::optional<uint16_t> GetOpcodeAt(size_t index) const {
    return opcodes_.GetAtIndex(index);
  }

  [[nodiscard]] std::optional<double> GetConstantAt(size_t index) const {
    return constants_.GetAtIndex(index);
  }

  [[nodiscard]] auto begin() const {
    return opcodes_.begin();
  }

  [[nodiscard]] auto end() const {
    return opcodes_.end();
  }

 private:
  DynamicArray<uint16_t> opcodes_;
  DynamicArray<double> constants_;
  size_t line_count_;
};

}  // namespace lamscripten::core

#endif  // SRC_LAMSCRIPTEN_CORE_CHUNK_H_
