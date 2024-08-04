#include "modules/6502/Module6502.h"
#include "modules/65C02/Module65C02.h"
#include "util/ByteConv.h"
#include <stdlib.h>

OpInfo opcodes65C02[] = {
  { "bra", "10000000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "ora", "00010010xxxxxxxx", opFlagsNone, generate65C02ZPInd },
  { "and", "00110010xxxxxxxx", opFlagsNone, generate65C02ZPInd },
  { "eor", "01010010xxxxxxxx", opFlagsNone, generate65C02ZPInd },
  { "adc", "01110010xxxxxxxx", opFlagsNone, generate65C02ZPInd },
  { "sta", "10010010xxxxxxxx", opFlagsNone, generate65C02ZPInd },
  { "lda", "10110010xxxxxxxx", opFlagsNone, generate65C02ZPInd },
  { "cmp", "11010010xxxxxxxx", opFlagsNone, generate65C02ZPInd },
  { "sbc", "11110010xxxxxxxx", opFlagsNone, generate65C02ZPInd },
  { "tsb", "00000100xxxxxxxx", opFlagsNone, generate6502ZP },
  { "trb", "00010100xxxxxxxx", opFlagsNone, generate6502ZP },
  { "bit", "00110100xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "stz", "01100100xxxxxxxx", opFlagsNone, generate6502ZP },
  { "stz", "01110100xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "rmb0", "00000111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "rmb1", "00010111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "rmb2", "00100111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "rmb3", "00110111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "rmb4", "01000111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "rmb5", "01010111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "rmb6", "01100111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "rmb7", "01110111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "smb0", "10000111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "smb1", "10010111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "smb2", "10100111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "smb3", "10110111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "smb4", "11000111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "smb5", "11010111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "smb6", "11100111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "smb7", "11110111xxxxxxxx", opFlagsNone, generate6502ZP },
  { "bit", "10001001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "inc", "00011010", opFlagsNone, generate6502Implied },
  { "dec", "00111010", opFlagsNone, generate6502Implied },
  { "phy", "01011010", opFlagsNone, generate6502Implied },
  { "ply", "01111010", opFlagsNone, generate6502Implied },
  { "phx", "11011010", opFlagsNone, generate6502Implied },
  { "plx", "11111010", opFlagsNone, generate6502Implied },
  { "tsb", "00001100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "trb", "00011100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "bit", "00111100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "jmp", "01111100xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02AbsIndX },
  { "stz", "10011100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "stz", "10011110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "bbr0", "00001111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbr1", "00011111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbr2", "00101111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbr3", "00111111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbr4", "01001111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbr5", "01011111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbr6", "01101111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbr7", "01111111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbs0", "10001111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbs1", "10011111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbs2", "10101111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbs3", "10111111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbs4", "11001111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbs5", "11011111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbs6", "11101111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel },
  { "bbs7", "11111111xxxxxxxxxxxxxxxx", opFlagsNone, generate65C02ZPRel }
};



/* ==================== AbsIndX ==================== */

GENERATE_65C02_OPCODE_GENERATION_DEFINITION(AbsIndX);

OpcodeSimilarity compare65C02AbsIndX(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare2bAddress6502(obj, other, config);
}

unsigned int readStep65C02AbsIndX(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read2bArg6502(obj, stream, config);
}

void printString65C02AbsIndX(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catC(dst, "(");
  print2bAddress6502(dst, obj, config);
  dst->catC(dst, ",X)");
}

/* ==================== ZPInd ==================== */

/*GENERATE_65C02_OPCODE_GENERATION_DEFINITION(ZPInd);

OpcodeSimilarity compare65C02ZPInd(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare2bAddress65C02(obj, other, config);
}

unsigned int readStep65C02ZPInd(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read2bArg65C02(obj, stream, config);
}

void printString65C02ZPInd(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catC(dst, "(");
  print2bAddress65C02(dst, obj, config);
  dst->catC(dst, ")");
} */

GENERATE_65C02_OPCODE_GENERATION_DEFINITION(ZPInd);

OpcodeSimilarity compare65C02ZPInd(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare1bAddress6502(obj, other, config);
}

unsigned int readStep65C02ZPInd(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read1bArg6502(obj, stream, config);
}

void printString65C02ZPInd(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catC(dst, "(");
  print1bAddress6502(dst, obj, config);
  dst->catC(dst, ")");
}

/* ==================== ZPRel ==================== */
void generate65C02ZPRel(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compare65C02ZPRel;
  dst->readStep = readStep65C02ZPRel;
  dst->printString = printString65C02ZPRel;
  dst->printName = OpcodeprintNameWithSpace;
  dst->setData(dst, malloc(sizeof(Struct65C02Dual)));
}

OpcodeSimilarity compare65C02ZPRel(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  Struct65C02Dual* otherDat = (Struct65C02Dual*)(other->data_);
  
  OpcodeSimilarity addrSim
    = compareAddress6502(dat->value1, otherDat->value1, config);
  OpcodeSimilarity offsetSim
    = compareRelative6502(dat->value2, otherDat->value2, config);
  
  return greaterOpDifference(addrSim, offsetSim);
}

unsigned int readStep65C02ZPRel(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  dat->value1 = fromBytes(stream->getcur(stream) + 1,
                          1, littleEnd, nosign);
  dat->value2 = fromBytes(stream->getcur(stream) + 2,
                          1, littleEnd, sign);
  return 3;
}

void printString65C02ZPRel(Opcode* obj, String* dst,
                         DismSettings* config) {
  Struct65C02Dual* dat = (Struct65C02Dual*)(obj->data_);
  
  dst->catC(dst, "(");
  print1bAddress6502Raw(dst, dat->value1, config);
  dst->catC(dst, ",");
  print1bRelative6502Raw(dst, dat->value2, config,
                         obj->loadAddr(obj) + 3);
  dst->catC(dst, ")");
}



void initModule65C02(DismModule* obj) {
  initDismModule(obj);
  initModule6502(obj);
  
  OpInfoArray ops = { opcodes65C02, sizeof(opcodes65C02) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModule65C02() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModule65C02(obj);
  return obj;
}

void freeModule65C02(DismModule* obj) {
  free(obj);
}
