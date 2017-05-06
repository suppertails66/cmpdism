#include "modules/LR35902/ModuleLR35902.h"
#include "modules/Z80/ModuleZ80.h"
#include "modules/StringMatcher.h"
#include "modules/OpArgCollator.h"
#include "modules/Consts.h"
#include "util/Logger.h"
#include "util/ByteConv.h"
#include <stdlib.h>
#include <string.h>


/* Forward declaration needed for opcodesLR35902 array */
void generateOpcodeLR35902(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config);

OpInfo opcodesLR35902[] = {
  { "adc", "10001RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "a,{R}" }, 
  { "adc", "11001110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "a,{N}" }, 
  { "adc", "10001110",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(hl)" }, 
  { "add", "10000RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "a,{R}" }, 
  { "add", "11000110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "a,{N}" }, 
  { "add", "10000110",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(hl)" }, 
  { "add", "00001001",
     opFlagsNone, generateOpcodeLR35902, 0, "hl,bc" }, 
  { "add", "00011001",
     opFlagsNone, generateOpcodeLR35902, 0, "hl,de" }, 
  { "add", "00101001",
     opFlagsNone, generateOpcodeLR35902, 0, "hl,hl" }, 
  { "add", "00111001",
     opFlagsNone, generateOpcodeLR35902, 0, "hl,sp" }, 
  { "and", "10100RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "and", "11100110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "{N}" }, 
  { "and", "10100110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "bit", "1100101101BBBRRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{B},{R}" }, 
  { "bit", "1100101101BBB110",
     opFlagsNone, generateOpcodeLR35902, 0, "{B},(hl)" }, 
  { "call", "11001101LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "{L}" }, 
  { "call", "11011100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "c,{L}" }, 
  { "call", "11010100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "nc,{L}" }, 
  { "call", "11001100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "z,{L}" }, 
  { "call", "11000100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "nz,{L}" }, 
  { "ccf", "00111111",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "cp", "10111RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "cp", "11111110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "{N}" }, 
  { "cp", "10111110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "cpl", "00101111",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "daa", "00100111",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "dec", "00111101",
     opFlagsNone, generateOpcodeLR35902, 0, "a" }, 
  { "dec", "00000101",
     opFlagsNone, generateOpcodeLR35902, 0, "b" }, 
  { "dec", "00001101",
     opFlagsNone, generateOpcodeLR35902, 0, "c" }, 
  { "dec", "00010101",
     opFlagsNone, generateOpcodeLR35902, 0, "d" }, 
  { "dec", "00011101",
     opFlagsNone, generateOpcodeLR35902, 0, "e" }, 
  { "dec", "00100101",
     opFlagsNone, generateOpcodeLR35902, 0, "h" }, 
  { "dec", "00101101",
     opFlagsNone, generateOpcodeLR35902, 0, "l" }, 
  { "dec", "00110101",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "dec", "00001011",
     opFlagsNone, generateOpcodeLR35902, 0, "bc" }, 
  { "dec", "00011011",
     opFlagsNone, generateOpcodeLR35902, 0, "de" }, 
  { "dec", "00101011",
     opFlagsNone, generateOpcodeLR35902, 0, "hl" }, 
  { "dec", "00111011",
     opFlagsNone, generateOpcodeLR35902, 0, "sp" }, 
  { "di", "11110011",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
     
/*  { "djnz", "00010000OOOOOOOO",
     opFlagsNone, generateOpcodeLR35902, 0, "{O}" }, */
  { "stop", "00010000",
     opFlagsNone, generateOpcodeLR35902, 0, "" },
     
  { "ei", "11111011",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
     
/*  { "ex", "00001000",
     opFlagsNone, generateOpcodeLR35902, 0, "af,af'" }, */
  { "ld", "00001000LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "({L}),sp" },
     
/*  { "exx", "11011001",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, */
  { "reti", "11011001",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
     
  { "halt", "01110110",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "inc", "00111100",
     opFlagsNone, generateOpcodeLR35902, 0, "a" }, 
  { "inc", "00000100",
     opFlagsNone, generateOpcodeLR35902, 0, "b" }, 
  { "inc", "00001100",
     opFlagsNone, generateOpcodeLR35902, 0, "c" }, 
  { "inc", "00010100",
     opFlagsNone, generateOpcodeLR35902, 0, "d" }, 
  { "inc", "00011100",
     opFlagsNone, generateOpcodeLR35902, 0, "e" }, 
  { "inc", "00100100",
     opFlagsNone, generateOpcodeLR35902, 0, "h" }, 
  { "inc", "00101100",
     opFlagsNone, generateOpcodeLR35902, 0, "l" }, 
  { "inc", "00000011",
     opFlagsNone, generateOpcodeLR35902, 0, "bc" }, 
  { "inc", "00010011",
     opFlagsNone, generateOpcodeLR35902, 0, "de" }, 
  { "inc", "00100011",
     opFlagsNone, generateOpcodeLR35902, 0, "hl" }, 
  { "inc", "00110011",
     opFlagsNone, generateOpcodeLR35902, 0, "sp" }, 
  { "inc", "00110100",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "jp", "11000011LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "{L}" }, 
  { "jp", "11101001",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "jp", "11011010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "c,{L}" }, 
  { "jp", "11010010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "nc,{L}" }, 
     
/*  { "jp", "11111010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "m,{L}" }, */
  { "ld", "11111010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "a,({L})" }, 
     
/*  { "jp", "11110010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "p,{L}" }, */
  { "ld", "11110010",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(FF00+c)" }, 
     
  { "jp", "11001010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "z,{L}" }, 
  { "jp", "11000010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "nz,{L}" }, 
     
/*  { "jp", "11101010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "pe,{L}" }, */
  { "ld", "11101010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "({L}),a" }, 
     
/*  { "jp", "11100010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "po,{L}" }, */
  { "ld", "11100010",
     opFlagsNone, generateOpcodeLR35902, 0, "(FF00+c),a" }, 
     
  { "jr", "00011000OOOOOOOO",
     opFlagsNone, generateOpcodeLR35902, 0, "{O}" }, 
  { "jr", "00111000OOOOOOOO",
     opFlagsNone, generateOpcodeLR35902, 0, "c,{O}" }, 
  { "jr", "00110000OOOOOOOO",
     opFlagsNone, generateOpcodeLR35902, 0, "nc,{O}" }, 
  { "jr", "00101000OOOOOOOO",
     opFlagsNone, generateOpcodeLR35902, 0, "z,{O}" }, 
  { "jr", "00100000OOOOOOOO",
     opFlagsNone, generateOpcodeLR35902, 0, "nz,{O}" }, 
  { "ld", "01111RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "a,{R}" }, 
  { "ld", "00111110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "a,{N}" }, 
  { "ld", "00001010",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(bc)" }, 
  { "ld", "00011010",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(de)" }, 
  { "ld", "01111110",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(hl)" }, 
     
/*  { "ld", "00111010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "a,({L})" }, */
  { "ldd", "00111010",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(hl)" }, 
     
  { "ld", "01000RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "b,{R}" }, 
  { "ld", "00000110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "b,{N}" }, 
  { "ld", "01000110",
     opFlagsNone, generateOpcodeLR35902, 0, "b,(hl)" }, 
  { "ld", "01001RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "c,{R}" }, 
  { "ld", "00001110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "c,{N}" }, 
  { "ld", "01001110",
     opFlagsNone, generateOpcodeLR35902, 0, "c,(hl)" }, 
  { "ld", "01010RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "d,{R}" }, 
  { "ld", "00010110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "d,{N}" }, 
  { "ld", "01010110",
     opFlagsNone, generateOpcodeLR35902, 0, "d,(hl)" }, 
  { "ld", "01011RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "e,{R}" }, 
  { "ld", "00011110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "e,{N}" }, 
  { "ld", "01011110",
     opFlagsNone, generateOpcodeLR35902, 0, "e,(hl)" }, 
  { "ld", "01100RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "h,{R}" }, 
  { "ld", "00100110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "h,{N}" }, 
  { "ld", "01100110",
     opFlagsNone, generateOpcodeLR35902, 0, "h,(hl)" }, 
  { "ld", "01101RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "l,{R}" }, 
  { "ld", "00101110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "l,{N}" }, 
  { "ld", "01101110",
     opFlagsNone, generateOpcodeLR35902, 0, "l,(hl)" }, 
  { "ld", "00000001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "bc,{N}" }, 
  { "ld", "00010001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "de,{N}" }, 
     
/*  { "ld", "00101010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "hl,({L})" }, */
  { "ldi", "00101010",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(hl)" },
     
  { "ld", "00100001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "hl,{N}" }, 
  { "ld", "11111001",
     opFlagsNone, generateOpcodeLR35902, 0, "sp,hl" }, 
  { "ld", "00110001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "sp,{N}" }, 
  { "ld", "01110RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl),{R}" }, 
  { "ld", "00110110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl),{N}" }, 
  { "ld", "00000010",
     opFlagsNone, generateOpcodeLR35902, 0, "(bc),a" }, 
  { "ld", "00010010",
     opFlagsNone, generateOpcodeLR35902, 0, "(de),a" }, 
     
/*  { "ld", "00110010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "({L}),a" }, */
  { "ldd", "00110010",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl),a" }, 
     
/*  { "ld", "00100010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeLR35902, 0, "({L}),hl" }, */
  { "ldi", "00100010",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl),a" },
     
  { "nop", "00000000",
     opFlagsSuspicious, generateOpcodeLR35902, 0, "" }, 
  { "or", "10110RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "or", "11110110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "{N}" }, 
  { "or", "10110110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "pop", "11110001",
     opFlagsNone, generateOpcodeLR35902, 0, "af" }, 
  { "pop", "11000001",
     opFlagsNone, generateOpcodeLR35902, 0, "bc" }, 
  { "pop", "11010001",
     opFlagsNone, generateOpcodeLR35902, 0, "de" }, 
  { "pop", "11100001",
     opFlagsNone, generateOpcodeLR35902, 0, "hl" }, 
  { "push", "11110101",
     opFlagsNone, generateOpcodeLR35902, 0, "af" }, 
  { "push", "11000101",
     opFlagsNone, generateOpcodeLR35902, 0, "bc" }, 
  { "push", "11010101",
     opFlagsNone, generateOpcodeLR35902, 0, "de" }, 
  { "push", "11100101",
     opFlagsNone, generateOpcodeLR35902, 0, "hl" }, 
  { "res", "1100101110BBBRRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{B},{R}" }, 
  { "res", "1100101110BBB110",
     opFlagsNone, generateOpcodeLR35902, 0, "{B},(hl)" }, 
  { "ret", "11001001",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "ret", "11011000",
     opFlagsNone, generateOpcodeLR35902, 0, "c" }, 
  { "ret", "11010000",
     opFlagsNone, generateOpcodeLR35902, 0, "nc" }, 
     
/*  { "ret", "11111000",
     opFlagsNone, generateOpcodeLR35902, 0, "m" }, */
  { "ld", "11111000NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "hl,sp+{N}" }, 
     
/*  { "ret", "11110000",
     opFlagsNone, generateOpcodeLR35902, 0, "p" }, */
  { "ld", "11110000NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "a,(FF00+{N})" }, 
     
  { "ret", "11001000",
     opFlagsNone, generateOpcodeLR35902, 0, "z" }, 
  { "ret", "11000000",
     opFlagsNone, generateOpcodeLR35902, 0, "nz" }, 
     
/*  { "ret", "11101000",
     opFlagsNone, generateOpcodeLR35902, 0, "pe" }, */
  { "add", "11101000NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "sp,{N}" }, 
     
/*  { "ret", "11100000",
     opFlagsNone, generateOpcodeLR35902, 0, "po" }, */
  { "ld", "11100000NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "(FF00+{N}),a" }, 
     
  { "rla", "00010111",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "rl", "1100101100010RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "rl", "1100101100010110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "rlca", "00000111",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "rlc", "1100101100000RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "rlc", "1100101100000110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "rra", "00011111",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "rr", "00011RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "rr", "1100101100011110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "rrca", "00001111",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "rrc", "00001RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "rrc", "1100101100001110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "rst", "11000111",
     opFlagsNone, generateOpcodeLR35902, 0, "$00" }, 
  { "rst", "11001111",
     opFlagsNone, generateOpcodeLR35902, 0, "$08" }, 
  { "rst", "11010111",
     opFlagsNone, generateOpcodeLR35902, 0, "$10" }, 
  { "rst", "11011111",
     opFlagsNone, generateOpcodeLR35902, 0, "$18" }, 
  { "rst", "11100111",
     opFlagsNone, generateOpcodeLR35902, 0, "$20" }, 
  { "rst", "11101111",
     opFlagsNone, generateOpcodeLR35902, 0, "$28" }, 
  { "rst", "11110111",
     opFlagsNone, generateOpcodeLR35902, 0, "$30" }, 
  { "rst", "11111111",
     opFlagsSuspicious, generateOpcodeLR35902, 0, "$38" }, 
  { "sbc", "10011RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "sbc", "11011110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "a,{N}" }, 
  { "sbc", "10011110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "scf", "00110111",
     opFlagsNone, generateOpcodeLR35902, 0, "" }, 
  { "set", "1100101111BBBRRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{B},{R}" }, 
  { "set", "1100101111BBB110",
     opFlagsNone, generateOpcodeLR35902, 0, "{B},(hl)" }, 
  { "sla", "1100101100100RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "sla", "1100101100100110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "sra", "1100101100101RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "sra", "1100101100101110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
     
/*  { "sll", "1100101100110RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "sll", "1100101100110110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, */
  { "swap", "1100101100110RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "swap", "1100101100110110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
     
  { "srl", "1100101100111RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "srl", "1100101100111110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "sub", "10010RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "sub", "11010110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "{N}" }, 
  { "sub", "10010110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }, 
  { "xor", "10101RRR",
     opFlagsNone, generateOpcodeLR35902, 0, "{R}" }, 
  { "xor", "11101110NNNNNNNN",
     opFlagsNone, generateOpcodeLR35902, 0, "{N}" }, 
  { "xor", "10101110",
     opFlagsNone, generateOpcodeLR35902, 0, "(hl)" }
};

const char* registerNamesLR35902[] = {
  "b",
  "c",
  "d",
  "e",
  "h",
  "l",
  "hl",
  "a"
};

const char* getRegisterNameLR35902(int index) {
  return registerNamesLR35902[index];
}

void printRegisterLR35902(int index, String* dst, DismSettings* config) {
/*  dst->catC(dst, "$"); */
  dst->catC(dst, getRegisterNameLR35902(index));
}

void print1bConstantLR35902(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "$%02X");
}

void print2bConstantLR35902(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "$%04X");
}

void printOffsetLR35902(int value, String* dst, DismSettings* config) {
  if (value & 0x80) {
    dst->catInt(dst, 0x100 - value, "-$%02X");
  }
  else {
    dst->catInt(dst, value, "+$%02X");
  }
}

void printBitLR35902(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "%d");
}

void printAddressLR35902(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "$%04X");
}

unsigned int readStepOpcodeLR35902(struct Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args) {
  unsigned int total = strlen(obj->info_->recString) / k_bitsPerByte;
  obj->data_ = args;
  
  return total;
}

OpcodeSimilarity compareRegisterLR35902(int first, int second,
                                      DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

OpcodeSimilarity compareConstantLR35902(int first, int second,
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

OpcodeSimilarity compareAddressLR35902(int first, int second,
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

OpcodeSimilarity compareOffsetLR35902(int first, int second,
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

OpcodeSimilarity compareParameterLR35902(MapSS* argsA, MapSS* argsB,
                            const char* key, DismSettings* config) {
  String stringKey;
  initString(&stringKey);
  
  /* if parameter doesn't exist, ignore it */
  if (argsA->find(argsA, stringKey) == NULL) {
    stringKey.destroy(&stringKey);
    return opcodeSimilaritySame;
  }
  
  stringKey.destroy(&stringKey);
  
  int valueA = getMapSSValueAsBinaryUint(argsA, key);
  int valueB = getMapSSValueAsBinaryUint(argsB, key);
  
  switch (key[0]) {
  case 'R':
    return compareRegisterLR35902(valueA, valueB, config);
    break;
  case 'B':
  case 'N':
    return compareConstantLR35902(valueA, valueB, config);
    break;
  case 'O':
    return compareOffsetLR35902(valueA, valueB, config);
    break;
  case 'L':
    return compareAddressLR35902(valueA, valueB, config);
    break;
  default:
    error("Unknown LR35902 parameter type (compare): ");
    error(key);
    fatal();
    break;
  }
  
  /* suppress compiler warning */
  return opcodeSimilarityNone;
}

OpcodeSimilarity compareParametersLR35902(MapSS* argsA, MapSS* argsB,
                           DismSettings* config) {
  /* compare all possible parameters and return the greatest difference
     among them */
  return greaterOpDifference(
    greaterOpDifference(
      greaterOpDifference(
        greaterOpDifference(
          compareParameterLR35902(argsA, argsB, "R", config),
          compareParameterLR35902(argsA, argsB, "B", config)
        ),
        compareParameterLR35902(argsA, argsB, "N", config)
      ),
      compareParameterLR35902(argsA, argsB, "O", config)
    ),
    compareParameterLR35902(argsA, argsB, "L", config)
  );
}

OpcodeSimilarity compareOpcodeLR35902(Opcode* obj, Opcode* other,
                         DismSettings* config) {
  
  MapSS* argsA = (MapSS*)(obj->data_);
  MapSS* argsB = (MapSS*)(other->data_);
  
  return compareParametersLR35902(argsA, argsB, config);
}

int printParameterLR35902(const char* infoString, int pos, MapSS* args,
                      String* dst, DismSettings* config) {
  int sz = 3;
  
  char type = infoString[pos + 1];
  
  char typestr[2];
  memset(typestr, 0, 2);
  typestr[0] = type;
  
/*  fprintf(stderr, "%s\n", infoString); */
  String valueString = getMapSSValue(args, typestr);
  int value = getMapSSValueAsBinaryUint(args, typestr);
  
  switch (type) {
  /* register */
  case 'R':
    printRegisterLR35902(value, dst, config);
    break;
  /* bit */
  case 'B':
    printBitLR35902(value, dst, config);
    break;
  /* constant */
  case 'N':
    /* check whether 8- or 16-bit */
    if (valueString.size(&valueString) > k_bitsPerByte) {
      print2bConstantLR35902(value, dst, config);
    }
    else {
      print1bConstantLR35902(value, dst, config);
    }
    break;
  /* offset */
  case 'O':
      printOffsetLR35902(value, dst, config);
    break;
  /* address */
  case 'L':
      printAddressLR35902(value, dst, config);
    break;
  default:
    error("Unknown LR35902 parameter type (print): ");
    {
      error(typestr);
    }
    fatal();
    break;
  }
  
  return sz;
}

void printOpcodeLR35902(Opcode* obj, String* dst, DismSettings* config) {
  
  const char* infoString = (const char*)(obj->info(obj)->data);
  int infoStringLen = strlen(infoString);
  
  MapSS* args = (MapSS*)(obj->data_);
  
  int i = 0;
  while (i < infoStringLen) {
    if (infoString[i] == '{') {
      i += printParameterLR35902(infoString, i, args, dst, config);
    }
    else {
      dst->catChar(dst, infoString[i]);
      ++i;
    }
  }
  
}

void generateOpcodeLR35902(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
/*  dst->compare = compareOpcodeLR35902;
  dst->readStep = readStepOpcodeLR35902;
  dst->printString = printOpcodeLR35902; */
  dst->compare = compareOpcodeZ80;
  dst->readStep = readStepOpcodeZ80;
  dst->printString = printOpcodeZ80;
  
  dst->setData(dst, NULL);
  dst->destroy = destructorOpcodeLR35902;
  dst->printName = OpcodeprintNameWithSpace;
}

void destructorOpcodeLR35902(Opcode* obj) {
/*  SubDataLR35902* dat = (SubDataLR35902*)(obj->data_);
  freeMapSS(dat->args); 
  free(dat); */
}

void ModuleLR35902destroyInternal(DismModule* obj) {
/*  freeModDataLR35902((ModDataLR35902*)(obj->data_)); */
}

void initModuleLR35902(DismModule* obj) {
  initDismModule(obj);
  obj->destroyInternal = ModuleLR35902destroyInternal;
/*  obj->byteAlignment_ = 4; */
  obj->reverseReadEndianness_ = 0;
  obj->reverseMatchEndianness_ = 0;
  obj->reverseArgumentBitOrder_ = 0;
  
  OpInfoArray ops = { opcodesLR35902, sizeof(opcodesLR35902) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModuleLR35902() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModuleLR35902(obj);
  return obj;
}

void freeModuleLR35902(DismModule* obj) {
  free(obj);
}
