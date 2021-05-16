#include <Lamscripten/core/Common.h>
#include <Lamscripten/core/Chunk.h>
#include <Lamscripten/core/Types.h>

#include <Lamscripten/util/Debug.h>

using lamscripten::core::OpCode;
using lamscripten::core::OpType;
using lamscripten::core::DynamicArray;

int main(int argc, const char* argv[]) {
  lamscripten::core::Chunk chunk;
  chunk.WriteOpCode(OpType::Return);

  auto index = static_cast<uint8_t>(chunk.WriteConstant(1.2));

  chunk.WriteOpCode(OpCode(OpType::Constant, DynamicArray({index})));

  lamscripten::core::DynamicArray<int> int_array;
  int_array.Push(20);
  int val = int_array.Pop().value_or(222);
  int val2 = int_array.Pop().value_or(5000);

  lamscripten::util::DisassembleChunk(chunk, "Disassembled opcode chunk");
  return 0;
}
