#include "modules/6502/Module6502.h"
#include "util/ByteConv.h"
#include <stdlib.h>

OpInfo opcodes6502[] = {
/*  { "adc", "01110001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "adc", "01101101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "adc", "01111101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "adc", "01111001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "adc", "01101001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "adc", "01100001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "adc", "01100101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "adc", "01110101xxxxxxxx", opFlagsNone, generate6502ZPX } */
  
  { "adc", "01110001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "adc", "01101101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "adc", "01111101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "adc", "01111001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "adc", "01101001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "adc", "01100001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "adc", "01100101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "adc", "01110101xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "and", "00110001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "and", "00101101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "and", "00111101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "and", "00111001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "and", "00101001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "and", "00100001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "and", "00100101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "and", "00110101xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "asl", "00001110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "asl", "00011110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "asl", "00001010", opFlagsNone, generate6502A },
  { "asl", "00000110xxxxxxxx", opFlagsNone, generate6502ZP },
  { "asl", "00010110xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "bcc", "10010000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "bcs", "10110000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "beq", "11110000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "bit", "00101100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "bit", "00100100xxxxxxxx", opFlagsNone, generate6502ZP },
  { "bmi", "00110000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "bne", "11010000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "bpl", "00010000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "brk", "00000000", opFlagsSuspicious, generate6502Implied },
  { "bvc", "01010000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "bvs", "01110000xxxxxxxx", opFlagsNone, generate6502Relative },
  { "clc", "00011000", opFlagsNone, generate6502Implied },
  { "cld", "11011000", opFlagsNone, generate6502Implied },
  { "cli", "01011000", opFlagsNone, generate6502Implied },
  { "clv", "10111000", opFlagsNone, generate6502Implied },
  { "cmp", "11010001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "cmp", "11001101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "cmp", "11011101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "cmp", "11011001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "cmp", "11001001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "cmp", "11000001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "cmp", "11000101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "cmp", "11010101xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "cpx", "11101100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "cpx", "11100000xxxxxxxx", opFlagsNone, generate6502Immed },
  { "cpx", "11100100xxxxxxxx", opFlagsNone, generate6502ZP },
  { "cpy", "11001100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "cpy", "11000000xxxxxxxx", opFlagsNone, generate6502Immed },
  { "cpy", "11000100xxxxxxxx", opFlagsNone, generate6502ZP },
  { "dec", "11001110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "dec", "11011110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "dec", "11000110xxxxxxxx", opFlagsNone, generate6502ZP },
  { "dec", "11010110xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "dex", "11001010", opFlagsNone, generate6502Implied },
  { "dey", "10001000", opFlagsNone, generate6502Implied },
  { "eor", "01010001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "eor", "01001101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "eor", "01011101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "eor", "01011001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "eor", "01001001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "eor", "01000001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "eor", "01000101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "eor", "01010101xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "inc", "11101110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "inc", "11111110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "inc", "11100110xxxxxxxx", opFlagsNone, generate6502ZP },
  { "inc", "11110110xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "inx", "11101000", opFlagsNone, generate6502Implied },
  { "iny", "11001000", opFlagsNone, generate6502Implied },
  { "jmp", "01001100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "jmp", "01101100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Indirect },
  { "jsr", "00100000xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "lda", "10110001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "lda", "10101101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "lda", "10111101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "lda", "10111001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "lda", "10101001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "lda", "10100001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "lda", "10100101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "lda", "10110101xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "ldx", "10101110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "ldx", "10111110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "ldx", "10100010xxxxxxxx", opFlagsNone, generate6502Immed },
  { "ldx", "10100110xxxxxxxx", opFlagsNone, generate6502ZP },
  { "ldx", "10110110xxxxxxxx", opFlagsNone, generate6502ZPY },
  { "ldy", "10101100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "ldy", "10111100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "ldy", "10100000xxxxxxxx", opFlagsNone, generate6502Immed },
  { "ldy", "10100100xxxxxxxx", opFlagsNone, generate6502ZP },
  { "ldy", "10110100xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "lsr", "01001110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "lsr", "01000110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "lsr", "01001010", opFlagsNone, generate6502A },
  { "lsr", "01010110xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "nop", "11101010", opFlagsNone, generate6502Implied },
  { "ora", "00010001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "ora", "00001101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "ora", "00011101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "ora", "00011001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "ora", "00001001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "ora", "00000001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "ora", "00000101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "ora", "00010101xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "pha", "01001000", opFlagsNone, generate6502Implied },
  { "php", "00001000", opFlagsNone, generate6502Implied },
  { "pla", "01101000", opFlagsNone, generate6502Implied },
  { "plp", "00101000", opFlagsNone, generate6502Implied },
  { "rol", "00101110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "rol", "00111110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "rol", "00101010", opFlagsNone, generate6502A },
  { "rol", "00100110xxxxxxxx", opFlagsNone, generate6502ZP },
  { "rol", "00110110xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "ror", "01101110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "ror", "01111110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "ror", "01101010", opFlagsNone, generate6502A },
  { "ror", "01100110xxxxxxxx", opFlagsNone, generate6502ZP },
  { "ror", "01110110xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "rti", "01000000", opFlagsNone, generate6502Implied },
  { "rts", "01100000", opFlagsNone, generate6502Implied },
  { "sbc", "11110001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "sbc", "11101101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "sbc", "11111101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "sbc", "11111001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "sbc", "11101001xxxxxxxx", opFlagsNone, generate6502Immed },
  { "sbc", "11101011", opFlagsNone, generate6502Immed },
  { "sbc", "11100001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "sbc", "11100101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "sbc", "11110101xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "sec", "00111000", opFlagsNone, generate6502Implied },
  { "sed", "11111000", opFlagsNone, generate6502Implied },
  { "sei", "01111000", opFlagsNone, generate6502Implied },
  { "sta", "10010001xxxxxxxx", opFlagsNone, generate6502IndY },
  { "sta", "10001101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "sta", "10011101xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsX },
  { "sta", "10011001xxxxxxxxxxxxxxxx", opFlagsNone, generate6502AbsY },
  { "sta", "10000001xxxxxxxx", opFlagsNone, generate6502IndX },
  { "sta", "10000101xxxxxxxx", opFlagsNone, generate6502ZP },
  { "sta", "10010101xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "stx", "10001110xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "stx", "10000110xxxxxxxx", opFlagsNone, generate6502ZP },
  { "stx", "10010110xxxxxxxx", opFlagsNone, generate6502ZPY },
  { "sty", "10001100xxxxxxxxxxxxxxxx", opFlagsNone, generate6502Abs },
  { "sty", "10000100xxxxxxxx", opFlagsNone, generate6502ZP },
  { "sty", "10010100xxxxxxxx", opFlagsNone, generate6502ZPX },
  { "tax", "10101010", opFlagsNone, generate6502Implied },
  { "tay", "10101000", opFlagsNone, generate6502Implied },
  { "tsx", "10111010", opFlagsNone, generate6502Implied },
  { "txa", "10001010", opFlagsNone, generate6502Implied },
  { "txs", "10011010", opFlagsNone, generate6502Implied },
  { "tya", "10011000", opFlagsNone, generate6502Implied }
};
 
OpcodeSimilarity compareAddress6502(unsigned int first, unsigned int second,
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
 
OpcodeSimilarity compareConstant6502(unsigned int first, unsigned int second,
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

OpcodeSimilarity compareRelative6502(int first, int second,
                                     DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

OpcodeSimilarity compare1bAddress6502(Opcode* first, Opcode* second,
                                      DismSettings* config) {
  return compareAddress6502(
                     ((Struct6502Single*)(first->data(first)))->value,
                     ((Struct6502Single*)(second->data(second)))->value,
                     config);
}

OpcodeSimilarity compare2bAddress6502(Opcode* first, Opcode* second,
                                      DismSettings* config) {
  return compareAddress6502(
                     ((Struct6502Double*)(first->data(first)))->value,
                     ((Struct6502Double*)(second->data(second)))->value,
                     config);
}

OpcodeSimilarity compare1bConstant6502(Opcode* first, Opcode* second,
                                      DismSettings* config) {
  return compareConstant6502(
                     ((Struct6502Single*)(first->data(first)))->value,
                     ((Struct6502Single*)(second->data(second)))->value,
                     config);
}

OpcodeSimilarity compare2bConstant6502(Opcode* first, Opcode* second,
                                      DismSettings* config) {
  return compareConstant6502(
                     ((Struct6502Double*)(first->data(first)))->value,
                     ((Struct6502Double*)(second->data(second)))->value,
                     config);
}

OpcodeSimilarity compare1bRelative6502(Opcode* first, Opcode* second,
                                      DismSettings* config) {
  return compareRelative6502(
                     ((Struct6502Double*)(first->data(first)))->value,
                     ((Struct6502Double*)(second->data(second)))->value,
                     config);
}



unsigned int read1bArg6502(Opcode* dst, BufferStream* stream,
                           DismSettings* config) {
  Struct6502Single* dat = (Struct6502Single*)(dst->data_);
  dat->value = fromBytes(stream->getcur(stream) + 1,
                         1, littleEnd, nosign);
  return 2;
}

unsigned int read2bArg6502(Opcode* dst, BufferStream* stream,
                           DismSettings* config) {
  Struct6502Single* dat = (Struct6502Single*)(dst->data_);
  dat->value = fromBytes(stream->getcur(stream) + 1,
                         2, littleEnd, nosign);
  return 3;
}

int read1bRelative6502(Opcode* dst, BufferStream* stream,
                           DismSettings* config) {
  Struct6502Single* dat = (Struct6502Single*)(dst->data_);
  dat->value = fromBytes(stream->getcur(stream) + 1,
                         1, littleEnd, sign);
  return 2;
}



void print1bAddress6502(String* dst, Opcode* src,
                      DismSettings* config) {
  print1bAddress6502Raw(dst, (unsigned int)singleValue6502(src), config);
}

void print2bAddress6502(String* dst, Opcode* src,
                      DismSettings* config) {
  print2bAddress6502Raw(dst, (unsigned int)singleValue6502(src), config);
}

void print1bConstant6502(String* dst, Opcode* src,
                      DismSettings* config) {
  print1bConstant6502Raw(dst, singleValue6502(src), config);
}

void print2bConstant6502(String* dst, Opcode* src,
                      DismSettings* config) {
  print2bConstant6502Raw(dst, singleValue6502(src), config);
}

void print1bRelative6502(String* dst, Opcode* src,
                     DismSettings* config) {
  print1bRelative6502Raw(dst, singleValue6502(src), config);
}

void print1bAddress6502Raw(String* dst, unsigned int value,
                      DismSettings* config) {
  dst->catInt(dst, value, "$%02X");
}

void print2bAddress6502Raw(String* dst, unsigned int value,
                      DismSettings* config) {
  dst->catInt(dst, value, "$%04X");
}

void print1bConstant6502Raw(String* dst, unsigned int value,
                      DismSettings* config) {
  dst->catInt(dst, value, "#$%02X");
}

void print2bConstant6502Raw(String* dst, unsigned int value,
                      DismSettings* config) {
  dst->catInt(dst, value, "#$%04X");
}

void print1bRelative6502Raw(String* dst, int value,
                     DismSettings* config) {
  if (value < 0) {
    dst->catInt(dst, -value, "-$%02X");
  }
  else {
    dst->catInt(dst, value, "+$%02X");
  }
}

/* ==================== Abs ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(Abs);
/*void generate6502Abs(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compare6502Abs;
  dst->readStep = readStep6502Abs;
  dst->printString = printString6502Abs;
  dst->setData(dst, malloc(sizeof(Struct6502Single)));
} */

OpcodeSimilarity compare6502Abs(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare2bAddress6502(obj, other, config);
}

unsigned int readStep6502Abs(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read2bArg6502(obj, stream, config);
}

void printString6502Abs(Opcode* obj, String* dst,
                         DismSettings* config) {
  print2bAddress6502(dst, obj, config);
}

/* ==================== AbsX ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(AbsX);

OpcodeSimilarity compare6502AbsX(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare2bAddress6502(obj, other, config);
}

unsigned int readStep6502AbsX(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read2bArg6502(obj, stream, config);
}

void printString6502AbsX(Opcode* obj, String* dst,
                         DismSettings* config) {
  print2bAddress6502(dst, obj, config);
  dst->catC(dst, ",X");
}

/* ==================== AbsY ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(AbsY);

OpcodeSimilarity compare6502AbsY(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare2bAddress6502(obj, other, config);
}

unsigned int readStep6502AbsY(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read2bArg6502(obj, stream, config);
}

void printString6502AbsY(Opcode* obj, String* dst,
                         DismSettings* config) {
  print2bAddress6502(dst, obj, config);
  dst->catC(dst, ",Y");
}

/* ==================== A ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(A);

OpcodeSimilarity compare6502A(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return 1;
}

unsigned int readStep6502A(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return 1;
}

void printString6502A(Opcode* obj, String* dst,
                         DismSettings* config) {
/*  dst->catC(dst, "A"); */
}

/* ==================== Immed ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(Immed);

OpcodeSimilarity compare6502Immed(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare1bConstant6502(obj, other, config);
}

unsigned int readStep6502Immed(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read1bArg6502(obj, stream, config);
}

void printString6502Immed(Opcode* obj, String* dst,
                         DismSettings* config) {
/*  dst->catC(dst, "#"); */
  print1bConstant6502(dst, obj, config);
}

/* ==================== Implied ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(Implied);

OpcodeSimilarity compare6502Implied(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return 1;
}

unsigned int readStep6502Implied(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return 1;
}

void printString6502Implied(Opcode* obj, String* dst,
                         DismSettings* config) {
  /* nothing to print */
}

/* ==================== IndX ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(IndX);

OpcodeSimilarity compare6502IndX(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare1bAddress6502(obj, other, config);
}

unsigned int readStep6502IndX(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read1bArg6502(obj, stream, config);
}

void printString6502IndX(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catC(dst, "(");
  print1bAddress6502(dst, obj, config);
  dst->catC(dst, ",X)");
}

/* ==================== Indirect ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(Indirect);

OpcodeSimilarity compare6502Indirect(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare2bAddress6502(obj, other, config);
}

unsigned int readStep6502Indirect(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read2bArg6502(obj, stream, config);
}

void printString6502Indirect(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catC(dst, "(");
  print2bAddress6502(dst, obj, config);
  dst->catC(dst, ")");
}

/* ==================== IndY ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(IndY);
/*void generate6502IndY(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compare6502IndY;
  dst->readStep = readStep6502IndY;
  dst->printString = printString6502IndY;
  dst->setData(dst, malloc(sizeof(Struct6502Single)));
} */

OpcodeSimilarity compare6502IndY(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare1bAddress6502(obj, other, config);
                                 
/*  Struct6502Single* dat1 = (Struct6502Single*)(obj->data_);
  Struct6502Single* dat2 = (Struct6502Single*)(other->data_);
  
  if (dat1->value == dat2->value) {
    return opcodeSimilaritySame;
  }
  else if (config->addressChangesDistinct) {
    return opcodeSimilarityDistinct;
  }
  else {
    return opcodeSimilarityNear;
  } */
}

unsigned int readStep6502IndY(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
/*  Struct6502Single* dat = (Struct6502Single*)(obj->data_);
  dat->value = fromBytes(stream->getcur(stream) + 1,
                         1, littleEnd, nosign);
  return 2; */
  return read1bArg6502(obj, stream, config);
}

void printString6502IndY(Opcode* obj, String* dst,
                         DismSettings* config) {
  dst->catC(dst, "(");
  print1bAddress6502(dst, obj, config);
  dst->catC(dst, "),Y");
}

/* ==================== Relative ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(Relative);

OpcodeSimilarity compare6502Relative(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare1bRelative6502(obj, other, config);
}

unsigned int readStep6502Relative(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read1bRelative6502(obj, stream, config);
}

void printString6502Relative(Opcode* obj, String* dst,
                         DismSettings* config) {
  print1bRelative6502(dst, obj, config);
}

/* ==================== ZP ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(ZP);

OpcodeSimilarity compare6502ZP(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare1bAddress6502(obj, other, config);
}

unsigned int readStep6502ZP(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read1bArg6502(obj, stream, config);
}

void printString6502ZP(Opcode* obj, String* dst,
                         DismSettings* config) {
  print1bAddress6502(dst, obj, config);
}

/* ==================== ZPX ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(ZPX);

OpcodeSimilarity compare6502ZPX(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare1bAddress6502(obj, other, config);
}

unsigned int readStep6502ZPX(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read1bArg6502(obj, stream, config);
}

void printString6502ZPX(Opcode* obj, String* dst,
                         DismSettings* config) {
  print1bAddress6502(dst, obj, config);
  dst->catC(dst, ",X");
}

/* ==================== ZPY ==================== */

GENERATE_6502_OPCODE_GENERATION_DEFINITION(ZPY);

OpcodeSimilarity compare6502ZPY(Opcode* obj, Opcode* other,
                                 DismSettings* config) {
  return compare1bAddress6502(obj, other, config);
}

unsigned int readStep6502ZPY(Opcode* obj, BufferStream* stream,
                              DismSettings* config, MapSS* args) {
  return read1bArg6502(obj, stream, config);
}

void printString6502ZPY(Opcode* obj, String* dst,
                         DismSettings* config) {
  print1bAddress6502(dst, obj, config);
  dst->catC(dst, ",Y");
}



int singleValue6502(Opcode* src) {
  return ((Struct6502Single*)(src->data_))->value;
}



void initModule6502(DismModule* obj) {
  initDismModule(obj);
  
  /* Disable arg collation */
  obj->enableOpArgCollation_ = 0;
  
  OpInfoArray ops = { opcodes6502, sizeof(opcodes6502) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModule6502() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModule6502(obj);
  return obj;
}

void freeModule6502(DismModule* obj) {
  free(obj);
}
