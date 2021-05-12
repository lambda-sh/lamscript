#include <Lamscripten/core/Common.h>
#include <Lamscripten/core/Chunk.h>

#include <Lamscripten/util/Debug.h>

int main(int argc, const char* argv[]) {
  lamscripten::core::Chunk chunk;
  chunk.WriteOpCode(lamscripten::core::OpCode::Return); 
  lamscripten::util::DisassembleChunk(chunk, "Disassembled opcode chunk");
  return 0;
}