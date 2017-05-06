#ifndef CPD_MODULE_Z80_H
#define CPD_MODULE_Z80_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Disassembly module for Zilog Z80 CPU architecture.
 */
 
/*typedef struct {
  int first;
  int second;
  int third;
} SubDataZ80; */

/* Op list */

extern OpInfo opcodesZ80[];

void destructorOpcodeZ80(Opcode* obj);

void ModuleZ80destroyInternal(DismModule* obj);

OpcodeSimilarity compareOpcodeZ80(Opcode* obj, Opcode* other,
                         DismSettings* config);

void printOpcodeZ80(Opcode* obj, String* dst, DismSettings* config);

unsigned int readStepOpcodeZ80(struct Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args);

/* Init functions */

void initModuleZ80(DismModule* obj);
DismModule* allocModuleZ80();
void freeModuleZ80(DismModule* obj);


#endif
