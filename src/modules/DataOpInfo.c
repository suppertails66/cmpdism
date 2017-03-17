#include "modules/DataOpInfo.h"
#include "util/ByteConv.h"
#include <stdlib.h>
#include <stdio.h>

OpInfo dataOpInfo = { ".db", "", opFlagsNone, DataOpgenerateOpcode,
                      DATA_OP_ID };

void DataOpgenerateOpcode(OpInfo* opInfo, Opcode* dst,
                         DismSettings* config) {
  dst->compare = DataOpcompare;
  dst->readStep = DataOpreadStep;
  dst->printString = DataOpprintString;
  dst->printName = OpcodeprintNameWithSpace;
/*  dst->info_ = &dataOpInfo; */
}

OpcodeSimilarity DataOpcompare(Opcode* obj, Opcode* other,
                         DismSettings* config) {
  char thisByte = *((char*)(obj->data_));
  char otherByte = *((char*)(other->data_));
  
  if (thisByte == otherByte) return opcodeSimilaritySame;
  
  return opcodeSimilarityNone;
}

unsigned int DataOpreadStep(Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args) {
  free(args);
  char* c = malloc(sizeof(char));
  *c = *(stream->getcur(stream));
  obj->data_ = (void*)c;
  return 1;
}

void DataOpprintString(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catInt(dst, *((unsigned char*)(obj->data_)), "$%02X");
}
