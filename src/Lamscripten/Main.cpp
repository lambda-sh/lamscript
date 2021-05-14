#include <Lamscripten/core/Common.h>
#include <Lamscripten/core/Chunk.h>
#include <Lamscripten/core/Types.h>

#include <Lamscripten/util/Debug.h>

int main(int argc, const char* argv[]) {
  lamscripten::core::Chunk chunk;
  chunk.WriteOpCode(lamscripten::core::OpCode::Return);

  lamscripten::core::Array<int> int_array;
  int_array.Push(20);
  int val = int_array.Pop().value_or(222);
  int val2 = int_array.Pop().value_or(5000);

  lamscripten::util::DisassembleChunk(chunk, "Disassembled opcode chunk");
  return 0;
}
