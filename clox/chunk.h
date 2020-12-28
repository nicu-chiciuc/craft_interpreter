#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_ADD, 
  OP_SUBTRACT, 
  OP_MULTIPLY, 
  OP_DIVIDE, 
  OP_NEGATE,
  OP_RETURN,
} OpCode;

typedef struct {
  int count;
  int capacity;
  uint8_t* code;
  // So that during runtime errors we know the line on which the
  // error happened in the original source code
  int* lines;
  ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
// We ought to use writeConstant instead of managing constant ourselves
// For now at least
int addConstant(Chunk* chunk, Value value);
void writeConstant(Chunk* chunk, Value value, int line);
void freeChunk(Chunk* chunk);

#endif
