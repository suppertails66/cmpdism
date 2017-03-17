#ifndef CPD_MODULE_HUC6280_H
#define CPD_MODULE_HUC6280_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Disassembly module for Hudson HuC6280.
 */

/* Op list */

extern OpInfo opcodesHuC6280[];

/* Subop data structs */

/*typedef struct StructHuC6280Single {
  int value;
} StructHuC6280Single;

typedef struct StructHuC6280Dual {
  int value1;
  int value2;
} StructHuC6280Dual; */

typedef struct StructHuC6280Triple {
  int value1;
  int value2;
  int value3;
} StructHuC6280Triple;
 
/* Opcode function overrides (one set per addressing mode) */

#define GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(ADDRESSING_MODE) \
void generateHuC6280 ## ADDRESSING_MODE( \
                      OpInfo* opInfo, Opcode* dst, \
                      DismSettings* config); \
OpcodeSimilarity compareHuC6280 ## ADDRESSING_MODE( \
                                 Opcode* obj, Opcode* other, \
                                 DismSettings* config); \
unsigned int readStepHuC6280 ## ADDRESSING_MODE( \
                              Opcode* obj, BufferStream* stream, \
                              DismSettings* config, MapSS* args); \
void printStringHuC6280  ## ADDRESSING_MODE( \
                         Opcode* obj, String* dst, \
                         DismSettings* config);

#define GENERATE_HuC6280_OPCODE_GENERATION_DEFINITION(ADDRESSING_MODE) \
void generateHuC6280 ## ADDRESSING_MODE(OpInfo* opInfo, Opcode* dst, \
                      DismSettings* config) { \
  dst->compare = compareHuC6280 ## ADDRESSING_MODE; \
  dst->readStep = readStepHuC6280 ## ADDRESSING_MODE; \
  dst->printString = printStringHuC6280 ## ADDRESSING_MODE; \
  dst->printName = OpcodeprintNameWithSpace; \
  dst->setData(dst, malloc(sizeof(Struct6502Single))); \
}


/* AbsIndX */
GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(AbsIndX);

/* ZPInd (this is actually just indirect) */
/*GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(ZPInd); */

/* ZPRel */
GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(ZPRel);



/* Blk */
GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(Blk);

/* ImAbs */
GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(ImAbs);

/* ImAbsX */
GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(ImAbsX);

/* ImZP */
GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(ImZP);

/* ImZPX */
GENERATE_HuC6280_OPCODE_FUNCTIONS_DECLARATION(ImZPX);

/* Init functions */

void initModuleHuC6280(DismModule* obj);
DismModule* allocModuleHuC6280();
void freeModuleHuC6280(DismModule* obj);


#endif
