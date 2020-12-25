#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


int main(int argc, const char* argv[]) {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  writeConstant(&chunk, 1.2, 12);

  writeChunk(&chunk, OP_RETURN, 12);

  disassembleChunk(&chunk, "test chunk");

  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return 0;
}
