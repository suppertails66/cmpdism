#ifndef CPD_MODULE_MIPS1_H
#define CPD_MODULE_MIPS1_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Disassembly module for MIPS-I instruction set.
 */
 
typedef struct {
  int first;
  int second;
  int third;
} SubDataMIPS1;

/* Op list */

extern OpInfo opcodesMIPS1[];

void destructorOpcodeMIPS1(Opcode* obj);

void ModuleMIPS1destroyInternal(DismModule* obj);

/* Init functions */

void initModuleMIPS1(DismModule* obj);
DismModule* allocModuleMIPS1();
void freeModuleMIPS1(DismModule* obj);


#endif
