#ifndef CPD_MODULE_LR35902_H
#define CPD_MODULE_LR35902_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Disassembly module for Sharp LR35902 CPU architecture.
 */
 
/*typedef struct {
  int first;
  int second;
  int third;
} SubDataSharpLR35902; */

/* Op list */

extern OpInfo opcodesLR35902[];

void destructorOpcodeLR35902(Opcode* obj);

void ModuleLR35902destroyInternal(DismModule* obj);

/* Init functions */

void initModuleLR35902(DismModule* obj);
DismModule* allocModuleLR35902();
void freeModuleLR35902(DismModule* obj);


#endif
