#include "modules/Data/ModuleData.h"
#include "util/ByteConv.h"
#include <stdlib.h>

OpInfo opcodesData[] = {
  { ".db", "xxxxxxxx", opFlagsNone, generateDataMOpcode }
/*  { "adc", "01110001xxxxxxxx", opFlagsNone, generate6502IndY } */
};

void generateDataMOpcode(OpInfo* opInfo, Opcode* dst,
                         DismSettings* config) {
  dst->compare = compareDataM;
  dst->readStep = readStepDataM;
  dst->printString = printStringDataM;
  dst->printName = OpcodeprintNameWithSpace;
  dst->setData(dst, malloc(sizeof(char)));
}

OpcodeSimilarity compareDataM(Opcode* obj, Opcode* other,
                              DismSettings* config) {
  char first = *((char*)(obj->data_));
  char second = *((char*)(other->data_));
  
  if (first == second) {
    return opcodeSimilarityNear;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}
                              
unsigned int readStepDataM(Opcode* obj, BufferStream* stream,
                           DismSettings* config,
                           MapSS* args) {
  *((char*)(obj->data_)) = *(stream->getcur(stream));
  return 1;
}
                           
void printStringDataM(Opcode* obj, String* dst,
                      DismSettings* config) {
  dst->catInt(dst, *((unsigned char*)(obj->data_)), "$%02X");
}

int areOpsSameModuleData(DismModule* obj, Opcode* opcodeA,
                              Opcode* opcodeB) {
  if (compareDataM(opcodeA, opcodeB, NULL) == opcodeSimilarityNear) {
    return 1;
  }
  
  return 0;
}

void initModuleData(DismModule* obj) {
  initDismModule(obj);
  
  /* Disable arg collation */
  obj->enableOpArgCollation_ = 0;
  
  obj->areOpsSame = areOpsSameModuleData;
  
  OpInfoArray ops = { opcodesData, sizeof(opcodesData) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModuleData() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModuleData(obj);
  return obj;
}

void freeModuleData(DismModule* obj) {
  free(obj);
}
