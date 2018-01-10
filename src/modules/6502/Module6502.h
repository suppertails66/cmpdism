#ifndef CPD_MODULE_6502_H
#define CPD_MODULE_6502_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Disassembly module for MOS Technology 6502.
 */
 
OpcodeSimilarity compareAddress6502(unsigned int first, unsigned int second,
                                DismSettings* config);
OpcodeSimilarity compareConstant6502(unsigned int first, unsigned int second,
                                DismSettings* config);
OpcodeSimilarity compareRelative6502(int first, int second,
                                DismSettings* config);

OpcodeSimilarity compare1bAddress6502(Opcode* first, Opcode* second,
                                      DismSettings* config);
OpcodeSimilarity compare2bAddress6502(Opcode* first, Opcode* second,
                                      DismSettings* config);
OpcodeSimilarity compare1bConstant6502(Opcode* first, Opcode* second,
                                      DismSettings* config);
OpcodeSimilarity compare2bConstant6502(Opcode* first, Opcode* second,
                                      DismSettings* config);
OpcodeSimilarity compare1bRelative6502(Opcode* first, Opcode* second,
                                      DismSettings* config);

unsigned int read1bArg6502(Opcode* dst, BufferStream* stream,
                           DismSettings* config);
unsigned int read2bArg6502(Opcode* dst, BufferStream* stream,
                           DismSettings* config);
int read1bRelative6502(Opcode* dst, BufferStream* stream,
                           DismSettings* config);

void print1bAddress6502(String* dst, Opcode* src,
                      DismSettings* config);
void print2bAddress6502(String* dst, Opcode* src,
                      DismSettings* config);
void print1bConstant6502(String* dst, Opcode* src,
                      DismSettings* config);
void print2bConstant6502(String* dst, Opcode* src,
                      DismSettings* config);
void print1bRelative6502(String* dst, Opcode* src,
                     DismSettings* config);
void print1bAddress6502Raw(String* dst, unsigned int value,
                      DismSettings* config);
void print2bAddress6502Raw(String* dst, unsigned int value,
                      DismSettings* config);
void print1bConstant6502Raw(String* dst, unsigned int value,
                      DismSettings* config);
void print2bConstant6502Raw(String* dst, unsigned int value,
                      DismSettings* config);
void print1bRelative6502Raw(String* dst, int value,
                     DismSettings* config,
                     int baseAddress);

int singleValue6502(Opcode* src);

/* Op list */

extern OpInfo opcodes6502[];

/* Subop data structs */

typedef struct Struct6502Single {
  int value;
} Struct6502Single;

typedef struct Struct6502Double {
  int value;
} Struct6502Double;
 
/* Opcode function overrides (one set per addressing mode) */

#define GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(ADDRESSING_MODE) \
void generate6502 ## ADDRESSING_MODE( \
                      OpInfo* opInfo, Opcode* dst, \
                      DismSettings* config); \
OpcodeSimilarity compare6502 ## ADDRESSING_MODE( \
                                 Opcode* obj, Opcode* other, \
                                 DismSettings* config); \
unsigned int readStep6502 ## ADDRESSING_MODE( \
                              Opcode* obj, BufferStream* stream, \
                              DismSettings* config, \
                              MapSS* args); \
void printString6502  ## ADDRESSING_MODE( \
                         Opcode* obj, String* dst, \
                         DismSettings* config);

#define GENERATE_6502_OPCODE_GENERATION_DEFINITION(ADDRESSING_MODE) \
void generate6502 ## ADDRESSING_MODE(OpInfo* opInfo, Opcode* dst, \
                      DismSettings* config) { \
  dst->compare = compare6502 ## ADDRESSING_MODE; \
  dst->readStep = readStep6502 ## ADDRESSING_MODE; \
  dst->printString = printString6502 ## ADDRESSING_MODE; \
  dst->printName = OpcodeprintNameWithSpace; \
  dst->setData(dst, malloc(sizeof(Struct6502Single))); \
}

/* Abs */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(Abs);
/*void generate6502Abs(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config);
OpcodeSimilarity compare6502Abs(Opcode* obj, Opcode* other,
                                 DismSettings* config);
unsigned int readStep6502Abs(Opcode* obj, BufferStream* stream,
                              DismSettings* config);
void printString6502Abs(Opcode* obj, String* dst,
                         DismSettings* config); */

/* AbsX */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(AbsX);

/* AbsY */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(AbsY);

/* A */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(A);

/* Immed */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(Immed);

/* Implied */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(Implied);

/* IndX */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(IndX);

/* Indirect */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(Indirect);

/* IndY */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(IndY);
/*void generate6502IndY(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config);
OpcodeSimilarity compare6502IndY(Opcode* obj, Opcode* other,
                                 DismSettings* config);
unsigned int readStep6502IndY(Opcode* obj, BufferStream* stream,
                              DismSettings* config);
void printString6502IndY(Opcode* obj, String* dst,
                         DismSettings* config); */

/* Relative */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(Relative);

/* ZP */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(ZP);

/* ZPX */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(ZPX);

/* ZPY */
GENERATE_6502_OPCODE_FUNCTIONS_DECLARATION(ZPY);

/* Init functions */

void initModule6502(DismModule* obj);
DismModule* allocModule6502();
void freeModule6502(DismModule* obj);


#endif
