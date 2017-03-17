#ifndef CPD_DISM_STRUCT_H
#define CPD_DISM_STRUCT_H


#include "util/BufferStream.h"
#include "modules/CodeMap.h"
#include "modules/DismSettings.h"
#include "util/Vectors.h"

/**
 * Simple data struct containing needed materials for disassembly.
 */
typedef struct {

  /**
   * Disassembly settings.
   */
  DismSettings settings;

  /**
   * BufferStream containing raw input file.
   */
  BufferStream* stream;
  
  /**
   * CodeMap identifying known code and data areas.
   */
  CodeMap* codeMap;
  
  /**
   * Vector of Opcodes representing the disassembled stream.
   */
  VectorOpcode opcodes;

} DismStruct;

/**
 * Initializer.
 */
void initDismStruct(DismStruct* obj);

/**
 * Allocator.
 */
DismStruct* allocDismStruct();

/**
 * Deallocator.
 */
void freeDismStruct(DismStruct* obj);


#endif
