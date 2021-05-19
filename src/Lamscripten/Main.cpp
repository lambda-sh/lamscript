#include <Lamscripten/core/Common.h>
#include <Lamscripten/core/Chunk.h>
#include <Lamscripten/core/Types.h>

#include <Lamscripten/util/Debug.h>

using lamscripten::core::OpCode;
using lamscripten::core::DynamicArray;

int main(int argc, const char* argv[]) {
  lamscripten::core::Chunk chunk;
  size_t _ = chunk.WriteOpCode(OpCode::Return);

  auto location = static_cast<uint8_t>(chunk.AddConstant(1.2));

  chunk.WriteOpCode(OpCode::Constant);
  chunk.WriteBytes({location});

  lamscripten::core::DynamicArray<int> int_array;
  _ = int_array.PushMemory(20);
  int val = int_array.Pop().value_or(222);
  int val2 = int_array.Pop().value_or(5000);

  lamscripten::util::DisassembleChunk(chunk, "Disassembled opcode chunk");
  return 0;
}
