#ifndef CPD_MODULE_DATA_H
#define CPD_MODULE_DATA_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Generic data disassembly module.
 * This does simple byte-by-byte comparison and alignment, which is useful
 * when dealing with some types of raw data.
 */

/* Op list */

extern OpInfo opcodesData[];

void generateDataMOpcode(OpInfo* opInfo, Opcode* dst,
                         DismSettings* config);

OpcodeSimilarity compareDataM(Opcode* obj, Opcode* other,
                              DismSettings* config);
                              
unsigned int readStepDataM(Opcode* obj, BufferStream* stream,
                           DismSettings* config,
                           MapSS* args);
                           
void printStringDataM(Opcode* obj, String* dst,
                      DismSettings* config);

int areOpsSameModuleData(DismModule* obj, Opcode* opcodeA,
                              Opcode* opcodeB);

/* Init functions */

void initModuleData(DismModule* obj);
DismModule* allocModuleData();
void freeModuleData(DismModule* obj);


#endif
