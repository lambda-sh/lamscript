#include <Lamscripten/core/Common.h>
#include <Lamscripten/core/Chunk.h>

int main(int argc, const char* argv[]) {
  lamscripten::core::Chunk chunk;
  chunk.WriteByte(0); 
  return 0;
}