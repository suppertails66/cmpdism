#include "modules/SH2/ModuleSH2.h"
#include "modules/StringMatcher.h"
#include "modules/OpArgCollator.h"
#include "modules/Consts.h"
#include "util/Logger.h"
#include "util/ByteConv.h"
#include <stdlib.h>
#include <string.h>

/*typedef enum OpTargetSH2 {
  
  opTargetSH2Constant,
  opTargetSH2Data3,
  opTargetSH2Data8,
  opTargetSH2CCC,
  opTargetSH2Data16As,
  opTargetSH2Dn,
  opTargetSH2Data3,
  
  
} OpTargetSH2; */

#define codeMapLongMarker 0xFF
#define codeMapWordMarker 0xFE
#define DATA_WORD_ID -2
#define DATA_LONG_ID -3

/* Forward declaration needed for opcodesSH2 array */
void generateOpcodeSH2(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config);

OpInfo opcodesSH2[] = {
  { "clrt", "0000000000001000",
    opFlagsNone, generateOpcodeSH2, 0, "0." },
  { "clrmac", "0000000000101000",
    opFlagsNone, generateOpcodeSH2, 0, "0." },
  { "div1u", "0000000000011001",
    opFlagsNone, generateOpcodeSH2, 0, "0." },
  { "nop", "0000000000001001",
    opFlagsNone, generateOpcodeSH2, 0, "0." },
  { "rte", "0000000000101011",
    opFlagsNone, generateOpcodeSH2, 0, "0." },
  { "rts", "0000000000001011",
    opFlagsNone, generateOpcodeSH2, 0, "0." },
  { "sett", "0000000000011000",
    opFlagsNone, generateOpcodeSH2, 0, "0." },
  { "sleep", "0000000000011011",
    opFlagsNone, generateOpcodeSH2, 0, "0." },
  { "cmp/pl", "0100nnnn00010101",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "cmp/pz", "0100nnnn00010001",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "dt", "0100nnnn00010000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "movt", "0000nnnn00101001",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "rotl", "0100nnnn00000100",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "rotr", "0100nnnn00000101",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "rotcl", "0100nnnn00100100",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "rotcr", "0100nnnn00100101",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shal", "0100nnnn00100000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shar", "0100nnnn00100001",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shll", "0100nnnn00000000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shlr", "0100nnnn00000001",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shll2", "0100nnnn00001000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shlr2", "0100nnnn00001001",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shll8", "0100nnnn00011000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shlr8", "0100nnnn00011001",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shll06", "0100nnnn00101000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "shlr06", "0100nnnn00101001",
    opFlagsNone, generateOpcodeSH2, 0, "0.rn" },
  { "stc", "0000nnnn00000010",
    opFlagsNone, generateOpcodeSH2, 0, "0.sr,rn" },
  { "stc", "0000nnnn00010010",
    opFlagsNone, generateOpcodeSH2, 0, "0.gbr,rn" },
  { "stc", "0000nnnn00100010",
    opFlagsNone, generateOpcodeSH2, 0, "0.vbr,rn" },
  { "sts", "0000nnnn00001010",
    opFlagsNone, generateOpcodeSH2, 0, "0.mach,rn" },
  { "sts", "0000nnnn00011010",
    opFlagsNone, generateOpcodeSH2, 0, "0.macl,rn" },
  { "sts", "0000nnnn00101010",
    opFlagsNone, generateOpcodeSH2, 0, "0.pr,rn" },
  { "tas.b", "0100nnnn00011011",
    opFlagsNone, generateOpcodeSH2, 0, "1.@rn" },
  { "stc.l", "0100nnnn00000011",
    opFlagsNone, generateOpcodeSH2, 0, "4.sr,@-rn" },
  { "stc.l", "0100nnnn00010011",
    opFlagsNone, generateOpcodeSH2, 0, "4.gbr,@-rn" },
  { "stc.l", "0100nnnn00100011",
    opFlagsNone, generateOpcodeSH2, 0, "4.vbr,@-rn" },
  { "sts.l", "0100nnnn00000010",
    opFlagsNone, generateOpcodeSH2, 0, "4.mach,@-rn" },
  { "sts.l", "0100nnnn00010010",
    opFlagsNone, generateOpcodeSH2, 0, "4.macl,@-rn" },
  { "sts.l", "0100nnnn00100010",
    opFlagsNone, generateOpcodeSH2, 0, "4.pr,@-rn" },
  { "ldc", "0100mmmm00001110",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,sr" },
  { "ldc", "0100mmmm00011110",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,gbr" },
  { "ldc", "0100mmmm00101110",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,vbr" },
  { "lds", "0100mmmm00001010",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,mach" },
  { "lds", "0100mmmm00011010",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,macl" },
  { "lds", "0100mmmm00101010",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,pr" },
  { "jmp", "0100mmmm00101011",
    opFlagsNone, generateOpcodeSH2, 0, "0.@rm" },
  { "jsr", "0100mmmm00001011",
    opFlagsNone, generateOpcodeSH2, 0, "0.@rm" },
  { "ldc.l", "0100mmmm00000111",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm+,sr" },
  { "ldc.l", "0100mmmm00010111",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm+,gbr" },
  { "ldc.l", "0100mmmm00100111",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm+,vbr" },
  { "lds.l", "0100mmmm00000110",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm+,mach" },
  { "lds.l", "0100mmmm00010110",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm+,macl" },
  { "lds.l", "0100mmmm00100110",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm+,pr" },
  { "braf", "0000mmmm00100011",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm" },
  { "bsrf", "0000mmmm00000011",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm" },
  { "add", "0011nnnnmmmm1100",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "addc", "0011nnnnmmmm1110",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "addv", "0011nnnnmmmm1111",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "and", "0010nnnnmmmm1001",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "cmp/eq", "0011nnnnmmmm0000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "cmp/hs", "0011nnnnmmmm0010",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "cmp/ge", "0011nnnnmmmm0011",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "cmp/hi", "0011nnnnmmmm0110",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "cmp/gt", "0011nnnnmmmm0111",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "cmp/str", "0010nnnnmmmm1100",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "div0", "0011nnnnmmmm0100",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "div1s", "0010nnnnmmmm0111",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "dmuls.l", "0011nnnnmmmm1101",
    opFlagsNone, generateOpcodeSH2, 0, "4.rm,rn" },
  { "dmulu.l", "0011nnnnmmmm0101",
    opFlagsNone, generateOpcodeSH2, 0, "4.rm,rn" },
  { "exts.b", "0110nnnnmmmm1110",
    opFlagsNone, generateOpcodeSH2, 0, "1.rm,rn" },
  { "exts.w", "0110nnnnmmmm1111",
    opFlagsNone, generateOpcodeSH2, 0, "2.rm,rn" },
  { "extu.b", "0110nnnnmmmm1100",
    opFlagsNone, generateOpcodeSH2, 0, "1.rm,rn" },
  { "extu.w", "0110nnnnmmmm1101",
    opFlagsNone, generateOpcodeSH2, 0, "2.rm,rn" },
  { "mov", "0110nnnnmmmm0011",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "mul.l", "0000nnnnmmmm0111",
    opFlagsNone, generateOpcodeSH2, 0, "4.rm,rn" },
  { "muls.w", "0010nnnnmmmm1111",
    opFlagsNone, generateOpcodeSH2, 0, "2.rm,rn" },
  { "mulu.w", "0010nnnnmmmm1110",
    opFlagsNone, generateOpcodeSH2, 0, "2.rm,rn" },
  { "neg", "0110nnnnmmmm1011",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "negc", "0110nnnnmmmm1010",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "not", "0110nnnnmmmm0111",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "or", "0010nnnnmmmm1011",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "sub", "0011nnnnmmmm1000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "subc", "0011nnnnmmmm1010",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "subv", "0011nnnnmmmm1011",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "swap.b", "0110nnnnmmmm1000",
    opFlagsNone, generateOpcodeSH2, 0, "1.rm,rn" },
  { "swap.w", "0110nnnnmmmm1001",
    opFlagsNone, generateOpcodeSH2, 0, "2.rm,rn" },
  { "tst", "0010nnnnmmmm1000",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "xor", "0010nnnnmmmm1010",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "xtrct", "0010nnnnmmmm1101",
    opFlagsNone, generateOpcodeSH2, 0, "0.rm,rn" },
  { "mov.b", "0010nnnnmmmm0000",
    opFlagsNone, generateOpcodeSH2, 0, "1.rm,@rn" },
  { "mov.w", "0010nnnnmmmm0001",
    opFlagsNone, generateOpcodeSH2, 0, "2.rm,@rn" },
  { "mov.l", "0010nnnnmmmm0010",
    opFlagsNone, generateOpcodeSH2, 0, "4.rm,@rn" },
  { "mov.b", "0110nnnnmmmm0000",
    opFlagsNone, generateOpcodeSH2, 0, "1.@rm,rn" },
  { "mov.w", "0110nnnnmmmm0001",
    opFlagsNone, generateOpcodeSH2, 0, "2.@rm,rn" },
  { "mov.l", "0110nnnnmmmm0010",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm,rn" },
  { "mac.l", "0000nnnnmmmm1111",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm+,@rn+" },
  { "mac.w", "0100nnnnmmmm1111",
    opFlagsNone, generateOpcodeSH2, 0, "2.@rm+,@rn+" },
  { "mov.b", "0110nnnnmmmm0100",
    opFlagsNone, generateOpcodeSH2, 0, "1.@rm+,rn" },
  { "mov.w", "0110nnnnmmmm0101",
    opFlagsNone, generateOpcodeSH2, 0, "2.@rm+,rn" },
  { "mov.l", "0110nnnnmmmm0110",
    opFlagsNone, generateOpcodeSH2, 0, "4.@rm+,rn" },
  { "mov.b", "0010nnnnmmmm0100",
    opFlagsNone, generateOpcodeSH2, 0, "1.rm,@-rn" },
  { "mov.w", "0010nnnnmmmm0101",
    opFlagsNone, generateOpcodeSH2, 0, "2.rm,@-rn" },
  { "mov.l", "0010nnnnmmmm0110",
    opFlagsNone, generateOpcodeSH2, 0, "4.rm,@-rn" },
  { "mov.b", "0000nnnnmmmm0100",
    opFlagsNone, generateOpcodeSH2, 0, "1.rm,@(r0|rn)" },
  { "mov.w", "0000nnnnmmmm0101",
    opFlagsNone, generateOpcodeSH2, 0, "2.rm,@(r0|rn)" },
  { "mov.l", "0000nnnnmmmm0110",
    opFlagsNone, generateOpcodeSH2, 0, "4.rm,@(r0|rn)" },
  { "mov.b", "0000nnnnmmmm1100",
    opFlagsNone, generateOpcodeSH2, 0, "1.@(r0|rm),rn" },
  { "mov.w", "0000nnnnmmmm1101",
    opFlagsNone, generateOpcodeSH2, 0, "2.@(r0|rm),rn" },
  { "mov.l", "0000nnnnmmmm1110",
    opFlagsNone, generateOpcodeSH2, 0, "4.@(r0|rm),rn" },
  { "mov.b", "10000100mmmmdddd",
    opFlagsNone, generateOpcodeSH2, 0, "1.@(d4|rm), r0" },
  { "mov.w", "10000101mmmmdddd",
    opFlagsNone, generateOpcodeSH2, 0, "2.@(d4|rm), r0" },
  { "mov.b", "10000000nnnndddd",
    opFlagsNone, generateOpcodeSH2, 0, "1.r0,@(d4|rn)" },
  { "mov.w", "10000001nnnndddd",
    opFlagsNone, generateOpcodeSH2, 0, "2.r0,@(d4|rn)" },
  { "mov.l", "0001nnnnmmmmdddd",
    opFlagsNone, generateOpcodeSH2, 0, "4.rm,@(d4|rn)" },
  { "mov.l", "0101nnnnmmmmdddd",
    opFlagsNone, generateOpcodeSH2, 0, "4.@(d4|rm),rn" },
  { "mov.b", "11000000dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "1.r0,@(d8|gbr)" },
  { "mov.w", "11000001dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "2.r0,@(d8|gbr)" },
  { "mov.l", "11000010dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "4.r0,@(d8|gbr)" },
  { "mov.b", "11000100dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "1.@(d8|gbr),r0" },
  { "mov.w", "11000101dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "2.@(d8|gbr),r0" },
  { "mov.l", "11000110dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "4.@(d8|gbr),r0" },
  { "mova", "11000111dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "4.@(d8|pc),r0" },
  { "bf", "10001011dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "0.d8" },
  { "bf/s", "10001111dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "0.d8" },
  { "bt", "10001001dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "0.d8" },
  { "bt/s", "10001101dddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "0.d8" },
  { "bra", "1010dddddddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "0.d12" },
  { "bsr", "1011dddddddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "0.d12" },
  { "mov.w", "1001nnnndddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "2.@(d8|pc),rn" },
  { "mov.l", "1101nnnndddddddd",
    opFlagsNone, generateOpcodeSH2, 0, "4.@(d8|pc),rn" },
  { "and.b", "11001101iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "1.iz,@(r0|gbr)" },
  { "or.b", "11001111iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "1.iz,@(r0|gbr)" },
  { "tst.b", "11001100iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "1.iz,@(r0|gbr)" },
  { "xor.b", "11001110iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "1.iz,@(r0|gbr)" },
  { "and", "11001001iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "0.iz,r0" },
  { "cmp/eq", "10001000iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "0.is,r0" },
  { "or", "11001011iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "0.iz,r0" },
  { "tst", "11001000iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "0.iz,r0" },
  { "xor", "11001010iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "0.iz,r0" },
  { "trapa", "11000011iiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "0.izq" },
  { "add", "0111nnnniiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "0.is,rn" },
  { "mov", "1110nnnniiiiiiii",
    opFlagsNone, generateOpcodeSH2, 0, "0.is,rn" }
};

OpInfo wordOpSH2Info = { ".dw", "", opFlagsNone, WordOpSH2generateOpcode,
                      DATA_WORD_ID };
OpInfo longOpSH2Info = { ".dl", "", opFlagsNone, LongOpSH2generateOpcode,
                      DATA_LONG_ID };

void WordOpSH2generateOpcode(OpInfo* opInfo, Opcode* dst,
                         DismSettings* config) {
  dst->compare = WordOpSH2compare;
  dst->readStep = WordOpSH2readStep;
  dst->printString = WordOpSH2printString;
  dst->printName = OpcodeprintNameWithSpace;
/*  dst->info_ = &dataOpInfo; */
}

OpcodeSimilarity WordOpSH2compare(Opcode* obj, Opcode* other,
                         DismSettings* config) {
  int thisByte = *((int*)(obj->data_));
  int otherByte = *((int*)(other->data_));
  
  if (thisByte == otherByte) return opcodeSimilaritySame;
  
  return opcodeSimilarityDistinct;
}

unsigned int WordOpSH2readStep(Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args) {
  free(args);
  int* c = malloc(sizeof(int));
  
  *c = fromBytes((stream->getcur(stream)), 2, bigEnd, nosign);
  obj->data_ = (void*)c;
  
  return 2;
}

void WordOpSH2printString(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catInt(dst, *((unsigned int*)(obj->data_)), "$%04X");
}

void LongOpSH2generateOpcode(OpInfo* opInfo, Opcode* dst,
                         DismSettings* config) {
  dst->compare = LongOpSH2compare;
  dst->readStep = LongOpSH2readStep;
  dst->printString = LongOpSH2printString;
  dst->printName = OpcodeprintNameWithSpace;
/*  dst->info_ = &dataOpInfo; */
}

OpcodeSimilarity LongOpSH2compare(Opcode* obj, Opcode* other,
                         DismSettings* config) {
  int thisByte = *((int*)(obj->data_));
  int otherByte = *((int*)(other->data_));
  
  if (thisByte == otherByte) return opcodeSimilaritySame;
  
  return opcodeSimilarityDistinct;
}

unsigned int LongOpSH2readStep(Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args) {
  free(args);
  int* c = malloc(sizeof(int));
  
  *c = fromBytes((stream->getcur(stream)), 4, bigEnd, nosign);
  obj->data_ = (void*)c;
  
  return 4;
}

void LongOpSH2printString(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catInt(dst, *((unsigned int*)(obj->data_)), "$%08X");
}

const char* addressModeSH2Names[] = {
  "",
  "rn",
  "rm",
  "@rn",
  "@rm",
  "@rn+",
  "@rm+",
  "@-rn",
  "@-rm",
  "@(d4|rn)",
  "@(d4|rm)",
  "@(r0|rn)",
  "@(r0|rm)",
  "@(d8|gbr)",
  "@(r0|gbr)",
  "@(d8|pc)",
  "d8",
  "d12",
  "?n",
  "?m",
  "iz",
  "is",
  "izq",
  "XXXXX"
};

const int addressModeSH2NamesSize
  = (sizeof(addressModeSH2Names) / sizeof(char*)) - 1;

AddressModeSH2 getAddressModeSH2FromName(const char* name) {
  int i;
  for (i = 0; i < addressModeSH2NamesSize; i++) {
    if (strcmp(addressModeSH2Names[i], name) == 0) return (AddressModeSH2)i;
  }
  
  return addrModeSH2Unrecognized;
}

int findComma(const char* str, int len) {
  int i;
  for (i = 0; i < len; i++) if (str[i] == ',') return i;
  
  return -1;
}

int pcDispToAddr(int value, int pos, int wsize) {
  if (wsize == 2) {
    return pos + (value  * wsize) + 4;
  }
  else {
    /* Mask low 2 bits of PC */
    return (pos & 0xFFFFFFFC) + (value  * wsize) + 4;
  }
}

/* SH-2 Opcode readStep override */
unsigned int readStepOpcodeSH2(struct Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args) {
  
/*  unsigned int streamBase = stream->tell(stream); */
  unsigned int total = strlen(obj->info_->recString) / k_bitsPerByte;
  
  SubDataSH2* dat = (SubDataSH2*)(obj->data_);
  dat->args = args;
  dat->address = 0;
  dat->srcMode = addrModeSH2Unrecognized;
  dat->dstMode = addrModeSH2Unrecognized;
  
  /* Determine if this is an instruction that loads data displaced from
     the PC, and mark it in the codemap if so.
     
     This does a bunch of redundant work that's repeated in the print code.
     This is obviously not ideal, but it's good enough for now. */
  
  const char* argString = (const char*)(obj->info(obj)->data);
  
  /* retrieve the byte size of the operation */
  char wsizeStr[2];
  memset(wsizeStr, 0, 2);
  wsizeStr[0] = argString[0];
  int wsize;
  sscanf(wsizeStr, "%d", &wsize);
  
  /* skip the byte size in the string */
  argString += 2;
  
  /* determine number of arguments */
  
  int len = strlen(argString);
  if (len == 0) {
    /* no arguments */
    
  }
  else {
    int commaPos = findComma(argString, len);
    
    AddressModeSH2 srcMode = addrModeSH2Unrecognized;
    AddressModeSH2 dstMode = addrModeSH2Unrecognized; 
    
    if (commaPos == -1) {
      /* one argument */
      srcMode = getAddressModeSH2FromName(argString);
    }
    else {
      /* two arguments */
      String argA;
      String argB; 
      initString(&argA);
      initString(&argB); 
      
      argA.catData(&argA, argString, commaPos);
      argB.catData(&argB, argString + commaPos + 1, len - commaPos - 1); 
      
      srcMode = getAddressModeSH2FromName(argA.cStr(&argA));
      dstMode = getAddressModeSH2FromName(argB.cStr(&argB)); 
      
      argA.destroy(&argA);
      argB.destroy(&argB);
    }
    
    /* Remember argument types -- this will save us having to recompute
       them during comparison */
    dat->srcMode = srcMode;
    dat->dstMode = dstMode;
    
    switch (srcMode) {
    case addrModeSH2AtDisp8PC:
      {
        ModDataSH2* modData = (ModDataSH2*)(config->module->data_);
        CodeMap* codeMap = &(modData->codeMap);
        
        /* Get the raw displacement value */
        int rawDisp = getMapSSValueAsBinaryUint(args, "d");
        
        /* Convert to address */
        int addr = pcDispToAddr(rawDisp, stream->pos(stream), wsize);
        
        /* Verify that address is within valid data range */
        if ((addr + wsize) >= stream->size(stream)) break;
        
        /* Set codemap according to operation size */
        switch (wsize) {
        case 2:
          codeMap->set(codeMap, addr, codeMapWordMarker);
          break;
        case 4:
          codeMap->set(codeMap, addr, codeMapLongMarker);
          break;
        default:
          error("Illegal operation size for AtDisp8PC");
          fatal();
          break;
        }
      }
      break;
    default:
      /* nothing we care about */
      break;
    }
  }
  
  
  
  
  
  return total;
}


void printIndexedRegisterSH2(int value, String* dst, DismSettings* config) {
  dst->catC(dst, "r");
  dst->catInt(dst, value, "%d");
}

void printDisp4SH2(unsigned int value, int pos, int wsize, String* dst,
                             DismSettings* config) {
  dst->catInt(dst, value * wsize, "%02X");
}

void printDisp8SH2(unsigned int value, int pos, int wsize, String* dst,
                             DismSettings* config) {
  dst->catInt(dst, value * wsize, "%03X");
}

void printSignedDisp8SH2(int value, int pos, int wsize, String* dst,
                         DismSettings* config) {
  if (value > 0) {
    dst->catInt(dst, value * wsize, "+%03X");
  }
  else {
    dst->catInt(dst, -value * wsize, "-%03X");
  }
}

void printSignedDisp12SH2(int value, int pos, int wsize, String* dst,
                         DismSettings* config) {
  if (value > 0) {
    dst->catInt(dst, value * wsize, "+%04X");
  }
  else {
    dst->catInt(dst, -value * wsize, "-%04X");
  }
}


void printOpcodeArgSH2(Opcode* obj, String* dst, DismSettings* config,
                       const char* name, int wsize, MapSS* args) {
  AddressModeSH2 addressMode = getAddressModeSH2FromName(name);
  
  switch (addressMode) {
  case addrModeSH2None:
    /* shouldn't happen? */
    break;
  case addrModeSH2Rn:
    printIndexedRegisterSH2(getMapSSValueAsBinaryUint(args, "n"), dst, config);
    break;
  case addrModeSH2Rm:
    printIndexedRegisterSH2(getMapSSValueAsBinaryUint(args, "m"), dst, config);
    break;
  case addrModeSH2AtRn:
    dst->catC(dst, "@");
    printIndexedRegisterSH2(getMapSSValueAsBinaryUint(args, "n"), dst, config);
    break;
  case addrModeSH2AtRm:
    dst->catC(dst, "@");
    printIndexedRegisterSH2(getMapSSValueAsBinaryUint(args, "m"), dst, config);
    break;
  case addrModeSH2AtRnInc:
    dst->catC(dst, "@");
    printIndexedRegisterSH2(getMapSSValueAsBinaryUint(args, "n"), dst, config);
    dst->catC(dst, "+");
    break;
  case addrModeSH2AtRmInc:
    dst->catC(dst, "@");
    printIndexedRegisterSH2(getMapSSValueAsBinaryUint(args, "m"), dst, config);
    dst->catC(dst, "+");
    break;
  case addrModeSH2AtDecRn:
    dst->catC(dst, "@-");
    printIndexedRegisterSH2(getMapSSValueAsBinaryUint(args, "n"), dst, config);
    break;
  case addrModeSH2AtDecRm:
    dst->catC(dst, "@-");
    printIndexedRegisterSH2(getMapSSValueAsBinaryUint(args, "m"), dst, config);
    break;
  case addrModeSH2AtDisp4Rn:
  case addrModeSH2AtDisp4Rm:
    dst->catC(dst, "@(");
    /* zero-extended = uint */
    printDisp4SH2(getMapSSValueAsBinaryUint(args, "d"), obj->pos(obj), wsize,
                  dst, config);
    dst->catC(dst, ",");
    if (addressMode == addrModeSH2AtDisp4Rn) {
      printIndexedRegisterSH2(
          getMapSSValueAsBinaryUint(args, "n"), dst, config);
    }
    else {
      printIndexedRegisterSH2(
          getMapSSValueAsBinaryUint(args, "m"), dst, config);
    }
    dst->catC(dst, ")");
    break;
  case addrModeSH2AtR0Rn:
  case addrModeSH2AtR0Rm:
    dst->catC(dst, "@(r0,");
    if (addressMode == addrModeSH2AtR0Rn) {
      printIndexedRegisterSH2(
          getMapSSValueAsBinaryUint(args, "n"), dst, config);
    }
    else {
      printIndexedRegisterSH2(
          getMapSSValueAsBinaryUint(args, "m"), dst, config);
    }
    dst->catC(dst, ")");
    break;
  case addrModeSH2AtDisp8GBR:
    dst->catC(dst, "@(");
    /* zero-extended = uint */
    printDisp8SH2(getMapSSValueAsBinaryUint(args, "d"), obj->pos(obj),
                  wsize, dst, config);
    dst->catC(dst, ",GBR");
    break;
  case addrModeSH2AtR0GBR:
    dst->catC(dst, "@(r0,GBR)");
    break;
  case addrModeSH2AtDisp8PC:
    dst->catC(dst, "@(");
    /* zero-extended = uint */
    printDisp8SH2(getMapSSValueAsBinaryUint(args, "d"), obj->pos(obj),
                  wsize, dst, config);
    dst->catC(dst, ",PC)");
    break;
  case addrModeSH2Disp8:
    /* sign-extended = sint */
    /* the "word size" is 2 here because the displacement is always doubled */
    printSignedDisp8SH2(getMapSSValueAsBinaryInt(args, "d"), obj->pos(obj),
                        2, dst, config);
    break;
  case addrModeSH2Disp12:
    /* sign-extended = sint */
    /* the "word size" is 2 here because the displacement is always doubled */
    printSignedDisp12SH2(getMapSSValueAsBinaryInt(args, "d"), obj->pos(obj),
                         2, dst, config);
    break;
  case addrModeSH2PCRn:
    /* ? */
    printIndexedRegisterSH2(
        getMapSSValueAsBinaryUint(args, "n"), dst, config);
    break;
  case addrModeSH2PCRm:
    /* ? */
    printIndexedRegisterSH2(
        getMapSSValueAsBinaryUint(args, "m"), dst, config);
    break;
  case addrModeSH2ImmedZ:
    dst->catC(dst, "#");
    /* zero-extended = uint */
    dst->catInt(dst, getMapSSValueAsBinaryUint(args, "i"), "%02X");
    break;
  case addrModeSH2ImmedS:
    dst->catC(dst, "#");
    /* sign-extended = sint */
    dst->catInt(dst, getMapSSValueAsBinaryInt(args, "i"), "%02X");
    break;
  case addrModeSH2ImmedZQ:
    dst->catC(dst, "#");
    /* zero-extended + quadrupled = uint */
    dst->catInt(dst, getMapSSValueAsBinaryUint(args, "i") * 4, "%03X");
    break;
  case addrModeSH2Unrecognized:
    /* Assume this is some register (e.g. GBR) and print as-is */
    dst->catC(dst, name);
    break;
  default:
    error("Invalid SH-2 address mode");
    fatal();
    break;
  }
}

const int wsizeOffset = 2;

/* SH-2 Opcode printing override */
void printOpcodeSH2(Opcode* obj, String* dst, DismSettings* config) {
  
  const char* argString = (const char*)(obj->info(obj)->data);
  
  SubDataSH2* dat = (SubDataSH2*)(obj->data_);
  MapSS* args = dat->args;
  
  /* retrieve the byte size of the operation */
  char wsizeStr[2];
  memset(wsizeStr, 0, 2);
  wsizeStr[0] = argString[0];
  int wsize;
  sscanf(wsizeStr, "%d", &wsize);
  
  /* skip the byte size in the string */
  argString += 2;
  
  /* determine number of arguments */
  
  int len = strlen(argString);
  if (len == 0) {
    /* no arguments */
    return;
  }
  
  int commaPos = findComma(argString, len);
  if (commaPos == -1) {
    /* one argument */
    printOpcodeArgSH2(obj, dst, config,
        argString, wsize, args);
    return;
  }
  
  /* two arguments */
  
  String argA;
  String argB;
  initString(&argA);
  initString(&argB);
  
  argA.catData(&argA, argString, commaPos);
  argB.catData(&argB, argString + commaPos + 1, len - commaPos - 1);
  
  printOpcodeArgSH2(obj, dst, config,
      argA.cStr(&argA), wsize, args);
  dst->catC(dst, ",");
  printOpcodeArgSH2(obj, dst, config,
      argB.cStr(&argB), wsize, args);
  
  argA.destroy(&argA);
  argB.destroy(&argB);
  
}

OpcodeSimilarity compareDisplacementSH2(int first, int second,
                                        DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else if (config->addressChangesDistinct) {
    return opcodeSimilarityDistinct;
  }
  else {
    return opcodeSimilarityNear;
  }
}

OpcodeSimilarity compareRegisterIndexSH2(int first, int second,
                                         DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

OpcodeSimilarity compareConstantSH2(int first, int second,
                                         DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else if (config->constantChangesDistinct) {
    return opcodeSimilarityDistinct;
  }
  else {
    return opcodeSimilarityNear;
  }
}

OpcodeSimilarity compareOpcodeByModeSH2(MapSS* argsA, MapSS* argsB,
                         AddressModeSH2 mode, DismSettings* config) {
  
  /* Note that int/uint distinctions aren't important here -- we merely
     test for equality */
  
  switch (mode) {
  case addrModeSH2None:
  case addrModeSH2AtR0GBR:
  case addrModeSH2Unrecognized:
    return opcodeSimilaritySame;
    break;
  case addrModeSH2Rn:
  case addrModeSH2AtRn:
  case addrModeSH2AtRnInc:
  case addrModeSH2AtDecRn:
  case addrModeSH2AtR0Rn:
  case addrModeSH2PCRn:
    return compareRegisterIndexSH2(getMapSSValueAsBinaryUint(argsA, "n"),
                                   getMapSSValueAsBinaryUint(argsB, "n"),
                                   config);
    break;
  case addrModeSH2Rm:
  case addrModeSH2AtRm:
  case addrModeSH2AtRmInc:
  case addrModeSH2AtDecRm:
  case addrModeSH2AtR0Rm:
  case addrModeSH2PCRm:
    return compareRegisterIndexSH2(getMapSSValueAsBinaryUint(argsA, "m"),
                                   getMapSSValueAsBinaryUint(argsB, "m"),
                                   config);
    break;
  case addrModeSH2AtDisp4Rn:
    return greaterOpDifference(
      compareRegisterIndexSH2(getMapSSValueAsBinaryUint(argsA, "n"),
                              getMapSSValueAsBinaryUint(argsB, "n"),
                              config),
      compareDisplacementSH2(getMapSSValueAsBinaryUint(argsA, "d"),
                             getMapSSValueAsBinaryUint(argsB, "d"),
                             config)
    );
    break;
  case addrModeSH2AtDisp4Rm:
    return greaterOpDifference(
      compareRegisterIndexSH2(getMapSSValueAsBinaryUint(argsA, "m"),
                              getMapSSValueAsBinaryUint(argsB, "m"),
                              config),
      compareDisplacementSH2(getMapSSValueAsBinaryUint(argsA, "d"),
                             getMapSSValueAsBinaryUint(argsB, "d"),
                             config)
    );
    break;
  case addrModeSH2AtDisp8GBR:
  case addrModeSH2AtDisp8PC:
  case addrModeSH2Disp8:
  case addrModeSH2Disp12:
    return compareDisplacementSH2(getMapSSValueAsBinaryUint(argsA, "d"),
                                  getMapSSValueAsBinaryUint(argsB, "d"),
                                  config);
    break;
  case addrModeSH2ImmedZ:
  case addrModeSH2ImmedS:
  case addrModeSH2ImmedZQ:
    return compareDisplacementSH2(getMapSSValueAsBinaryUint(argsA, "i"),
                                  getMapSSValueAsBinaryUint(argsB, "i"),
                                  config);
    break;
  default:
    error("Comparing unknown SH2 address mode");
    fatal();
    break;
  }
  
  /* suppress compiler warning */
  return opcodeSimilarityNone;
}

OpcodeSimilarity compareOpcodeSH2(Opcode* obj, Opcode* other,
                         DismSettings* config) {
  
  SubDataSH2* datA = (SubDataSH2*)(obj->data_);
  SubDataSH2* datB = (SubDataSH2*)(other->data_);
  
  if ((datA->srcMode != datB->srcMode)
      || (datA->dstMode != datB->dstMode)) return opcodeSimilarityNone;
  
  return greaterOpDifference(
    compareOpcodeByModeSH2(datA->args, datB->args, datA->srcMode, config),
    compareOpcodeByModeSH2(datA->args, datB->args, datA->dstMode, config)
  );
}

void destructorOpcodeSH2(Opcode* obj) {
  SubDataSH2* dat = (SubDataSH2*)(obj->data_);
  freeMapSS(dat->args);
  free(dat);
}

void ModuleSH2destroyInternal(DismModule* obj) {
  freeModDataSH2((ModDataSH2*)(obj->data_));
}

ModDataSH2* allocModDataSH2() {
  ModDataSH2* obj = malloc(sizeof(ModDataSH2));
  initCodeMap(&(obj->codeMap));
  return obj;
}

void freeModDataSH2(ModDataSH2* obj) {
  obj->codeMap.destroyAll(&(obj->codeMap));
}

void ModuleSH2disassemble(DismModule* obj, DismStruct* dismStruct,
                           unsigned int start, unsigned int end) {
  /* Reset the temporary codemap */
  ModDataSH2* modData = (ModDataSH2*)(obj->data_);
  modData->codeMap.clear(&(modData->codeMap));
  modData->codeMap.resize(&(modData->codeMap),
      dismStruct->stream->size(dismStruct->stream));
  modData->codeMap.fill(&(modData->codeMap), 0);
  
  /* Disassemble as usual */
  DismModuledisassemble(obj, dismStruct, start, end);
}
  
int ModuleSH2tryOpRead(DismModule* obj, DismStruct* dismStruct,
                        OpInfoArray ops, unsigned int remaining) {
  /* Is a word/long known to exist at this location? */
  ModDataSH2* modData = (ModDataSH2*)(obj->data_);
  unsigned char marker
      = (unsigned char)(modData->codeMap.get(&(modData->codeMap),
              dismStruct->stream->pos(dismStruct->stream)));
  switch (marker) {
  case codeMapLongMarker:
  case codeMapWordMarker:
    /* yes */
    {
      Opcode opcode;
      initOpcode(&opcode);
      opcode.info_ = (marker == codeMapLongMarker) ? &longOpSH2Info
                        : &wordOpSH2Info;
      opcode.info_->generateOpcode(
          opcode.info_, &opcode, &(dismStruct->settings));
      int result = opcode.read(
        &opcode, dismStruct->stream, &(dismStruct->settings),
          NULL);
      dismStruct->opcodes.pushBack(&(dismStruct->opcodes), opcode);
      dismStruct->stream->seekOff(dismStruct->stream, result);
      return result;
    }
    break;
  default:
    /* no */
    break;
  }
  
  /* Do the usual tryOpRead() */
  return DismModuletryOpRead(obj, dismStruct, ops, remaining);
}

void ModuleSH2printComparedDisassemblyConsts(DismModule* obj, String* dst,
                                DismStruct* firstDismStruct,
                                DismStruct* secondDismStruct) {
  
  /* 
    - read ops sequentially
    - if op IDs match, print disassemblies side-by-side and continue
    - if op IDs don't match, figure out how to realign stream
      - try to find if the pending sequence in A is located somewhere
        further ahead in B (i.e. content was added in B)
        - print B only until we reach the resumption point
      - try the above with A and B interchanged (i.e. content was removed
        in B)
        - print A only until we reach the resumption point
      - if both of those fail, assume we are at the start of a transformed
        block
        - advance A and B by one token and repeat the search procedure
          outlined above until it succeeds. At that point, all the
          preceding content from the initial detection to the success
          point is the transformed block, possibly followed by an addition
          or removal section
  */
  
  VectorOpcode* opcodesA = &(firstDismStruct->opcodes);
  VectorOpcode* opcodesB = &(secondDismStruct->opcodes);
  unsigned int opcodesASize = (opcodesA->size(opcodesA));
  unsigned int opcodesBSize = (opcodesB->size(opcodesB));
  
  String tempString;
  initString(&tempString);
  
  /* Stop at end of shorter stream */
  unsigned int limit = (opcodesASize < opcodesBSize)
                         ? opcodesASize
                         : opcodesBSize;
  unsigned int iA = 0;
  unsigned int iB = 0;
  
  while ((iA < limit) && (iB < limit)) {
    
    Opcode* opcodeA = opcodesA->getP(opcodesA, iA);
    Opcode* opcodeB = opcodesB->getP(opcodesB, iB);
    OpInfo* opInfoA = opcodeA->info(opcodeA);
    OpInfo* opInfoB = opcodeB->info(opcodeB);
    
/*    fprintf(stderr, "%d %d\n", iA, iB); */
    
    /* Are the current Opcodes of the same type? */
    if (opInfoA->id == opInfoB->id) {
      /* Yes */
      
      /* Is the current Opcode a word or a long? */
      if ((opInfoA->id == DATA_WORD_ID)
          || (opInfoA->id == DATA_LONG_ID)) {
          
        int valueA = *((int*)(opcodeA->data_));
        int valueB = *((int*)(opcodeB->data_));
        
        /* Value mismatch -- screen for Saturn addresses */
        if ((valueA != valueB)
            && (((unsigned int)valueA < 0x06000000)
                || ((unsigned int)valueA >= 0x80000000))) {
          
          dst->catInt(dst, opcodeA->pos_, "%08X");
          dst->catC(dst, " ");
          dst->catInt(dst, opcodeB->pos_, "%08X");
          dst->catC(dst, " ");
        
          if ((opInfoA->id == DATA_WORD_ID)) {
            dst->catC(dst, "W ");
            dst->catInt(dst, valueA, "%04X");
            dst->catC(dst, " ");
            dst->catInt(dst, valueB, "%04X");
          }
          else if ((opInfoA->id == DATA_LONG_ID)) {
            dst->catC(dst, "L ");
            dst->catInt(dst, valueA, "%08X");
            dst->catC(dst, " ");
            dst->catInt(dst, valueB, "%08X");
          }
      
          dst->catC(dst, "\n");
        }
      }
      
      ++iA;
      ++iB;
      continue;
    }
    
/*    printf("%04X %04X\n", iA, iB); */
/*    printf("%04X %04X\n", opcodeA->pos(opcodeA), opcodeB->pos(opcodeB)); */
    
    /* Opcodes are of different types: stream realignment needed */
    OpcodeAlignmentData alignment = obj->detectNewAlignment(obj,
                      firstDismStruct,
                      secondDismStruct,
                      iA, iB,
                      limit,
                      firstDismStruct->settings.requiredSequentialOps,
                      firstDismStruct->settings.maxChangeBlockSize);
    
    /* Check if we hit the limit */
    if ((alignment.alignmentType == opcodeAlignmentTransformed)
        && ((alignment.iA >= limit) && (alignment.iB >= limit))) {
      
      /* Print remaining shared content */
      while ((iA < opcodesASize) && (iB < opcodesBSize)) {
        
        ++iA;
        ++iB;
      }
      
      /* Done */
      break;
    }
                      
    /* Deal with transform block, if present */
    if (alignment.transformBlockLength != 0) {
      
      /* Skip over transform block */
      iA += alignment.transformBlockLength;
      iB += alignment.transformBlockLength;
      
/*      printf("y: %d %d\n", iA, iB); */
    
/*      iA = alignment.iA;
      iB = alignment.iB;
      printf("%d %d\n", alignment.iA, alignment.iB);
      continue; */
    }
    
    /* Move to new alignment positions */
    iA = alignment.iA;
    iB = alignment.iB;
  }
  
  
  tempString.destroy(&tempString);
}

void ModuleSH2printComparedDisassemblyImmeds(DismModule* obj, String* dst,
                                DismStruct* firstDismStruct,
                                DismStruct* secondDismStruct) {
  
  /* 
    - read ops sequentially
    - if op IDs match, print disassemblies side-by-side and continue
    - if op IDs don't match, figure out how to realign stream
      - try to find if the pending sequence in A is located somewhere
        further ahead in B (i.e. content was added in B)
        - print B only until we reach the resumption point
      - try the above with A and B interchanged (i.e. content was removed
        in B)
        - print A only until we reach the resumption point
      - if both of those fail, assume we are at the start of a transformed
        block
        - advance A and B by one token and repeat the search procedure
          outlined above until it succeeds. At that point, all the
          preceding content from the initial detection to the success
          point is the transformed block, possibly followed by an addition
          or removal section
  */
  
  VectorOpcode* opcodesA = &(firstDismStruct->opcodes);
  VectorOpcode* opcodesB = &(secondDismStruct->opcodes);
  unsigned int opcodesASize = (opcodesA->size(opcodesA));
  unsigned int opcodesBSize = (opcodesB->size(opcodesB));
  
  String tempString;
  initString(&tempString);
  
  /* Stop at end of shorter stream */
  unsigned int limit = (opcodesASize < opcodesBSize)
                         ? opcodesASize
                         : opcodesBSize;
  unsigned int iA = 0;
  unsigned int iB = 0;
  
  while ((iA < limit) && (iB < limit)) {
    
    Opcode* opcodeA = opcodesA->getP(opcodesA, iA);
    Opcode* opcodeB = opcodesB->getP(opcodesB, iB);
    OpInfo* opInfoA = opcodeA->info(opcodeA);
    OpInfo* opInfoB = opcodeB->info(opcodeB);
    
/*    fprintf(stderr, "%d %d\n", iA, iB); */
    
    /* Are the current Opcodes of the same type? */
    if (opInfoA->id == opInfoB->id) {
      /* Yes */
      
      /* Does the current Opcode use immediate addressing? */
      if ((strlen(opInfoA->recString) == 16)
          && (strcmp(opInfoA->recString + 8, "iiiiiiii") == 0)) {
          
        SubDataSH2* subDataA = (SubDataSH2*)(opcodeA->data_);
        SubDataSH2* subDataB = (SubDataSH2*)(opcodeB->data_);
        
        MapSS* argsA = subDataA->args;
        MapSS* argsB = subDataB->args;
        
        int valueA = getMapSSValueAsBinaryUint(argsA, "i");
        int valueB = getMapSSValueAsBinaryUint(argsB, "i");
        
        /* Value mismatch -- screen for Saturn addresses */
        if (valueA != valueB) {
          
          dst->catInt(dst, opcodeA->pos_, "%08X");
          dst->catC(dst, " ");
          dst->catInt(dst, opcodeB->pos_, "%08X");
          dst->catC(dst, " ");
        
          dst->catC(dst, "B ");
          dst->catInt(dst, valueA, "%02X");
          dst->catC(dst, " ");
          dst->catInt(dst, valueB, "%02X");
      
          dst->catC(dst, "\n");
        }
      }
      
      ++iA;
      ++iB;
      continue;
    }
    
/*    printf("%04X %04X\n", iA, iB); */
/*    printf("%04X %04X\n", opcodeA->pos(opcodeA), opcodeB->pos(opcodeB)); */
    
    /* Opcodes are of different types: stream realignment needed */
    OpcodeAlignmentData alignment = obj->detectNewAlignment(obj,
                      firstDismStruct,
                      secondDismStruct,
                      iA, iB,
                      limit,
                      firstDismStruct->settings.requiredSequentialOps,
                      firstDismStruct->settings.maxChangeBlockSize);
    
    /* Check if we hit the limit */
    if ((alignment.alignmentType == opcodeAlignmentTransformed)
        && ((alignment.iA >= limit) && (alignment.iB >= limit))) {
      
      /* Print remaining shared content */
      while ((iA < opcodesASize) && (iB < opcodesBSize)) {
        
        ++iA;
        ++iB;
      }
      
      /* Done */
      break;
    }
                      
    /* Deal with transform block, if present */
    if (alignment.transformBlockLength != 0) {
      
      /* Skip over transform block */
      iA += alignment.transformBlockLength;
      iB += alignment.transformBlockLength;
      
/*      printf("y: %d %d\n", iA, iB); */
    
/*      iA = alignment.iA;
      iB = alignment.iB;
      printf("%d %d\n", alignment.iA, alignment.iB);
      continue; */
    }
    
    /* Move to new alignment positions */
    iA = alignment.iA;
    iB = alignment.iB;
  }
  
  
  tempString.destroy(&tempString);
}

void generateOpcodeSH2(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareOpcodeSH2;
  dst->readStep = readStepOpcodeSH2;
  dst->printString = printOpcodeSH2;
  dst->printName = OpcodeprintNameWithSpace;
  
  dst->setData(dst, malloc(sizeof(SubDataSH2)));
  dst->destroy = destructorOpcodeSH2;
}

void initModuleSH2(DismModule* obj) {
  initDismModule(obj);
  obj->disassemble = ModuleSH2disassemble;
  obj->tryOpRead = ModuleSH2tryOpRead;
  obj->destroyInternal = ModuleSH2destroyInternal;
  obj->data_ = allocModDataSH2();
  obj->byteAlignment_ = 2;
  
/*  obj->printComparedDisassembly = ModuleSH2printComparedDisassemblyConsts; */
/*  obj->printComparedDisassembly = ModuleSH2printComparedDisassemblyImmeds; */
  
  OpInfoArray ops = { opcodesSH2, sizeof(opcodesSH2) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModuleSH2() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModuleSH2(obj);
  return obj;
}

void freeModuleSH2(DismModule* obj) {
  free(obj);
}

