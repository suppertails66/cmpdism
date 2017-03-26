#include "modules/MIPS1/ModuleMIPS1.h"
#include "modules/StringMatcher.h"
#include "modules/OpArgCollator.h"
#include "modules/Consts.h"
#include "util/Logger.h"
#include "util/ByteConv.h"
#include <stdlib.h>
#include <string.h>


/* Forward declaration needed for opcodesMIPS1 array */
void generateOpcodeMIPS1(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config);

OpInfo opcodesMIPS1[] = {
  { "add", "000000ssssstttttdddddaaaaa100000",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "addu", "000000ssssstttttdddddaaaaa100001",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "and", "000000ssssstttttdddddaaaaa100100",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "break", "000000ssssstttttdddddaaaaa001101",
     opFlagsNone, generateOpcodeMIPS1, 0, "R. , , " },
  { "div", "000000ssssstttttdddddaaaaa011010",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.s,t, " },
  { "divu", "000000ssssstttttdddddaaaaa011011",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.s,t, " },
  { "jalr", "000000ssssstttttdddddaaaaa001001",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s, " },
  { "jr", "000000ssssstttttdddddaaaaa001000",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.s, , " },
  { "mfhi", "000000ssssstttttdddddaaaaa010000",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d, , " },
  { "mflo", "000000ssssstttttdddddaaaaa010010",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d, , " },
  { "mthi", "000000ssssstttttdddddaaaaa010001",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.s, , " },
  { "mtlo", "000000ssssstttttdddddaaaaa010011",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.s, , " },
  { "mult", "000000ssssstttttdddddaaaaa011000",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.s,t, " },
  { "multu", "000000ssssstttttdddddaaaaa011001",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.s,t, " },
  { "nor", "000000ssssstttttdddddaaaaa100111",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "or", "000000ssssstttttdddddaaaaa100101",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "sll", "000000ssssstttttdddddaaaaa000000",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,t,a" },
  { "sllv", "000000ssssstttttdddddaaaaa000100",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,t,s" },
  { "slt", "000000ssssstttttdddddaaaaa101010",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "sltu", "000000ssssstttttdddddaaaaa101011",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "sra", "000000ssssstttttdddddaaaaa000011",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,t,a" },
  { "srav", "000000ssssstttttdddddaaaaa000111",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,t,s" },
  { "srl", "000000ssssstttttdddddaaaaa000010",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,t,a" },
  { "srlv", "000000ssssstttttdddddaaaaa000110",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,t,s" },
  { "sub", "000000ssssstttttdddddaaaaa100010",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "subu", "000000ssssstttttdddddaaaaa100011",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "syscall", "000000ssssstttttdddddaaaaa001100",
     opFlagsNone, generateOpcodeMIPS1, 0, "R. , , " },
  { "xor", "000000ssssstttttdddddaaaaa100110",
     opFlagsNone, generateOpcodeMIPS1, 0, "R.d,s,t" },
  { "addi", "001000ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,s,i" },
  { "addiu", "001001ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,s,i" },
  { "andi", "001100ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,s,i" },
  { "beq", "000100ssssstttttllllllllllllllll",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.s,t,l" },
  { "bgez", "000001sssss00001llllllllllllllll",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.s,l, " },
  { "bgtz", "000111sssss00000llllllllllllllll",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.s,l, " },
  { "blez", "000110sssss00000llllllllllllllll",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.s,l, " },
  { "bltz", "000001sssss00000llllllllllllllll",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.s,l, " },
  { "bne", "000101ssssstttttllllllllllllllll",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.s,t,l" },
  { "lb", "100000ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "lbu", "100100ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "lh", "100001ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "lhu", "100101ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "lui", "001111ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,i, " },
  { "lw", "100011ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "lwc1", "110001ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "ori", "001101ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,s,i" },
  { "sb", "101000ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "slti", "001010ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,s,i" },
  { "sltiu", "001011ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,s,i" },
  { "sh", "101001ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "sw", "101011ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "swc1", "111001ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,R, " },
  { "xori", "001110ssssstttttiiiiiiiiiiiiiiii",
     opFlagsNone, generateOpcodeMIPS1, 0, "I.t,s,i" },
  { "j", "000010LLLLLLLLLLLLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeMIPS1, 0, "J.L, , " },
  { "jal", "000011LLLLLLLLLLLLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeMIPS1, 0, "J.L, , " }
};

const char* registerNamesMIPS1[] = {
  "zero",
  "at",
  "v0",
  "v1",
  "a0",
  "a1",
  "a2",
  "a3",
  "t0",
  "t1",
  "t2",
  "t3",
  "t4",
  "t5",
  "t6",
  "t7",
  "s0",
  "s1",
  "s2",
  "s3",
  "s4",
  "s5",
  "s6",
  "s7",
  "t8",
  "t9",
  "k0",
  "k1",
  "gp",
  "sp",
  "fp",
  "ra"
};

const char* getRegisterNameMIPS1(int index) {
  return registerNamesMIPS1[index];
}

void printRegisterMIPS1(int index, String* dst, DismSettings* config) {
  dst->catC(dst, "$");
  dst->catC(dst, getRegisterNameMIPS1(index));
}

void printSignedConstantMIPS1(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "%04X");
}

void printUnsignedConstantMIPS1(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "%04X");
}

void printOffsetMIPS1(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "%04X");
}

void printShiftAmountMIPS1(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "%d");
}

void printAddressMIPS1(int value, String* dst, DismSettings* config) {
  /* High 4 bits and low 2 bits of PC aren't known */
  value <<= 2;
  
  dst->catC(dst, "X");
  dst->catInt(dst, value, "%07X");
}

void setParameterMIPS1(SubDataMIPS1* dat, MapSS* args, char code, int num) {
  if (code == ' ') return;

  int* target;
  if (num == 0) target = &(dat->first);
  else if (num == 1) target = &(dat->second);
  else target = &(dat->third);
  
  char buffer[2];
  buffer[0] = code;
  buffer[1] = 0;
  
  switch (code) {
  /* Indexed -- this is actually two parameters */
  case 'R':
    {
    dat->second = getMapSSValueAsBinaryUint(args, "s");
    dat->third = getMapSSValueAsBinaryUint(args, "i");
    }
    break;
  /* offset */
  case 'l':
    {
    *target = getMapSSValueAsBinaryInt(args, buffer);
    }
    break;
  default:
    {
    *target = getMapSSValueAsBinaryUint(args, buffer);
    }
    break;
  }
}

unsigned int readStepOpcodeMIPS1(struct Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args) {
  
/*  unsigned int streamBase = stream->tell(stream); */
  unsigned int total = strlen(obj->info_->recString) / k_bitsPerByte;
  
  SubDataMIPS1* dat = (SubDataMIPS1*)(obj->data_);
  const char* infoString = (const char*)(obj->info(obj)->data);
  
  setParameterMIPS1(dat, args, infoString[2], 0);
  setParameterMIPS1(dat, args, infoString[4], 1);
  setParameterMIPS1(dat, args, infoString[6], 2);
  
  freeMapSS(args);
  
  return total;
}

OpcodeSimilarity compareRegisterMIPS1(int first, int second,
                                      DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

OpcodeSimilarity compareConstantMIPS1(int first, int second,
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

OpcodeSimilarity compareAddressMIPS1(int first, int second,
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

OpcodeSimilarity compareOffsetMIPS1(int first, int second,
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

OpcodeSimilarity compareParameterMIPS1(SubDataMIPS1* datA, SubDataMIPS1* datB,
                           DismSettings* config, char code, int num) {
  if (code == ' ') return opcodeSimilaritySame;
  
  int* targetA;
  int* targetB;
  if (num == 0) { targetA = &(datA->first); targetB = &(datB->first); }
  else if (num == 1) { targetA = &(datA->second); targetB = &(datB->second); }
  else { targetA = &(datA->third); targetB = &(datB->third); }

  switch (code) {
  /* Indexed -- this is actually two parameters */
  case 'R':
    {
    return greaterOpDifference(
      compareOffsetMIPS1(datA->third, datB->third, config),
      compareRegisterMIPS1(datA->second, datB->second, config)
    );
    }
    break;
  /* register */
  case 's':
    return compareRegisterMIPS1(*targetA, *targetB, config);
    break;
  /* register */
  case 't':
    return compareRegisterMIPS1(*targetA, *targetB, config);
    break;
  /* register */
  case 'd':
    return compareRegisterMIPS1(*targetA, *targetB, config);
    break;
  /* immediate */
  case 'i':
    return compareConstantMIPS1(*targetA, *targetB, config);
    break;
  /* shift amount */
  case 'a':
    return compareConstantMIPS1(*targetA, *targetB, config);
    break;
  /* offset */
  case 'l':
    return compareOffsetMIPS1(*targetA, *targetB, config);
    break;
  /* absolute label */
  case 'L':
    return compareAddressMIPS1(*targetA, *targetB, config);
    break;
  default:
    {
    error("Unknown MIPS1 parameter type (compare): ");
    char buffer[2];
    buffer[0] = code;
    buffer[1] = 0;
    error(buffer);
    fatal();
    }
    break;
  }
  
  /* suppress compiler warning */
  return opcodeSimilarityNone;
}

OpcodeSimilarity compareOpcodeMIPS1(Opcode* obj, Opcode* other,
                         DismSettings* config) {
  
  SubDataMIPS1* datA = (SubDataMIPS1*)(obj->data_);
  SubDataMIPS1* datB = (SubDataMIPS1*)(other->data_);
  const char* infoString = (const char*)(obj->info(obj)->data);
  
  return greaterOpDifference(
    greaterOpDifference(
      compareParameterMIPS1(datA, datB, config, infoString[2], 0),
      compareParameterMIPS1(datA, datB, config, infoString[4], 1)
    ),
    compareParameterMIPS1(datA, datB, config, infoString[6], 2)
  );
  
/*  printParameterMIPS1(dat, dst, config, infoString[2], 0);
  printParameterMIPS1(dat, dst, config, infoString[4], 1);
  printParameterMIPS1(dat, dst, config, infoString[6], 2); */
  
/*  if ((datA->srcMode != datB->srcMode)
      || (datA->dstMode != datB->dstMode)) return opcodeSimilarityNone;
  
  return greaterOpDifference(
    compareOpcodeByModeMIPS1(datA->args, datB->args, datA->srcMode, config),
    compareOpcodeByModeMIPS1(datA->args, datB->args, datA->dstMode, config)
  ); */
}

void printParameterMIPS1(SubDataMIPS1* dat, String* dst, DismSettings* config,
                         char code, int num) {
  if (code == ' ') return;
  
  int* target;
  if (num == 0) target = &(dat->first);
  else if (num == 1) target = &(dat->second);
  else target = &(dat->third);
  
  /* Print comma if this isn't the first argument */
  if (num != 0) dst->catC(dst, ", ");

  switch (code) {
  case ' ':
    /* none */
    break;
  /* Indexed -- this is actually two parameters */
  case 'R':
    {
    /* Index */
    printUnsignedConstantMIPS1(dat->third, dst, config);
    
    /* Rs */
    dst->catC(dst, "(");
    printRegisterMIPS1(dat->second, dst, config);
    dst->catC(dst, ")");
    }
    break;
  /* register */
  case 's':
    printRegisterMIPS1(*target, dst, config);
    break;
  /* register */
  case 't':
    printRegisterMIPS1(*target, dst, config);
    break;
  /* register */
  case 'd':
    printRegisterMIPS1(*target, dst, config);
    break;
  /* immediate */
  case 'i':
    printUnsignedConstantMIPS1(*target, dst, config);
    break;
  /* shift amount */
  case 'a':
    printShiftAmountMIPS1(*target, dst, config);
    break;
  /* offset */
  case 'l':
    printOffsetMIPS1(*target, dst, config);
    break;
  /* absolute label */
  case 'L':
    printAddressMIPS1(*target, dst, config);
    break;
  default:
    {
    error("Unknown MIPS1 parameter type (print): ");
    char buffer[2];
    buffer[0] = code;
    buffer[1] = 0;
    error(buffer);
    fatal();
    }
    break;
  }
}

void printOpcodeMIPS1(Opcode* obj, String* dst, DismSettings* config) {
  
  const char* infoString = (const char*)(obj->info(obj)->data);
  
  SubDataMIPS1* dat = (SubDataMIPS1*)(obj->data_);
  
  printParameterMIPS1(dat, dst, config, infoString[2], 0);
  printParameterMIPS1(dat, dst, config, infoString[4], 1);
  printParameterMIPS1(dat, dst, config, infoString[6], 2);
  
}

void generateOpcodeMIPS1(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareOpcodeMIPS1;
  dst->readStep = readStepOpcodeMIPS1;
  dst->printString = printOpcodeMIPS1;
  
  dst->setData(dst, malloc(sizeof(SubDataMIPS1)));
  dst->destroy = destructorOpcodeMIPS1;
  dst->printName = OpcodeprintNameWithSpace;
}

void destructorOpcodeMIPS1(Opcode* obj) {
  SubDataMIPS1* dat = (SubDataMIPS1*)(obj->data_);
/*  freeMapSS(dat->args); */
  free(dat);
}

void ModuleMIPS1destroyInternal(DismModule* obj) {
/*  freeModDataMIPS1((ModDataMIPS1*)(obj->data_)); */
}

void initModuleMIPS1(DismModule* obj) {
  initDismModule(obj);
/*  obj->disassemble = ModuleMIPS1disassemble;
  obj->tryOpRead = ModuleMIPS1tryOpRead; */
  obj->destroyInternal = ModuleMIPS1destroyInternal;
/*  obj->data_ = allocModDataMIPS1(); */
  obj->byteAlignment_ = 4;
  obj->reverseReadEndianness_ = 1;
  obj->reverseMatchEndianness_ = 1;
  obj->reverseArgumentBitOrder_ = 0;
  
  OpInfoArray ops = { opcodesMIPS1, sizeof(opcodesMIPS1) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModuleMIPS1() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModuleMIPS1(obj);
  return obj;
}

void freeModuleMIPS1(DismModule* obj) {
  free(obj);
}
