#ifndef CPD_MODULE_65C02_H
#define CPD_MODULE_65C02_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Disassembly module for WDC 65C02.
 */

/* Op list */

extern OpInfo opcodes65C02[];

/* Subop data structs */

/*typedef struct Struct65C02Single {
  int value;
} Struct65C02Single; */

typedef struct Struct65C02Dual {
  int value1;
  int value2;
} Struct65C02Dual;
 
/* Opcode function overrides (one set per addressing mode) */

#define GENERATE_65C02_OPCODE_FUNCTIONS_DECLARATION(ADDRESSING_MODE) \
void generate65C02 ## ADDRESSING_MODE( \
                      OpInfo* opInfo, Opcode* dst, \
                      DismSettings* config); \
OpcodeSimilarity compare65C02 ## ADDRESSING_MODE( \
                                 Opcode* obj, Opcode* other, \
                                 DismSettings* config); \
unsigned int readStep65C02 ## ADDRESSING_MODE( \
                              Opcode* obj, BufferStream* stream, \
                              DismSettings* config, MapSS* args); \
void printString65C02  ## ADDRESSING_MODE( \
                         Opcode* obj, String* dst, \
                         DismSettings* config);

#define GENERATE_65C02_OPCODE_GENERATION_DEFINITION(ADDRESSING_MODE) \
void generate65C02 ## ADDRESSING_MODE(OpInfo* opInfo, Opcode* dst, \
                      DismSettings* config) { \
  dst->compare = compare65C02 ## ADDRESSING_MODE; \
  dst->readStep = readStep65C02 ## ADDRESSING_MODE; \
  dst->printString = printString65C02 ## ADDRESSING_MODE; \
  dst->printName = OpcodeprintNameWithSpace; \
  dst->setData(dst, malloc(sizeof(Struct6502Single))); \
}


/* AbsIndX */
GENERATE_65C02_OPCODE_FUNCTIONS_DECLARATION(AbsIndX);

/* ZPInd (this is actually just indirect) */
/*GENERATE_65C02_OPCODE_FUNCTIONS_DECLARATION(ZPInd); */
GENERATE_65C02_OPCODE_FUNCTIONS_DECLARATION(ZPInd);

/* ZPRel */
GENERATE_65C02_OPCODE_FUNCTIONS_DECLARATION(ZPRel);

/* Init functions */

void initModule65C02(DismModule* obj);
DismModule* allocModule65C02();
void freeModule65C02(DismModule* obj);


#endif
