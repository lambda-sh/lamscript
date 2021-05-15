#include <Lamscripten/core/Common.h>
#include <Lamscripten/core/Chunk.h>
#include <Lamscripten/core/Types.h>

#include <Lamscripten/util/Debug.h>

using lamscripten::core::OpCode;

int main(int argc, const char* argv[]) {
  lamscripten::core::Chunk chunk;
  chunk.WriteOpCode(OpCode::Return);
  size_t index = chunk.WriteConstant(1.2);
  chunk.WriteOpCode(OpCode::Constant);

  lamscripten::core::DynamicArray<int> int_array;
  int_array.Push(20);
  int val = int_array.Pop().value_or(222);
  int val2 = int_array.Pop().value_or(5000);

  lamscripten::util::DisassembleChunk(chunk, "Disassembled opcode chunk");
  return 0;
}
