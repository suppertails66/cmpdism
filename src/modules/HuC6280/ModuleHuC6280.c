#include "modules/6502/Module6502.h"
#include "modules/65C02/Module65C02.h"
#include "modules/HuC6280/ModuleHuC6280.h"
#include "util/ByteConv.h"
#include <stdlib.h>

OpInfo opcodesHuC6280[] = {
  { "sxy", "00000010", opFlagsNone, generate6502Implied },
  { "sax", "00100010", opFlagsNone, generate6502Implied },
  { "say", "01000010", opFlagsNone, generate6502Implied },
  { "cla", "01100010", opFlagsNone, generate6502Implied },
  { "clx", "10000010", opFlagsNone, generate6502Implied },
  { "cly", "11000010", opFlagsNone, generate6502Implied },
  { "st0", "00000011xxxxxxxx", opFlagsNone, generate6502Immed },
  { "st1", "00010011xxxxxxxx", opFlagsNone, generate6502Immed },
  { "st2", "00100011xxxxxxxx", opFlagsNone, generate6502Immed },
  { "tma", "01000011xxxxxxxx", opFlagsNone, generate6502Immed },
  { "tam", "01010011xxxxxxxx", opFlagsNone, generate6502Immed },
  { "tii", "01110011xxxxxxxxxxxxxxxxxxxxxxxx"
           "xxxxxxxxxxxxxxxxxxxxxxxx", opFlagsNone, generateHuC6280Blk },
  { "tst", "10000011xxxxxxxxxxxxxxxx", opFlagsNone, generateHuC6280ImZP },
  { "tst", "10010011xxxxxxxxxxxxxxxxxxxxxxxx", opFlagsNone,
      generateHuC6280ImAbs },
  { "tst", "10100011xxxxxxxxxxxxxxxx", opFlagsNone, generateHuC6280ImZPX },
  { "tst", "10110011xxxxxxxxxxxxxxxxxxxxxxxx", opFlagsNone,
      generateHuC6280ImAbsX },
  { "tdd", "11000011xxxxxxxxxxxxxxxxxxxxxxxx"
           "xxxxxxxxxxxxxxxxxxxxxxxx", opFlagsNone, generateHuC6280Blk },
  { "tin", "11010011xxxxxxxxxxxxxxxxxxxxxxxx"
           "xxxxxxxxxxxxxxxxxxxxxxxx", opFlagsNone, generateHuC6280Blk },
  { "tia", "11100011xxxxxxxxxxxxxxxxxxxxxxxx"
           "xxxxxxxxxxxxxxxxxxxxxxxx", opFlagsNone, generateHuC6280Blk },
  { "tai", "11110011xxxxxxxxxxxxxxxxxxxxxxxx"
           "xxxxxxxxxxxxxxxxxxxxxxxx", opFlagsNone, generateHuC6280Blk },
  { "bsr", "01000100xxxxxxxx", opFlagsNone, generate6502Relative },
  { "csl", "01010100", opFlagsNone, generate6502Implied },
  { "csh", "11010100", opFlagsNone, generate6502Implied },
  { "set", "11110100", opFlagsNone, generate6502Implied }
};



/* ==================== Blk ==================== */

void generateHuC6280Blk(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareHuC6280Blk;
  dst->readStep = readStepHuC6280Blk;
  dst->printString = printStringHuC6280Blk;
  dst->printName = OpcodeprintNameWithSpace;
  dst->setData(dst, malloc(sizeof(StructHuC6280Triple)));
}

OpcodeSimilarity compareHuC6280Blk(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  StructHuC6280Triple* dat = (StructHuC6280Triple*)(obj->data_);
  StructHuC6280Triple* otherDat = (StructHuC6280Triple*)(other->data_);
  
  OpcodeSimilarity addrSim1
    = compareAddress6502(dat->value1, otherDat->value1, config);
  OpcodeSimilarity addrSim2
    = compareAddress6502(dat->value2, otherDat->value2, config);
  OpcodeSimilarity constSim1
    = compareConstant6502(dat->value3, otherDat->value3, config);
  
  /* where is my variadic min??? */
  return greaterOpDifference(
          greaterOpDifference(addrSim1, addrSim2),
          constSim1);
}

unsigned int readStepHuC6280Blk(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  StructHuC6280Triple* dat = (StructHuC6280Triple*)(obj->data_);
  dat->value1 = fromBytes(stream->getcur(stream) + 1,
                          2, littleEnd, nosign);
  dat->value2 = fromBytes(stream->getcur(stream) + 3,
                          2, littleEnd, nosign);
  dat->value3 = fromBytes(stream->getcur(stream) + 5,
                          2, littleEnd, nosign);
  return 7;
}

void printStringHuC6280Blk(Opcode* obj, String* dst,
                         DismSettings* config) {
  StructHuC6280Triple* dat = (StructHuC6280Triple*)(obj->data_);
  print2bAddress6502Raw(dst, dat->value1, config);
  dst->catC(dst, ",");
  print2bAddress6502Raw(dst, dat->value2, config);
  dst->catC(dst, ",");
  print2bConstant6502Raw(dst, dat->value3, config);
}

/* ==================== ImAbs ==================== */

void generateHuC6280ImAbs(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareHuC6280ImAbs;
  dst->readStep = readStepHuC6280ImAbs;
  dst->printString = printStringHuC6280ImAbs;
  dst->printName = OpcodeprintNameWithSpace;
  dst->setData(dst, malloc(sizeof(Struct65C02Dual)));
}

OpcodeSimilarity compareHuC6280ImAbs(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  Struct65C02Dual* otherDat = (Struct65C02Dual*)(other->data_);
  
  OpcodeSimilarity constSim
    = compareConstant6502(dat->value1, otherDat->value1, config);
  OpcodeSimilarity addrSim
    = compareAddress6502(dat->value2, otherDat->value2, config);
  
  return greaterOpDifference(constSim, addrSim);
}

unsigned int readStepHuC6280ImAbs(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  dat->value1 = fromBytes(stream->getcur(stream) + 1,
                          1, littleEnd, nosign);
  dat->value2 = fromBytes(stream->getcur(stream) + 2,
                          2, littleEnd, nosign);
  return 4;
}

void printStringHuC6280ImAbs(Opcode* obj, String* dst,
                         DismSettings* config) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  print1bConstant6502Raw(dst, dat->value1, config);
  dst->catC(dst, ",");
  print2bAddress6502Raw(dst, dat->value2, config);
}

/* ==================== ImAbsX ==================== */

void generateHuC6280ImAbsX(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareHuC6280ImAbs;
  dst->readStep = readStepHuC6280ImAbs;
  dst->printString = printStringHuC6280ImAbsX;
  dst->printName = OpcodeprintNameWithSpace;
  dst->setData(dst, malloc(sizeof(Struct65C02Dual)));
}

void printStringHuC6280ImAbsX(Opcode* obj, String* dst,
                         DismSettings* config) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  print1bConstant6502Raw(dst, dat->value1, config);
  dst->catC(dst, ",");
  print2bAddress6502Raw(dst, dat->value2, config);
  dst->catC(dst, ",X");
}

/* ==================== ImZP ==================== */

void generateHuC6280ImZP(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareHuC6280ImZP;
  dst->readStep = readStepHuC6280ImZP;
  dst->printString = printStringHuC6280ImZP;
  dst->printName = OpcodeprintNameWithSpace;
  dst->setData(dst, malloc(sizeof(Struct65C02Dual)));
}

OpcodeSimilarity compareHuC6280ImZP(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  Struct65C02Dual* otherDat = (Struct65C02Dual*)(other->data_);
  
  OpcodeSimilarity constSim
    = compareConstant6502(dat->value1, otherDat->value1, config);
  OpcodeSimilarity addrSim
    = compareAddress6502(dat->value2, otherDat->value2, config);
  
  return greaterOpDifference(constSim, addrSim);
}

unsigned int readStepHuC6280ImZP(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  dat->value1 = fromBytes(stream->getcur(stream) + 1,
                          1, littleEnd, nosign);
  dat->value2 = fromBytes(stream->getcur(stream) + 2,
                          1, littleEnd, nosign);
  return 3;
}

void printStringHuC6280ImZP(Opcode* obj, String* dst,
                         DismSettings* config) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  print1bConstant6502Raw(dst, dat->value1, config);
  dst->catC(dst, ",");
  print1bAddress6502Raw(dst, dat->value2, config);
}

/* ==================== ImZPX ==================== */

void generateHuC6280ImZPX(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareHuC6280ImZP;
  dst->readStep = readStepHuC6280ImZP;
  dst->printString = printStringHuC6280ImZPX;
  dst->printName = OpcodeprintNameWithSpace;
  dst->setData(dst, malloc(sizeof(Struct65C02Dual)));
}

void printStringHuC6280ImZPX(Opcode* obj, String* dst,
                         DismSettings* config) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  print1bConstant6502Raw(dst, dat->value1, config);
  dst->catC(dst, ",");
  print1bAddress6502Raw(dst, dat->value2, config);
  dst->catC(dst, ",X");
}



void initModuleHuC6280(DismModule* obj) {
  initDismModule(obj);
  initModule6502(obj);
  initModule65C02(obj);
  
  OpInfoArray ops
    = { opcodesHuC6280, sizeof(opcodesHuC6280) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModuleHuC6280() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModuleHuC6280(obj);
  return obj;
}

void freeModuleHuC6280(DismModule* obj) {
  free(obj);
}
