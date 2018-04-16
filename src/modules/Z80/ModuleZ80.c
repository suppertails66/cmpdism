#include "modules/Z80/ModuleZ80.h"
#include "modules/StringMatcher.h"
#include "modules/OpArgCollator.h"
#include "modules/Consts.h"
#include "util/Logger.h"
#include "util/ByteConv.h"
#include <stdlib.h>
#include <string.h>


/* Forward declaration needed for opcodesZ80 array */
void generateOpcodeZ80(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config);

OpInfo opcodesZ80[] = {
  { "adc", "10001RRR",
     opFlagsNone, generateOpcodeZ80, 0, "a,{R}" }, 
  { "adc", "11001110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,{N}" }, 
  { "adc", "10001110",
     opFlagsNone, generateOpcodeZ80, 0, "a,(hl)" }, 
  { "adc", "1101110110001110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,(ix+{N})" }, 
  { "adc", "1111110110001110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,(iy+{N})" }, 
  { "adc", "1110110101001010",
     opFlagsNone, generateOpcodeZ80, 0, "hl,bc" }, 
  { "adc", "1110110101011010",
     opFlagsNone, generateOpcodeZ80, 0, "hl,de" }, 
  { "adc", "1110110101101010",
     opFlagsNone, generateOpcodeZ80, 0, "hl,hl" }, 
  { "adc", "1110110101111010",
     opFlagsNone, generateOpcodeZ80, 0, "hl,sp" }, 
  { "add", "10000RRR",
     opFlagsNone, generateOpcodeZ80, 0, "a,{R}" }, 
  { "add", "11000110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,{N}" }, 
  { "add", "10000110",
     opFlagsNone, generateOpcodeZ80, 0, "a,(hl)" }, 
  { "add", "1101110110000110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,(ix+{N})" }, 
  { "add", "1111110110000110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,(iy+{N})" }, 
  { "add", "00001001",
     opFlagsNone, generateOpcodeZ80, 0, "hl,bc" }, 
  { "add", "00011001",
     opFlagsNone, generateOpcodeZ80, 0, "hl,de" }, 
  { "add", "00101001",
     opFlagsNone, generateOpcodeZ80, 0, "hl,hl" }, 
  { "add", "00111001",
     opFlagsNone, generateOpcodeZ80, 0, "hl,sp" }, 
  { "add", "1101110100001001",
     opFlagsNone, generateOpcodeZ80, 0, "ix,bc" }, 
  { "add", "1101110100011001",
     opFlagsNone, generateOpcodeZ80, 0, "ix,de" }, 
  { "add", "1101110100101001",
     opFlagsNone, generateOpcodeZ80, 0, "ix,ix" }, 
  { "add", "1101110100111001",
     opFlagsNone, generateOpcodeZ80, 0, "ix,sp" }, 
  { "add", "1111110100001001",
     opFlagsNone, generateOpcodeZ80, 0, "iy,bc" }, 
  { "add", "1111110100011001",
     opFlagsNone, generateOpcodeZ80, 0, "iy,de" }, 
  { "add", "1111110100101001",
     opFlagsNone, generateOpcodeZ80, 0, "iy,iy" }, 
  { "add", "1111110100111001",
     opFlagsNone, generateOpcodeZ80, 0, "iy,sp" }, 
  { "and", "10100RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "and", "11100110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "{N}" }, 
  { "and", "10100110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "and", "1101110110100110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "and", "1111110110100110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "bit", "1100101101BBBRRR",
     opFlagsNone, generateOpcodeZ80, 0, "{B},{R}" }, 
  { "bit", "1100101101BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(hl)" }, 
  { "bit", "1101110110111100NNNNNNNN01BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(ix+{N})" }, 
  { "bit", "1111110110111100NNNNNNNN01BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(iy+{N})" }, 
  { "call", "11001101LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "{L}" }, 
  { "call", "11011100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "c,{L}" }, 
  { "call", "11010100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "nc,{L}" }, 
  { "call", "11111100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "m,{L}" }, 
  { "call", "11110100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "p,{L}" }, 
  { "call", "11001100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "z,{L}" }, 
  { "call", "11000100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "nz,{L}" }, 
  { "call", "11101100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "pe,{L}" }, 
  { "call", "11100100LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "po,{L}" }, 
  { "ccf", "00111111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "cp", "10111RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "cp", "11111110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "{N}" }, 
  { "cp", "10111110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "cp", "1101110110111110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "cp", "1111110110111110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "cpd", "1110110110101001",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "cpdr", "1110110110111001",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "cpi", "1110110110100001",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "cpir", "1110110110110001",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "cpl", "00101111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "daa", "00100111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "dec", "00111101",
     opFlagsNone, generateOpcodeZ80, 0, "a" }, 
  { "dec", "00000101",
     opFlagsNone, generateOpcodeZ80, 0, "b" }, 
  { "dec", "00001101",
     opFlagsNone, generateOpcodeZ80, 0, "c" }, 
  { "dec", "00010101",
     opFlagsNone, generateOpcodeZ80, 0, "d" }, 
  { "dec", "00011101",
     opFlagsNone, generateOpcodeZ80, 0, "e" }, 
  { "dec", "00100101",
     opFlagsNone, generateOpcodeZ80, 0, "h" }, 
  { "dec", "00101101",
     opFlagsNone, generateOpcodeZ80, 0, "l" }, 
  { "dec", "00110101",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "dec", "1101110100110101NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "dec", "1111110100110101NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "dec", "00001011",
     opFlagsNone, generateOpcodeZ80, 0, "bc" }, 
  { "dec", "00011011",
     opFlagsNone, generateOpcodeZ80, 0, "de" }, 
  { "dec", "00101011",
     opFlagsNone, generateOpcodeZ80, 0, "hl" }, 
  { "dec", "00111011",
     opFlagsNone, generateOpcodeZ80, 0, "sp" }, 
  { "dec", "1101110100101011",
     opFlagsNone, generateOpcodeZ80, 0, "ix" }, 
  { "dec", "1111110100101011",
     opFlagsNone, generateOpcodeZ80, 0, "iy" }, 
  { "di", "11110011",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "djnz", "00010000OOOOOOOO",
     opFlagsNone, generateOpcodeZ80, 0, "{O}" }, 
  { "ei", "11111011",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "ex", "11100011",
     opFlagsNone, generateOpcodeZ80, 0, "(sp),hl" }, 
  { "ex", "1101110111100011",
     opFlagsNone, generateOpcodeZ80, 0, "(sp),ix" }, 
  { "ex", "1111110111100011",
     opFlagsNone, generateOpcodeZ80, 0, "(sp),iy" }, 
  { "ex", "00001000",
     opFlagsNone, generateOpcodeZ80, 0, "af,af'" }, 
  { "ex", "11101011",
     opFlagsNone, generateOpcodeZ80, 0, "de,hl" }, 
  { "exx", "11011001",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "halt", "01110110",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "im", "1110110101000110",
     opFlagsNone, generateOpcodeZ80, 0, "0" }, 
  { "im", "1110110101010110",
     opFlagsNone, generateOpcodeZ80, 0, "1" }, 
  { "im", "1110110101011110",
     opFlagsNone, generateOpcodeZ80, 0, "2" }, 
  { "in", "11011011NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,({N})" }, 
  { "in", "1110110101110000",
     opFlagsNone, generateOpcodeZ80, 0, "(c)" }, 
  { "in", "1110110101111000",
     opFlagsNone, generateOpcodeZ80, 0, "a,(c)" }, 
  { "in", "1110110101000000",
     opFlagsNone, generateOpcodeZ80, 0, "b,(c)" }, 
  { "in", "1110110101001000",
     opFlagsNone, generateOpcodeZ80, 0, "c,(c)" }, 
  { "in", "1110110101010000",
     opFlagsNone, generateOpcodeZ80, 0, "d,(c)" }, 
  { "in", "1110110101011000",
     opFlagsNone, generateOpcodeZ80, 0, "e,(c)" }, 
  { "in", "1110110101100000",
     opFlagsNone, generateOpcodeZ80, 0, "h,(c)" }, 
  { "in", "1110110101101000",
     opFlagsNone, generateOpcodeZ80, 0, "l,(c)" }, 
  { "inc", "00111100",
     opFlagsNone, generateOpcodeZ80, 0, "a" }, 
  { "inc", "00000100",
     opFlagsNone, generateOpcodeZ80, 0, "b" }, 
  { "inc", "00001100",
     opFlagsNone, generateOpcodeZ80, 0, "c" }, 
  { "inc", "00010100",
     opFlagsNone, generateOpcodeZ80, 0, "d" }, 
  { "inc", "00011100",
     opFlagsNone, generateOpcodeZ80, 0, "e" }, 
  { "inc", "00100100",
     opFlagsNone, generateOpcodeZ80, 0, "h" }, 
  { "inc", "00101100",
     opFlagsNone, generateOpcodeZ80, 0, "l" }, 
  { "inc", "00000011",
     opFlagsNone, generateOpcodeZ80, 0, "bc" }, 
  { "inc", "00010011",
     opFlagsNone, generateOpcodeZ80, 0, "de" }, 
  { "inc", "00100011",
     opFlagsNone, generateOpcodeZ80, 0, "hl" }, 
  { "inc", "00110011",
     opFlagsNone, generateOpcodeZ80, 0, "sp" }, 
  { "inc", "1101110100100011",
     opFlagsNone, generateOpcodeZ80, 0, "ix" }, 
  { "inc", "1111110100100011",
     opFlagsNone, generateOpcodeZ80, 0, "iy" }, 
  { "inc", "00110100",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "inc", "1101110100110100NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "inc", "1111110100110100NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "ind", "1110110110101010",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "indr", "1110110110111010",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "ini", "1110110110100010",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "inir", "1110110110110010",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "jp", "11000011LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "{L}" }, 
  { "jp", "11101001",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "jp", "1101110111101001",
     opFlagsNone, generateOpcodeZ80, 0, "(ix)" }, 
  { "jp", "1111110111101001",
     opFlagsNone, generateOpcodeZ80, 0, "(iy)" }, 
  { "jp", "11011010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "c,{L}" }, 
  { "jp", "11010010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "nc,{L}" }, 
  { "jp", "11111010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "m,{L}" }, 
  { "jp", "11110010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "p,{L}" }, 
  { "jp", "11001010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "z,{L}" }, 
  { "jp", "11000010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "nz,{L}" }, 
  { "jp", "11101010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "pe,{L}" }, 
  { "jp", "11100010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "po,{L}" }, 
  { "jr", "00011000OOOOOOOO",
     opFlagsNone, generateOpcodeZ80, 0, "{O}" }, 
  { "jr", "00111000OOOOOOOO",
     opFlagsNone, generateOpcodeZ80, 0, "c,{O}" }, 
  { "jr", "00110000OOOOOOOO",
     opFlagsNone, generateOpcodeZ80, 0, "nc,{O}" }, 
  { "jr", "00101000OOOOOOOO",
     opFlagsNone, generateOpcodeZ80, 0, "z,{O}" }, 
  { "jr", "00100000OOOOOOOO",
     opFlagsNone, generateOpcodeZ80, 0, "nz,{O}" }, 
  { "ld", "1110110101000111",
     opFlagsNone, generateOpcodeZ80, 0, "i,a" }, 
  { "ld", "1110110101001111",
     opFlagsNone, generateOpcodeZ80, 0, "r,a" }, 
  { "ld", "1110110101010111",
     opFlagsNone, generateOpcodeZ80, 0, "a,i" }, 
  { "ld", "1110110101011111",
     opFlagsNone, generateOpcodeZ80, 0, "a,r" }, 
  { "ld", "01111RRR",
     opFlagsNone, generateOpcodeZ80, 0, "a,{R}" }, 
  { "ld", "00111110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,{N}" }, 
  { "ld", "00001010",
     opFlagsNone, generateOpcodeZ80, 0, "a,(bc)" }, 
  { "ld", "00011010",
     opFlagsNone, generateOpcodeZ80, 0, "a,(de)" }, 
  { "ld", "01111110",
     opFlagsNone, generateOpcodeZ80, 0, "a,(hl)" }, 
  { "ld", "1101110101111110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,(ix+{N})" }, 
  { "ld", "1111110101111110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,(iy+{N})" }, 
  { "ld", "00111010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "a,({L})" }, 
  { "ld", "01000RRR",
     opFlagsNone, generateOpcodeZ80, 0, "b,{R}" }, 
  { "ld", "00000110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "b,{N}" }, 
  { "ld", "01000110",
     opFlagsNone, generateOpcodeZ80, 0, "b,(hl)" }, 
  { "ld", "1101110101000110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "b,(ix+{N})" }, 
  { "ld", "1111110101000110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "b,(iy+{N})" }, 
  { "ld", "01001RRR",
     opFlagsNone, generateOpcodeZ80, 0, "c,{R}" }, 
  { "ld", "00001110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "c,{N}" }, 
  { "ld", "01001110",
     opFlagsNone, generateOpcodeZ80, 0, "c,(hl)" }, 
  { "ld", "1101110101001110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "c,(ix+{N})" }, 
  { "ld", "1111110101001110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "c,(iy+{N})" }, 
  { "ld", "01010RRR",
     opFlagsNone, generateOpcodeZ80, 0, "d,{R}" }, 
  { "ld", "00010110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "d,{N}" }, 
  { "ld", "01010110",
     opFlagsNone, generateOpcodeZ80, 0, "d,(hl)" }, 
  { "ld", "1101110101010110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "d,(ix+{N})" }, 
  { "ld", "1111110101010110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "d,(iy+{N})" }, 
  { "ld", "01011RRR",
     opFlagsNone, generateOpcodeZ80, 0, "e,{R}" }, 
  { "ld", "00011110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "e,{N}" }, 
  { "ld", "01011110",
     opFlagsNone, generateOpcodeZ80, 0, "e,(hl)" }, 
  { "ld", "1101110101011110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "e,(ix+{N})" }, 
  { "ld", "1111110101011110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "e,(iy+{N})" }, 
  { "ld", "01100RRR",
     opFlagsNone, generateOpcodeZ80, 0, "h,{R}" }, 
  { "ld", "00100110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "h,{N}" }, 
  { "ld", "01100110",
     opFlagsNone, generateOpcodeZ80, 0, "h,(hl)" }, 
  { "ld", "1101110101100110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "h,(ix+{N})" }, 
  { "ld", "1111110101100110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "h,(iy+{N})" }, 
  { "ld", "01101RRR",
     opFlagsNone, generateOpcodeZ80, 0, "l,{R}" }, 
  { "ld", "00101110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "l,{N}" }, 
  { "ld", "01101110",
     opFlagsNone, generateOpcodeZ80, 0, "l,(hl)" }, 
  { "ld", "1101110101101110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "l,(ix+{N})" }, 
  { "ld", "1111110101101110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "l,(iy+{N})" }, 
  { "ld", "1110110101001011LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "bc,({L})" }, 
  { "ld", "00000001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "bc,{N}" }, 
  { "ld", "1110110101011011LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "de,({L})" }, 
  { "ld", "00010001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "de,{N}" }, 
  { "ld", "00101010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "hl,({L})" }, 
  { "ld", "00100001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "hl,{N}" }, 
  { "ld", "1110110101111011LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "sp,({L})" }, 
  { "ld", "11111001",
     opFlagsNone, generateOpcodeZ80, 0, "sp,hl" }, 
  { "ld", "1101110111111001",
     opFlagsNone, generateOpcodeZ80, 0, "sp,ix" }, 
  { "ld", "1111110111111001",
     opFlagsNone, generateOpcodeZ80, 0, "sp,iy" }, 
  { "ld", "00110001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "sp,{N}" }, 
  { "ld", "1101110100101010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "ix,({L})" }, 
  { "ld", "1101110100100001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "ix,{N}" }, 
  { "ld", "1111110100101010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "iy,({L})" }, 
  { "ld", "1111110100100001NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "iy,{N}" }, 
  { "ld", "01110RRR",
     opFlagsNone, generateOpcodeZ80, 0, "(hl),{R}" }, 
  { "ld", "00110110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(hl),{N}" }, 
  { "ld", "00000010",
     opFlagsNone, generateOpcodeZ80, 0, "(bc),a" }, 
  { "ld", "00010010",
     opFlagsNone, generateOpcodeZ80, 0, "(de),a" }, 
  { "ld", "00110010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "({L}),a" }, 
  { "ld", "1110110101000011LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "({L}),bc" }, 
  { "ld", "1110110101010011LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "({L}),de" }, 
  { "ld", "00100010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "({L}),hl" }, 
  { "ld", "1101110100100010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "({L}),ix" }, 
  { "ld", "1111110100100010LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "({L}),iy" }, 
  { "ld", "1110110101110011LLLLLLLLLLLLLLLL",
     opFlagsNone, generateOpcodeZ80, 0, "({L}),sp" }, 
  { "ld", "1101110101110RRRNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N}),{R}" }, 
  { "ld", "1101110100110110NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N}),{N}" }, 
  { "ld", "1111110101110RRRNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N}),{R}" }, 
  { "ld", "1111110100110110NNNNNNNNNNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N}),{N}" }, 
  { "ldd", "1110110110101000",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "lddr", "1110110110111000",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "ldi", "1110110110100000",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "ldir", "1110110110110000",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "neg", "1110110101000100",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "nop", "00000000",
     opFlagsSuspicious, generateOpcodeZ80, 0, "" }, 
  { "or", "10110RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "or", "11110110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "{N}" }, 
  { "or", "10110110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "or", "1101110110110110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "or", "1111110110110110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "out", "11010011NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "({N}),a" }, 
  { "out", "1110110101110001",
     opFlagsNone, generateOpcodeZ80, 0, "(c),0" }, 
  { "out", "1110110101111001",
     opFlagsNone, generateOpcodeZ80, 0, "(c),a" }, 
  { "out", "1110110101000001",
     opFlagsNone, generateOpcodeZ80, 0, "(c),b" }, 
  { "out", "1110110101001001",
     opFlagsNone, generateOpcodeZ80, 0, "(c),c" }, 
  { "out", "1110110101010001",
     opFlagsNone, generateOpcodeZ80, 0, "(c),d" }, 
  { "out", "1110110101011001",
     opFlagsNone, generateOpcodeZ80, 0, "(c),e" }, 
  { "out", "1110110101100001",
     opFlagsNone, generateOpcodeZ80, 0, "(c),h" }, 
  { "out", "1110110101101001",
     opFlagsNone, generateOpcodeZ80, 0, "(c),l" }, 
  { "outd", "1110110110101011",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "otdr", "1110110110111011",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "outi", "1110110110100011",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "otir", "1110110110110011",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "pop", "11110001",
     opFlagsNone, generateOpcodeZ80, 0, "af" }, 
  { "pop", "11000001",
     opFlagsNone, generateOpcodeZ80, 0, "bc" }, 
  { "pop", "11010001",
     opFlagsNone, generateOpcodeZ80, 0, "de" }, 
  { "pop", "11100001",
     opFlagsNone, generateOpcodeZ80, 0, "hl" }, 
  { "pop", "1101110111100001",
     opFlagsNone, generateOpcodeZ80, 0, "ix" }, 
  { "pop", "1111110111100001",
     opFlagsNone, generateOpcodeZ80, 0, "iy" }, 
  { "push", "11110101",
     opFlagsNone, generateOpcodeZ80, 0, "af" }, 
  { "push", "11000101",
     opFlagsNone, generateOpcodeZ80, 0, "bc" }, 
  { "push", "11010101",
     opFlagsNone, generateOpcodeZ80, 0, "de" }, 
  { "push", "11100101",
     opFlagsNone, generateOpcodeZ80, 0, "hl" }, 
  { "push", "1101110111100101",
     opFlagsNone, generateOpcodeZ80, 0, "ix" }, 
  { "push", "1111110111100101",
     opFlagsNone, generateOpcodeZ80, 0, "iy" }, 
  { "res", "1100101110BBBRRR",
     opFlagsNone, generateOpcodeZ80, 0, "{B},{R}" }, 
  { "res", "1100101110BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(hl)" }, 
  { "res", "1101110111001011NNNNNNNN10BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(ix+{N})" }, 
  { "res", "1111110111001011NNNNNNNN10BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(iy+{N})" }, 
  { "ret", "11001001",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "ret", "11011000",
     opFlagsNone, generateOpcodeZ80, 0, "c" }, 
  { "ret", "11010000",
     opFlagsNone, generateOpcodeZ80, 0, "nc" }, 
  { "ret", "11111000",
     opFlagsNone, generateOpcodeZ80, 0, "m" }, 
  { "ret", "11110000",
     opFlagsNone, generateOpcodeZ80, 0, "p" }, 
  { "ret", "11001000",
     opFlagsNone, generateOpcodeZ80, 0, "z" }, 
  { "ret", "11000000",
     opFlagsNone, generateOpcodeZ80, 0, "nz" }, 
  { "ret", "11101000",
     opFlagsNone, generateOpcodeZ80, 0, "pe" }, 
  { "ret", "11100000",
     opFlagsNone, generateOpcodeZ80, 0, "po" }, 
  { "reti", "1110110101001101",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "retn", "1110110101000101",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "rla", "00010111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "rl", "1100101100010RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
/*  { "rl", "1100101100010110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, */
  { "rl", "1101110111001011NNNNNNNN00010110",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "rl", "1111110111001011NNNNNNNN00010110",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "rlca", "00000111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "rlc", "1100101100000RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
/*  { "rlc", "1100101100000110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, */
  { "rlc", "1101110111001011NNNNNNNN00000110",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "rlc", "1111110111001011NNNNNNNN00000110",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "rld", "1110110101101111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "rra", "00011111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "rr", "1100101100011RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
/*  { "rr", "1100101100011110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, */
  { "rr", "1101110111001011NNNNNNNN00011110",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "rr", "1111110111001011NNNNNNNN00011110",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "rrca", "00001111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "rrc", "1100101100001RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
/*  { "rrc", "1100101100001110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, */
  { "rrc", "1101110111001011NNNNNNNN00001110",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "rrc", "1111110111001011NNNNNNNN00001110",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "rrd", "1110110101100111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "rst", "11000111",
     opFlagsNone, generateOpcodeZ80, 0, "$00" }, 
  { "rst", "11001111",
     opFlagsNone, generateOpcodeZ80, 0, "$08" }, 
  { "rst", "11010111",
     opFlagsNone, generateOpcodeZ80, 0, "$10" }, 
  { "rst", "11011111",
     opFlagsNone, generateOpcodeZ80, 0, "$18" }, 
  { "rst", "11100111",
     opFlagsNone, generateOpcodeZ80, 0, "$20" }, 
  { "rst", "11101111",
     opFlagsNone, generateOpcodeZ80, 0, "$28" }, 
  { "rst", "11110111",
     opFlagsNone, generateOpcodeZ80, 0, "$30" }, 
  { "rst", "11111111",
     opFlagsSuspicious, generateOpcodeZ80, 0, "$38" }, 
  { "sbc", "10011RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "sbc", "11011110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,{N}" }, 
  { "sbc", "10011110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "sbc", "1101110110011110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,(ix+{N})" }, 
  { "sbc", "1111110110011110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "a,(iy+{N})" }, 
  { "sbc", "1110110101000010",
     opFlagsNone, generateOpcodeZ80, 0, "hl,bc" }, 
  { "sbc", "1110110101010010",
     opFlagsNone, generateOpcodeZ80, 0, "hl,de" }, 
  { "sbc", "1110110101100010",
     opFlagsNone, generateOpcodeZ80, 0, "hl,hl" }, 
  { "sbc", "1110110101110010",
     opFlagsNone, generateOpcodeZ80, 0, "hl,sp" }, 
  { "scf", "00110111",
     opFlagsNone, generateOpcodeZ80, 0, "" }, 
  { "set", "1100101111BBBRRR",
     opFlagsNone, generateOpcodeZ80, 0, "{B},{R}" }, 
  { "set", "1100101111BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(hl)" }, 
  { "set", "1101110111001011NNNNNNNN11BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(ix+{N})" }, 
  { "set", "1111110111001011NNNNNNNN11BBB110",
     opFlagsNone, generateOpcodeZ80, 0, "{B},(iy+{N})" }, 
  { "sla", "1100101100100RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "sla", "1100101100100110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "sla", "1101110111001011NNNNNNNN00100110",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "sla", "1111110111001011NNNNNNNN00100110",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "sra", "1100101100101RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "sra", "1100101100101110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "sra", "1101110111001011NNNNNNNN00101110",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "sra", "1111110111001011NNNNNNNN00101110",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "sll", "1100101100110RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "sll", "1100101100110110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "sll", "1101110111001011NNNNNNNN00110110",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "sll", "1111110111001011NNNNNNNN00110110",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "srl", "1100101100111RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "srl", "1100101100111110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "srl", "1101110111001011NNNNNNNN00111110",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "srl", "1111110111001011NNNNNNNN00111110",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "sub", "10010RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "sub", "11010110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "{N}" }, 
  { "sub", "10010110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "sub", "1101110110010110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "sub", "1111110110010110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }, 
  { "xor", "10101RRR",
     opFlagsNone, generateOpcodeZ80, 0, "{R}" }, 
  { "xor", "11101110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "{N}" }, 
  { "xor", "10101110",
     opFlagsNone, generateOpcodeZ80, 0, "(hl)" }, 
  { "xor", "1101110110101110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(ix+{N})" }, 
  { "xor", "1111110110101110NNNNNNNN",
     opFlagsNone, generateOpcodeZ80, 0, "(iy+{N})" }
};

const char* registerNamesZ80[] = {
  "b",
  "c",
  "d",
  "e",
  "h",
  "l",
  "(hl)",
  "a"
};

const char* getRegisterNameZ80(int index) {
  return registerNamesZ80[index];
}

void printRegisterZ80(int index, String* dst, DismSettings* config) {
/*  dst->catC(dst, "$"); */
  dst->catC(dst, getRegisterNameZ80(index));
}

void print1bConstantZ80(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "$%02X");
}

void print2bConstantZ80(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "$%04X");
}

void printOffsetZ80(int value, int base, String* dst, DismSettings* config) {
  if (value & 0x80) {
    dst->catInt(dst, 0x100 - value, "-$%02X");
    dst->catInt(dst, base - (0x100 - value) + config->fileLoadAddr, " [$%X]");
  }
  else {
    dst->catInt(dst, value, "+$%02X");
    dst->catInt(dst, value + base + config->fileLoadAddr, " [$%X]");
  }
  
}

void printBitZ80(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "%d");
}

void printAddressZ80(int value, String* dst, DismSettings* config) {
  dst->catInt(dst, value, "$%04X");
}

unsigned int readStepOpcodeZ80(struct Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args) {
  unsigned int total = strlen(obj->info_->recString) / k_bitsPerByte;
  obj->data_ = args;
  
  return total;
}

OpcodeSimilarity compareRegisterZ80(int first, int second,
                                      DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

OpcodeSimilarity compareConstantZ80(int first, int second,
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

OpcodeSimilarity compareAddressZ80(int first, int second,
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

OpcodeSimilarity compareOffsetZ80(int first, int second,
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

OpcodeSimilarity compareParameterZ80(MapSS* argsA, MapSS* argsB,
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
    return compareRegisterZ80(valueA, valueB, config);
    break;
  case 'B':
  case 'N':
    return compareConstantZ80(valueA, valueB, config);
    break;
  case 'O':
    return compareOffsetZ80(valueA, valueB, config);
    break;
  case 'L':
    return compareAddressZ80(valueA, valueB, config);
    break;
  default:
    error("Unknown Z80 parameter type (compare): ");
    error(key);
    fatal();
    break;
  }
  
  /* suppress compiler warning */
  return opcodeSimilarityNone;
}

OpcodeSimilarity compareParametersZ80(MapSS* argsA, MapSS* argsB,
                           DismSettings* config) {
  /* compare all possible parameters and return the greatest difference
     among them */
  return greaterOpDifference(
    greaterOpDifference(
      greaterOpDifference(
        greaterOpDifference(
          compareParameterZ80(argsA, argsB, "R", config),
          compareParameterZ80(argsA, argsB, "B", config)
        ),
        compareParameterZ80(argsA, argsB, "N", config)
      ),
      compareParameterZ80(argsA, argsB, "O", config)
    ),
    compareParameterZ80(argsA, argsB, "L", config)
  );
}

OpcodeSimilarity compareOpcodeZ80(Opcode* obj, Opcode* other,
                         DismSettings* config) {
  
  MapSS* argsA = (MapSS*)(obj->data_);
  MapSS* argsB = (MapSS*)(other->data_);
  
  return compareParametersZ80(argsA, argsB, config);
}

int printParameterZ80(const Opcode* obj,
                      const char* infoString, int pos, MapSS* args,
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
    printRegisterZ80(value, dst, config);
    break;
  /* bit */
  case 'B':
    printBitZ80(value, dst, config);
    break;
  /* constant */
  case 'N':
    /* check whether 8- or 16-bit */
    if (valueString.size(&valueString) > k_bitsPerByte) {
      print2bConstantZ80(reverseEndianness(value, 2), dst, config);
    }
    else {
      print1bConstantZ80(value, dst, config);
    }
    break;
  /* offset */
  case 'O':
      printOffsetZ80(value, obj->pos_ + 2, dst, config);
    break;
  /* address */
  case 'L':
      printAddressZ80(reverseEndianness(value, 2), dst, config);
    break;
  default:
    error("Unknown Z80 parameter type (print): ");
    error(typestr);
    fatal();
    break;
  }
  
  return sz;
}

void printOpcodeZ80(Opcode* obj, String* dst, DismSettings* config) {
  
  const char* infoString = (const char*)(obj->info(obj)->data);
  int infoStringLen = strlen(infoString);
  
  MapSS* args = (MapSS*)(obj->data_);
  
  int i = 0;
  while (i < infoStringLen) {
    if (infoString[i] == '{') {
      i += printParameterZ80(obj, infoString, i, args, dst, config);
    }
    else {
      dst->catChar(dst, infoString[i]);
      ++i;
    }
  }
  
}

void generateOpcodeZ80(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareOpcodeZ80;
  dst->readStep = readStepOpcodeZ80;
  dst->printString = printOpcodeZ80;
  
  dst->setData(dst, NULL);
  dst->destroy = destructorOpcodeZ80;
  dst->printName = OpcodeprintNameWithSpace;
}

void destructorOpcodeZ80(Opcode* obj) {
/*  SubDataZ80* dat = (SubDataZ80*)(obj->data_);
  freeMapSS(dat->args); 
  free(dat); */
}

void ModuleZ80destroyInternal(DismModule* obj) {
/*  freeModDataZ80((ModDataZ80*)(obj->data_)); */
}

void initModuleZ80(DismModule* obj) {
  initDismModule(obj);
  obj->destroyInternal = ModuleZ80destroyInternal;
/*  obj->byteAlignment_ = 4; */
  obj->reverseReadEndianness_ = 0;
  obj->reverseMatchEndianness_ = 0;
  obj->reverseArgumentBitOrder_ = 0;
  
  OpInfoArray ops = { opcodesZ80, sizeof(opcodesZ80) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModuleZ80() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModuleZ80(obj);
  return obj;
}

void freeModuleZ80(DismModule* obj) {
  free(obj);
}
