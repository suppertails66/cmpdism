#ifndef CPD_DATA_OP_INFO_H
#define CPD_DATA_OP_INFO_H


#include "modules/OpInfo.h"
#include "modules/Opcode.h"

#define DATA_OP_ID -1

extern OpInfo dataOpInfo;

void DataOpgenerateOpcode(OpInfo* opInfo, Opcode* dst,
                         DismSettings* config);
OpcodeSimilarity DataOpcompare(Opcode* obj, Opcode* other,
                         DismSettings* config);
unsigned int DataOpreadStep(Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args);
void DataOpprintString(Opcode* obj, String* dst,
                         DismSettings* config);


#endif
