#ifndef SRC_LAMSCRIPTEN_CORE_CHUNK_H_
#define SRC_LAMSCRIPTEN_CORE_CHUNK_H_

#include <algorithm>
#include <cstdint>

#include <Lamscripten/core/Memory.h>
#include <Lamscripten/core/Types.h>

namespace lamscripten::core {

/// @brief Opcode types
enum class OpType {
  NoOp,
  Return,
  Constant,
  RawBytes,
};

/// @brief Store an Opcode with or without a raw byte. Certain
/// bytecodes may contain byte data, while others do not.
class OpCode {
 public:
  OpCode() : type_(OpType::NoOp), bytes_() {}

  OpCode(OpType type, const DynamicArray<uint8_t>& bytes)
      : type_(type), bytes_(bytes) {}

  OpCode(OpType type, std::initializer_list<uint8_t> bytes)
    : type_(type), bytes_(bytes) {}

  OpCode(const OpCode& opcode) : type_(opcode.type_), bytes_(opcode.bytes_) {}

  void operator=(const OpCode& opcode) {
    type_ = opcode.type_;
    bytes_ = opcode.bytes_;
  }

  bool HasRawBytes() const {
    return bytes_.GetCount() > 0;
  }

  [[nodiscard]] OpType GetType() const {
    return type_;
  }

  [[nodiscard]] const DynamicArray<uint8_t>& GetBytes() const {
    return bytes_;
  }

 private:
  OpType type_;
  DynamicArray<uint8_t> bytes_;
};

/// @brief A dynamic array of opcodes
class Chunk {
 public:
  Chunk() : count_(0), capacity_(0), opcodes_(), constants_() {}

  /// @brief Write an opcode with no byte information.
  [[nodiscard]] size_t WriteOpCode(OpType type) {
    size_t index = opcodes_.PushMemory(
        OpCode(type, DynamicArray<uint8_t>()));
    return index;
  }

  [[nodiscard]] size_t WriteOpCode(const OpCode& code) {
    size_t index = opcodes_.PushCopy(code);
    return index;
  }

  [[nodiscard]] size_t WriteBytes(const DynamicArray<uint8_t>& bytes) {
    size_t index = opcodes_.PushMemory(OpCode(OpType::RawBytes, bytes));
    return index;
  }

  [[nodsicard]] size_t AddConstant(double value) {
    size_t index = constants_.PushCopy(value);
    return index;
  }

  [[nodiscard]] size_t GetOpCodeCount() const {
    return opcodes_.GetCount();
  }

  [[nodiscard]] std::optional<OpCode> GetOpcodeAt(size_t index) const {
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
  size_t count_;
  size_t capacity_;
  DynamicArray<OpCode> opcodes_;
  DynamicArray<double> constants_;
};

}  // namespace lamscripten::core

#endif  // SRC_LAMSCRIPTEN_CORE_CHUNK_H_
