#ifndef CPD_MODULE_SH2_H
#define CPD_MODULE_SH2_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Disassembly module for Hitachi SH-2.
 *
 * Code for this module is partly based on the public domain code for sh2d,
 * written by Bart Trzynadlowski with changes by Björn Stenberg.
 */

/* Enum of SH-2 addressing modes. */
typedef enum {
  
  addrModeSH2None,
  addrModeSH2Rn,
  addrModeSH2Rm,
  addrModeSH2AtRn,
  addrModeSH2AtRm,
  addrModeSH2AtRnInc,
  addrModeSH2AtRmInc,
  addrModeSH2AtDecRn,
  addrModeSH2AtDecRm,
  addrModeSH2AtDisp4Rn,     /* zero-extended */
  addrModeSH2AtDisp4Rm,     /* zero-extended */
  addrModeSH2AtR0Rn,
  addrModeSH2AtR0Rm,
  addrModeSH2AtDisp8GBR,    /* zero-extended */
  addrModeSH2AtR0GBR,       /* zero-extended */
  addrModeSH2AtDisp8PC,     /* zero-extended */
  addrModeSH2Disp8,         /* sign-extended */
  addrModeSH2Disp12,        /* sign-extended */
  addrModeSH2PCRn,
  addrModeSH2PCRm,
  addrModeSH2ImmedZ,        /* zero: tst, and, or, xor */
  addrModeSH2ImmedS,        /* sign: mov, add, cmp/eq */
  addrModeSH2ImmedZQ,       /* zero + 4x: trapa */
  addrModeSH2Unrecognized
  
} AddressModeSH2;

/* Supplementary data structure for SH-2 Opcodes */
typedef struct {

  MapSS* args;
  int address;
  AddressModeSH2 srcMode;
  AddressModeSH2 dstMode;

} SubDataSH2;

/* Supplementary data structure for SH-2 module */

typedef struct {

  /* CodeMap used to identify word/long constants */
  CodeMap codeMap;

} ModDataSH2;

ModDataSH2* allocModDataSH2();
void freeModDataSH2(ModDataSH2* obj);

/* Pseudo-opcodes for representing known words/longwords.
   These are not registered with the module. Instead, they're inserted
   automatically by tracking known data load addresses and inserting
   these into the Opcode stream where appropriate. */

extern OpInfo wordOpSH2Info;
extern OpInfo longOpSH2Info;

void WordOpSH2generateOpcode(OpInfo* opInfo, Opcode* dst,
                         DismSettings* config);
OpcodeSimilarity WordOpSH2compare(Opcode* obj, Opcode* other,
                         DismSettings* config);
unsigned int WordOpSH2readStep(Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args);
void WordOpSH2printString(Opcode* obj, String* dst,
                         DismSettings* config);

void LongOpSH2generateOpcode(OpInfo* opInfo, Opcode* dst,
                         DismSettings* config);
OpcodeSimilarity LongOpSH2compare(Opcode* obj, Opcode* other,
                         DismSettings* config);
unsigned int LongOpSH2readStep(Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args);
void LongOpSH2printString(Opcode* obj, String* dst,
                         DismSettings* config);

/* Op list */

extern OpInfo opcodesSH2[];

void destructorOpcodeSH2(Opcode* obj);

void ModuleSH2destroyInternal(DismModule* obj);

/* Module function overrides */

void ModuleSH2disassemble(DismModule* obj, DismStruct* dismStruct,
                           unsigned int start, unsigned int end);
  
int ModuleSH2tryOpRead(DismModule* obj, DismStruct* dismStruct,
                        OpInfoArray ops, unsigned int remaining);
                           
/* Override/hijack of printComparedDisassembly for getting a list
   of changed word/longword constants */
void ModuleSH2printComparedDisassemblyConsts(DismModule* obj, String* dst,
                                DismStruct* firstDismStruct,
                                DismStruct* secondDismStruct);
/* Same as above for immediate register changes */
void ModuleSH2printComparedDisassemblyImmeds(DismModule* obj, String* dst,
                                DismStruct* firstDismStruct,
                                DismStruct* secondDismStruct);

/* Init functions */

void initModuleSH2(DismModule* obj);
DismModule* allocModuleSH2();
void freeModuleSH2(DismModule* obj);


#endif
