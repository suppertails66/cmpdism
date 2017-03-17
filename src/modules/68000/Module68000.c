#include "modules/68000/Module68000.h"
#include "modules/StringMatcher.h"
#include "modules/OpArgCollator.h"
#include "modules/Consts.h"
#include "util/Logger.h"
#include "util/ByteConv.h"
#include <stdlib.h>
#include <string.h>

/*typedef enum OpTarget68000 {
  
  opTarget68000Constant,
  opTarget68000Data3,
  opTarget68000Data8,
  opTarget68000CCC,
  opTarget68000Data16As,
  opTarget68000Dn,
  opTarget68000Data3,
  
  
} OpTarget68000; */

/* Forward declaration needed for opcodes68000 array */
void generateOpcode68000(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config);

OpInfo opcodes68000[] = {
  { "ori.b", "0000000000111100xxxxxxxxdddddddd",
    opFlagsSuspicious, generateOpcode68000, 0, "#db,ccr" },
  { "ori.b", "0000000000aaaaaaxxxxxxxxdddddddd",
    opFlagsSuspicious, generateOpcode68000, 0, "#db,a" },
  { "ori.w", "0000000001aaaaaadddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dw,a" },
  { "ori.l", "0000000010aaaaaadddddddddddddddddddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dl,a" },
  { "andi.b", "0000001000111100xxxxxxxxdddddddd",
    opFlagsNone, generateOpcode68000, 0, "#db,ccr" },
  { "andi.b", "0000001000aaaaaaxxxxxxxxdddddddd",
    opFlagsNone, generateOpcode68000, 0, "#db,a" },
  { "andi.w", "0000001001aaaaaadddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dw,a" },
  { "andi.l", "0000001010aaaaaadddddddddddddddddddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dl,a" },
  { "subi.b", "0000010000aaaaaaxxxxxxxxdddddddd",
    opFlagsNone, generateOpcode68000, 0, "#db,a" },
  { "subi.w", "0000010001aaaaaadddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dw,a" },
  { "subi.l", "0000010010aaaaaadddddddddddddddddddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dl,a" },
  { "addi.b", "0000011000aaaaaaxxxxxxxxdddddddd",
    opFlagsNone, generateOpcode68000, 0, "#db,a" },
  { "addi.w", "0000011001aaaaaadddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dw,a" },
  { "addi.l", "0000011010aaaaaadddddddddddddddddddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dl,a" },
  { "btst", "0000100000aaaaaaxxxxxxxxxxxbbbbb",
    opFlagsNone, generateOpcode68000, 0, "#b,a" },
  { "bchg", "0000100001aaaaaaxxxxxxxxxxxbbbbb",
    opFlagsNone, generateOpcode68000, 0, "#b,a" },
  { "bclr", "0000100010aaaaaaxxxxxxxxxxxbbbbb",
    opFlagsNone, generateOpcode68000, 0, "#b,a" },
  { "bset", "0000100011aaaaaaxxxxxxxxxxxbbbbb",
    opFlagsNone, generateOpcode68000, 0, "#b,a" },
  { "eori.b", "0000101000111100xxxxxxxxdddddddd",
    opFlagsNone, generateOpcode68000, 0, "#db,ccr" },
  { "eori.b", "0000101000aaaaaaxxxxxxxxdddddddd",
    opFlagsNone, generateOpcode68000, 0, "#db,a" },
  { "eori.w", "0000101001aaaaaadddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dw,a" },
  { "eori.l", "0000101010aaaaaadddddddddddddddddddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dl,a" },
    
  { "cmpi.b", "0000110000aaaaaaxxxxxxxxdddddddd",
    opFlagsNone, generateOpcode68000, 0, "#db,a" },
  { "cmpi.w", "0000110001aaaaaadddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dw,a" },
  { "cmpi.l", "0000110010aaaaaadddddddddddddddddddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "#dw,a" },
    
  { "movep.w", "0000DDD100001SSSdddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "data16(AS),DD" },
  { "movep.l", "0000DDD101001SSSdddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "data16(AS),DD" },
  { "movep.w", "0000SSS110001DDDdddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "DS,data16(AD)" },
  { "movep.l", "0000SSS111001DDDdddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "DS,data16(AD)" },
  { "btst", "0000SSS100aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "bchg", "0000SSS101aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "bclr", "0000SSS110aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "bset", "0000SSS111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "move.b", "0001ffffffeeeeee",
    opFlagsNone, generateOpcode68000, 0, "ae,af" },
  { "movea.l", "0010DDD001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "move.l", "0010ffffffeeeeee",
    opFlagsNone, generateOpcode68000, 0, "ae,af" },
  { "movea.w", "0011DDD001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "move.w", "0011ffffffeeeeee",
    opFlagsNone, generateOpcode68000, 0, "ae,af" },
  { "negx.b", "0100000000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "negx.w", "0100000001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "negx.l", "0100000010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "clr.b", "0100001000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "clr.w", "0100001001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "clr.l", "0100001010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "neg.b", "0100010000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "neg.w", "0100010001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "neg.l", "0100010010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "move", "0100010011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,ccr" },
  { "not.b", "0100011000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "not.w", "0100011001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "not.l", "0100011010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "nbcd", "0100100000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "swap", "0100100001000DDD",
    opFlagsNone, generateOpcode68000, 0, "DD" },
  { "pea", "0100100001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "ext.w", "0100100010000DDD",
    opFlagsNone, generateOpcode68000, 0, "DD" },
  { "ext.l", "0100100011000DDD",
    opFlagsNone, generateOpcode68000, 0, "DD" },
  { "movem.w", "0100100010aaaaaaqqqqqqqqrrrrrrrr",
    opFlagsNone, generateOpcode68000, 0, "reg-list,a" },
  { "movem.l", "0100100011aaaaaaqqqqqqqqrrrrrrrr",
    opFlagsNone, generateOpcode68000, 0, "reg-list,a" },
  { "tst.b", "0100101000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "tst.w", "0100101001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "tst.l", "0100101010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "illegal", "0100101011111100",
    opFlagsNone, generateOpcode68000, 0, "" },
  { "tas", "0100101011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "movem.w", "0100110010aaaaaaqqqqqqqqrrrrrrrr",
    opFlagsNone, generateOpcode68000, 0, "a,reg-list" },
  { "movem.l", "0100110011aaaaaaqqqqqqqqrrrrrrrr",
    opFlagsNone, generateOpcode68000, 0, "a,reg-list" },
  { "trap", "010011100100vvvv",
    opFlagsNone, generateOpcode68000, 0, "#vector" },
  { "link", "0100111001010SSSdddddddddddddddd",
    opFlagsNone, generateOpcode68000, 0, "AS,#dw" },
  { "unlk", "0100111001011DDD",
    opFlagsNone, generateOpcode68000, 0, "AD" },
  { "nop", "0100111001110001",
    opFlagsNone, generateOpcode68000, 0, "" },
  { "rts", "0100111001110101",
    opFlagsNone, generateOpcode68000, 0, "" },
  { "trapv", "0100111001110110",
    opFlagsNone, generateOpcode68000, 0, "" },
  { "rtr", "0100111001110111",
    opFlagsNone, generateOpcode68000, 0, "" },
  { "chk.w", "0100DDD110aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "lea", "0100DDD111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "jsr", "0100111010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "jmp", "0100111011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "addq.b", "0101DDD000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "#data3,a" },
  { "addq.w", "0101DDD001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "#data3,a" },
  { "addq.l", "0101DDD010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "#data3,a" },
  { "subq.b", "0101DDD100aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "#data3,a" },
  { "subq.w", "0101DDD101aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "#data3,a" },
  { "subq.l", "0101DDD110aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "#data3,a" },
  { "dba", "0101000011001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbn", "0101000111001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbhi", "0101001011001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbls", "0101001111001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbcc", "0101010011001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbcs", "0101010111001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbne", "0101011011001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbeq", "0101011111001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbvc", "0101100011001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbvs", "0101100111001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbpl", "0101101011001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbmi", "0101101111001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbge", "0101110011001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dblt", "0101110111001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dbgt", "0101111011001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "dble", "0101111111001SSSllllllllllllllll",
    opFlagsNone, generateOpcode68000, 0, "DS,label" },
  { "sa", "0101000011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "sn", "0101000111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "shi", "0101001011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "sls", "0101001111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "scc", "0101010011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "scs", "0101010111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "sne", "0101011011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "seq", "0101011111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "svc", "0101100011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "svs", "0101100111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "spl", "0101101011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "smi", "0101101111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "sge", "0101110011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "slt", "0101110111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "sgt", "0101111011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "sle", "0101111111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "bra", "01100000llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bsr", "01100001llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "ba", "01100000llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bn", "01100001llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bhi", "01100010llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bls", "01100011llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bcc", "01100100llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bcs", "01100101llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bne", "01100110llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "beq", "01100111llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bvc", "01101000llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bvs", "01101001llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bpl", "01101010llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bmi", "01101011llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bge", "01101100llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "blt", "01101101llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "bgt", "01101110llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "ble", "01101111llllllll",
    opFlagsNone, generateOpcode68000, 0, "label" },
  { "moveq", "0111DDD0dddddddd",
    opFlagsNone, generateOpcode68000, 0, "#data8,DD" },
  { "or.b", "1000DDD000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "or.w", "1000DDD001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "or.l", "1000DDD010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "divu.w", "1000DDD011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "sbcd", "1000DDD100000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "sbcd", "1000DDD100001SSS",
    opFlagsNone, generateOpcode68000, 0, "-(AS),-(AD)" },
  { "or.b", "1000SSS100aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "or.w", "1000SSS101aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "or.l", "1000SSS110aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "divs.w", "1000DDD111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "subx.b", "1001DDD100000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "subx.w", "1001DDD101000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "subx.l", "1001DDD110000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "subx.b", "1001DDD100001SSS",
    opFlagsNone, generateOpcode68000, 0, "-(AS),-(AD)" },
  { "subx.w", "1001DDD101001SSS",
    opFlagsNone, generateOpcode68000, 0, "-(AS),-(AD)" },
  { "subx.l", "1001DDD110001SSS",
    opFlagsNone, generateOpcode68000, 0, "-(AS),-(AD)" },
  { "sub.b", "1001DDD000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "sub.w", "1001DDD001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "sub.l", "1001DDD010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "sub.b", "1001SSS100aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "sub.w", "1001SSS101aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "sub.l", "1001SSS110aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "suba.w", "1001DDD011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "suba.l", "1001DDD111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "cmpm.b", "1011DDD100000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "cmpm.w", "1011DDD101000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "cmpm.l", "1011DDD110000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "cmpm.b", "1011DDD100001SSS",
    opFlagsNone, generateOpcode68000, 0, "(AS)+,(AD)+" },
  { "cmpm.w", "1011DDD101001SSS",
    opFlagsNone, generateOpcode68000, 0, "(AS)+,(AD)+" },
  { "cmpm.l", "1011DDD110001SSS",
    opFlagsNone, generateOpcode68000, 0, "(AS)+,(AD)+" },
  { "cmp.b", "1011DDD000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "cmp.w", "1011DDD001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "cmp.l", "1011DDD010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "eor.b", "1011DDD100aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "eor.w", "1011DDD101aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "eor.l", "1011DDD110aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "cmpa.w", "1011DDD011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "cmpa.l", "1011DDD111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "abcd", "1100DDD100000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "abcd", "1100DDD100001SSS",
    opFlagsNone, generateOpcode68000, 0, "-(AS),-(AD)" },
  { "exg", "1100DDD101000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "exg", "1100DDD101001SSS",
    opFlagsNone, generateOpcode68000, 0, "AS,AD" },
  { "exg", "1100DDD110001SSS",
    opFlagsNone, generateOpcode68000, 0, "AS,DD" },
  { "and.b", "1100DDD000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "and.w", "1100DDD001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "and.l", "1100DDD010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "mulu.w", "1100DDD011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "and.b", "1100SSS100aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "and.w", "1100SSS101aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "and.l", "1100SSS110aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "muls.w", "1100DDD111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "addx.b", "1101DDD100000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "addx.w", "1101DDD101000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "addx.l", "1101DDD110000SSS",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "addx.b", "1101DDD100001SSS",
    opFlagsNone, generateOpcode68000, 0, "-(AS),-(AD)" },
  { "addx.w", "1101DDD101001SSS",
    opFlagsNone, generateOpcode68000, 0, "-(AS),-(AD)" },
  { "addx.l", "1101DDD110001SSS",
    opFlagsNone, generateOpcode68000, 0, "-(AS),-(AD)" },
  { "add.b", "1101DDD000aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "add.w", "1101DDD001aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "add.l", "1101DDD010aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,DD" },
  { "add.b", "1101SSS100aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "add.w", "1101SSS101aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "add.l", "1101SSS110aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "DS,a" },
  { "adda.w", "1101DDD011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "adda.l", "1101DDD111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a,AD" },
  { "asrd.b", "1110ccc000000DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "asrd.w", "1110ccc001000DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "asrd.l", "1110ccc010000DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "asld.b", "1110ccc100000DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "asld.w", "1110ccc101000DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "asld.l", "1110ccc110000DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "lsrd.b", "1110ccc000001DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "lsrd.w", "1110ccc001001DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "lsrd.l", "1110ccc010001DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "lsld.b", "1110ccc100001DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "lsld.w", "1110ccc101001DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "lsld.l", "1110ccc110001DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "roxrd.b", "1110ccc000010DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "roxrd.w", "1110ccc001010DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "roxrd.l", "1110ccc010010DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "roxld.b", "1110ccc100010DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "roxld.w", "1110ccc101010DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "roxld.l", "1110ccc110010DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "rord.b", "1110ccc000011DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "rord.w", "1110ccc001011DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "rord.l", "1110ccc010011DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "rold.b", "1110ccc100011DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "rold.w", "1110ccc101011DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "rold.l", "1110ccc110011DDD",
    opFlagsNone, generateOpcode68000, 0, "#ccc,DD" },
  { "asrd.b", "1110SSS000100DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "asrd.w", "1110SSS001100DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "asrd.l", "1110SSS010100DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "asld.b", "1110SSS100100DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "asld.w", "1110SSS101100DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "asld.l", "1110SSS110100DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "lsrd.b", "1110SSS000101DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "lsrd.w", "1110SSS001101DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "lsrd.l", "1110SSS010101DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "lsld.b", "1110SSS100101DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "lsld.w", "1110SSS101101DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "lsld.l", "1110SSS110101DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "roxrd.b", "1110SSS000110DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "roxrd.w", "1110SSS001110DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "roxrd.l", "1110SSS010110DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "roxld.b", "1110SSS100110DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "roxld.w", "1110SSS101110DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "roxld.l", "1110SSS110110DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "rord.b", "1110SSS000111DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "rord.w", "1110SSS001111DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "rord.l", "1110SSS010111DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "rold.b", "1110SSS100111DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "rold.w", "1110SSS101111DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "rold.l", "1110SSS110111DDD",
    opFlagsNone, generateOpcode68000, 0, "DS,DD" },
  { "asrd", "1110000011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "asld", "1110000111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "lsrd", "1110001011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "lsld", "1110001111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "roxrd", "1110010011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "roxld", "1110010111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "rord", "1110011011aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" },
  { "rold", "1110011111aaaaaa",
    opFlagsNone, generateOpcode68000, 0, "a" }
};

const char* opTarget68000Names[] = {
  "XXXXX",
  "#db",
  "#dw",
  "#dl",
  "#b",
  "#data3",
  "#data8",
  "#vector",
  "#ccc",
  "ccr",
  "data16(AS)",
  "data16(AD)",
  "DS",
  "DD",
  "a",
  "ae",
  "af",
  "AS",
  "AD",
  "reg-list",
  "-(AS)",
  "(AS)+",
  "-(AD)",
  "(AD)+",
  "label"
};

const int numOpTargets68000
  = sizeof(opTarget68000Names) / sizeof(const char*);

OpTarget68000 opNameToType68000(const char* name) {
  int i = 0;
  for (i = 0; i < numOpTargets68000; i++) {
    if (strcmp(name, opTarget68000Names[i]) == 0) return (OpTarget68000)i;
  }
  
  error("Invalid op name: ");
  error(name);
  error("\n");
  fatal();
  
  /* suppress compiler warning */
  return (OpTarget68000)0;
}

int findCommaPos(const char* argString) {
  int len = strlen(argString);

  int i;
  for (i = 0; i < len; i++) {
    if (argString[i] == ',') return i;
  }
  
  return 0;
}

OpTarget68000 srcArgType68000(const char* argString) {
  int len = strlen(argString);
  if (len == 0) return opTarget68000None;
  
  int splitPoint = findCommaPos(argString);
  
  String result;
  initString(&result);
  result.catData(&result, argString, splitPoint);
  
  if (result.size(&result) == 0) {
    result.destroy(&result);
    return opTarget68000None;
  }
  
  OpTarget68000 resultType = opNameToType68000(result.cStr(&result));
  
  result.destroy(&result);
  
  return resultType;
}

OpTarget68000 dstArgType68000(const char* argString) {
  int len = strlen(argString);
  if (len == 0) return opTarget68000None;
  
  int splitPoint = findCommaPos(argString);
  
  String result;
  initString(&result);
  /* If only one argument is present, it must be a destination */
  if (splitPoint == 0) {
    result.catData(&result, argString, strlen(argString));
  }
  else {
    result.catData(&result, argString + splitPoint + 1,
                    strlen(argString) - splitPoint - 1);
  }
  
  OpTarget68000 resultType = opNameToType68000(result.cStr(&result));
  
  result.destroy(&result);
  
  return resultType;
}

SubOps68000* allocSubOps68000() {
  SubOps68000* obj = malloc(sizeof(SubOps68000));
  memset(obj, 0, sizeof(SubOps68000));
  return obj;
}

void freeSubOps68000(SubOps68000* obj) {
  if (obj->args != NULL) {
    freeMapSS(obj->args);
  }
  
  if (obj->addressA != NULL) {
    freeMapSS(obj->addressA);
  }
  
  if (obj->addressE != NULL) {
    freeMapSS(obj->addressE);
  }
  
  if (obj->addressF != NULL) {
    freeMapSS(obj->addressF);
  }

  free(obj);
}

void destructorSubOps68000Opcode(Opcode* obj) {
  freeSubOps68000(obj->data_);
}

AddressMode68000 getAddressMode(const char* addrModeString) {
  switch (binaryStringToUint(addrModeString, 3)) {
  case 0x0: /* Dn */
    return addressMode68000Dn;
    break;
  case 0x1: /* An */
    return addressMode68000An;
    break;
  case 0x2: /* (An) */
    return addressMode68000AnInd;
    break;
  case 0x3: /* (An)+ */
    return addressMode68000AnIndInc;
    break;
  case 0x4: /* -(An) */
    return addressMode68000AnIndDec;
    break;
  case 0x5: /* (E16, An) */
    return addressMode68000AnOff16;
    break;
  case 0x6: /* (E8,An,Ri.z) */
    return addressMode68000AnOff8;
    break;
  case 0x7:
    switch (binaryStringToInt(addrModeString + 3, 3)) {
    case 0x0: /* addr16 */
      return addressMode68000Addr16;
      break;
    case 0x1: /* addr32 */
      return addressMode68000Addr32;
      break;
    case 0x2: /* E16(PC) */
      return addressMode68000PC16;
      break;
    case 0x3: /* E8(PC,Ri.z) */
      return addressMode68000PC8;
      break;
    case 0x4: /* Immediate/implied */
      return addressMode68000Immed;
      break;
    default:  /* Not an address */
      return addressMode68000Invalid;
      break;
    }
    break;
  default:  /* Not an address */
    return addressMode68000Invalid;
    break;
  }
}

AddressMode68000Info addressMode68000DnInfo
  = { addressMode68000Dn, 0, "" };
AddressMode68000Info addressMode68000AnInfo
  = { addressMode68000An, 0, "" };
AddressMode68000Info addressMode68000AnIndInfo
  = { addressMode68000AnInd, 0, "" };
AddressMode68000Info addressMode68000AnIndIncInfo
  = { addressMode68000AnIndInc, 0, "" };
AddressMode68000Info addressMode68000AnIndDecInfo
  = { addressMode68000AnIndDec, 0, "" };
AddressMode68000Info addressMode68000AnOff16Info
  = { addressMode68000AnOff16, 2, "EEEEEEEEEEEEEEEE" };
AddressMode68000Info addressMode68000AnOff8Info
  = { addressMode68000AnOff8, 2, "iiiiz000EEEEEEEE" };
AddressMode68000Info addressMode68000Addr16Info
  = { addressMode68000Addr16, 2, "EEEEEEEEEEEEEEEE" };
AddressMode68000Info addressMode68000Addr32Info
  = { addressMode68000Addr32, 4, "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" };
AddressMode68000Info addressMode68000PC16Info
  = { addressMode68000PC16, 2, "EEEEEEEEEEEEEEEE" };
AddressMode68000Info addressMode68000PC8Info
  = { addressMode68000PC8, 2, "iiiiz000EEEEEEEE" };
AddressMode68000Info addressMode68000ImmedInfo
  = { addressMode68000Immed, 0, "" };

AddressMode68000Info* getAddressModeInfo(const char* addrModeString) {
/*    fprintf(stderr, "getAddressModeInfo: %s %d\n", addrModeString,
              binaryStringToUint(addrModeString, 3)); */

  switch (binaryStringToUint(addrModeString, 3)) {
  case 0x0: /* Dn */
    return &addressMode68000DnInfo;
    break;
  case 0x1: /* An */
    return &addressMode68000AnInfo;
    break;
  case 0x2: /* (An) */
    return &addressMode68000AnIndInfo;
    break;
  case 0x3: /* (An)+ */
    return &addressMode68000AnIndIncInfo;
    break;
  case 0x4: /* -(An) */
    return &addressMode68000AnIndDecInfo;
    break;
  case 0x5: /* (E16, An) */
    return &addressMode68000AnOff16Info;
    break;
  case 0x6: /* (E8,An,Ri.z) */
    return &addressMode68000AnOff8Info;
    break;
  case 0x7:
    switch (binaryStringToUint(addrModeString + 3, 3)) {
    case 0x0: /* addr16 */
      return &addressMode68000Addr16Info;
      break;
    case 0x1: /* addr32 */
      return &addressMode68000Addr32Info;
      break;
    case 0x2: /* E16(PC) */
      return &addressMode68000PC16Info;
      break;
    case 0x3: /* E8(PC,Ri.z) */
      return &addressMode68000PC8Info;
      break;
    case 0x4: /* Immediate/implied */
      return &addressMode68000ImmedInfo;
      break;
    default:  /* Not an address */
      return NULL;
      break;
    }
    break;
  default:  /* Not an address */
    return NULL;
    break;
  }
}

int verifyRemainingSpace(BufferStream* stream, int pos, int amount) {
  if ((stream->size(stream)) - pos < amount) return 0;
  
  return 1;
}

/* Return struct for read68000Address() */
typedef struct Address68000Struct {

  int success;
  MapSS* addr;
  int addrSize;
  AddressMode68000Info* addrInfo;

} Address68000Struct;

Address68000Struct read68000Address(String addrModeString,
                                    BufferStream* stream, int pos,
                                    DismSettings* config,
                                    OpInfo* opInfo,
                                    int isDestination) {
  Address68000Struct result;
  result.addr = NULL;
  result.addrSize = 0;
  result.success = 0;
  
  /* Sanity check (address mode must be 6 bits) */
  if (addrModeString.size(&addrModeString) < 6) return result;
  
  /* Get information for address mode */
  AddressMode68000Info* addrInfo = getAddressModeInfo(
      addrModeString.cStr(&addrModeString));
  result.addrInfo = addrInfo;
  
/*  fprintf(stderr, "%s\n", addrModeString.cStr(&addrModeString)); */
  
  /* Ensure address type is valid */
  if ((addrInfo == NULL)
      || (addrInfo->mode == addressMode68000Invalid)) return result;
  
  result.addrSize = addrInfo->size;
  
  /* Contend with immediate mode.
     Since I didn't realize how this worked until I'd already written
     all the code, it's an awful hack. */
  int immedSize = 0;
  if ((addrInfo->mode == addressMode68000Immed)) {
    if (isDestination) {
      /* immediate mode destination = 32-bit literal address? */
      immedSize = 4;
    }
    else {
      char mode = opInfo->name[strlen(opInfo->name) - 1];
      
      switch (mode) {
      /* word */
      case 'w':
        immedSize = 2;
        break;
      /* long */
      case 'l':
        immedSize = 4;
        break;
      /* assume byte mode if 'b' or data size is not explicitly specified */
      default:
        immedSize = 2;
        break;
      }
    }
    
    result.addrSize += immedSize;
  }
  
  /* Make sure enough space remains to read the given address type */
  if (!verifyRemainingSpace(stream, pos, result.addrSize)) return result;
  
  /* Create map to hold result data */
  result.addr = allocMapSS();

  /* Put a copy of the address mode string in the map so we can check it
     later if needed.
     Separate into "a" (first 3 digits) and "n" (last 3 digits) */
     
  String addrModeCopy1;
  String addrModeCopy2;
  String keyString1;
  String keyString2;
  initString(&addrModeCopy1);
  initString(&addrModeCopy2);
  initString(&keyString1);
  initString(&keyString2);
  
  addrModeCopy1.catData(&addrModeCopy1,
                        addrModeString.cStr(&addrModeString) + 0,
                        3);
  addrModeCopy2.catData(&addrModeCopy2,
                        addrModeString.cStr(&addrModeString) + 3,
                        3);
  keyString1.catC(&keyString1, "a");
  keyString2.catC(&keyString2, "n");
  result.addr->insert(result.addr, keyString1, addrModeCopy1);
  result.addr->insert(result.addr, keyString2, addrModeCopy2);
  
  
  /* Move stream to data position */
  int curPos = stream->tell(stream);
  stream->seek(stream, pos);
  
  /* More immediate mode hacks */
  if ((addrInfo->mode == addressMode68000Immed)) {
    switch (immedSize) {
    case 2:
      collateOpArgs(stream, result.addr, "EEEEEEEEEEEEEEEE");
      break;
    case 4:
      collateOpArgs(stream, result.addr, "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
      break;
    /* assume byte */
    default:
      collateOpArgs(stream, result.addr, "xxxxxxxxEEEEEEEE");
      break;
    }
  }
  else {
    /* Read the appropriate data */
    collateOpArgs(stream, result.addr, addrInfo->argStr);
  }
  
  /* Move stream back to original position
     (not at all necessary, and in fact might be preferable to an explicit
     return value, but we're playing it safe) */
  stream->seek(stream, curPos);
  
  /* Indicate successful read (again, unnecessary but "safe") */
  result.success = 1;
  
  return result;
}

void print680001bOffset(int value, String* dst,
                        DismSettings* config) {
  if (value < 0) {
    dst->catInt(dst, -value, "-$%02X");
  }
  else {
    dst->catInt(dst, value, "+$%02X");
  }
}

void print680002bOffset(unsigned int value, String* dst,
                        DismSettings* config) {
  if (value < 0) {
    dst->catInt(dst, -value, "-$%04X");
  }
  else {
    dst->catInt(dst, value, "+$%04X");
  }
}

void print680002bAddress(unsigned int value, String* dst,
                        DismSettings* config) {
  /* sign-extend */
  if (value & 0x8000) {
    dst->catInt(dst, value, "$FF%04X");
  }
  else {
    dst->catInt(dst, value, "$00%04X");
  }
}

void print680004bAddress(int value, String* dst,
                        DismSettings* config) {
  dst->catInt(dst, value, "$%08X");
}

void print68000Data3Constant(unsigned int value, String* dst,
                        DismSettings* config) {
  if (value == 0) dst->catInt(dst, 8, "#$%02X");
  else dst->catInt(dst, value, "#$%02X");
}

void print680001bConstant(unsigned int value, String* dst,
                        DismSettings* config) {
  dst->catInt(dst, value, "#$%02X");
}

void print680002bConstant(unsigned int value, String* dst,
                        DismSettings* config) {
  dst->catInt(dst, value, "#$%04X");
}

void print680004bConstant(int value, String* dst,
                        DismSettings* config) {
  dst->catInt(dst, value, "#$%08X");
}

OpcodeSimilarity compare680002bAddress(int first, int second,
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

OpcodeSimilarity compare680004bAddress(int first, int second,
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

OpcodeSimilarity compare680001bOffset(int first, int second,
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

OpcodeSimilarity compare680002bOffset(int first, int second,
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

OpcodeSimilarity compare680001bConstant(int first, int second,
                        DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

OpcodeSimilarity compare680002bConstant(int first, int second,
                        DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

OpcodeSimilarity compare680004bConstant(int first, int second,
                        DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

OpcodeSimilarity compare68000RegisterIndex(int first, int second,
                        DismSettings* config) {
  if (first == second) {
    return opcodeSimilaritySame;
  }
  else {
    return opcodeSimilarityDistinct;
  }
}

void print68000AddressFromMode(MapSS* addrArgs,
                               AddressMode68000Info* addrInfo,
                               String* dst,
                        DismSettings* config) {
  
/*  addressMode68000Dn,
  addressMode68000An,
  addressMode68000AnInd,
  addressMode68000AnIndInc,
  addressMode68000AnIndDec,
  addressMode68000AnOff16,
  addressMode68000AnOff8,
  addressMode68000Addr16,
  addressMode68000Addr32,
  addressMode68000PC16,
  addressMode68000PC8,
  addressMode68000Immed,
  addressMode68000Invalid */
  
  /* Retrieve the address field's "n" parameter (low 3 bits of address code) */
/*  String nStr;
  initString(&nStr);
  nStr.catC(&nStr, "n");
  String value = addrArgs->find(addrArgs, nStr)->value;
  int n = binaryStringToInt(value.cStr(&value), value.size(&value));
  nStr.destroy(&nStr); */
  
/*  fprintf(stderr, "size: %d\n", addrArgs->size_); */
  
/*  int n = getMapSSValueAsBinaryInt(addrArgs, "n"); */
  /* lazy */
  unsigned int nu = getMapSSValueAsBinaryUint(addrArgs, "n");
  
  switch (addrInfo->mode) {
  case addressMode68000Dn:
    dst->catC(dst, "D");
    dst->catInt(dst, nu, "%d");
    break;
  case addressMode68000An:
    dst->catC(dst, "A");
    dst->catInt(dst, nu, "%d");
    break;
  case addressMode68000AnInd:
    dst->catC(dst, "(A");
    dst->catInt(dst, nu, "%d");
    dst->catC(dst, ")");
    break;
  case addressMode68000AnIndInc:
    dst->catC(dst, "(A");
    dst->catInt(dst, nu, "%d");
    dst->catC(dst, ")+");
    break;
  case addressMode68000AnIndDec:
    dst->catC(dst, "-(A");
    dst->catInt(dst, nu, "%d");
    dst->catC(dst, ")");
    break;
  case addressMode68000AnOff16:
    {
      int E = getMapSSValueAsBinaryInt(addrArgs, "E");
  
      dst->catC(dst, "(");
      print680002bOffset(E, dst, config);
      dst->catC(dst, ",A");
      dst->catInt(dst, nu, "%d");
      dst->catC(dst, ")");
    }
    break;
  case addressMode68000AnOff8:
    {
      int E = getMapSSValueAsBinaryInt(addrArgs, "E");
      unsigned int i = getMapSSValueAsBinaryUint(addrArgs, "i");
      int z = getMapSSValueAsBinaryInt(addrArgs, "z");
  
      dst->catC(dst, "(");
      print680001bOffset(E, dst, config);
      dst->catC(dst, ",A");
      dst->catInt(dst, nu, "%d");
      dst->catC(dst, ",");
      
      /* Register type */
      if (i & 0x08) {
        dst->catC(dst, "A");
      }
      else {
        dst->catC(dst, "D");
      }
      
      /* Register number */
      dst->catInt(dst, (i & 0x07), "%d");
      
      /* Size */
      if (z) {
        dst->catC(dst, ".l");
      }
      else {
        dst->catC(dst, ".w");
      }
      
      dst->catC(dst, ")");
    }
    break;
  case addressMode68000Addr16:
    {
      unsigned int E = getMapSSValueAsBinaryUint(addrArgs, "E");
  
      print680002bAddress(E, dst, config);
    }
    break;
  case addressMode68000Addr32:
    {
      unsigned int E = getMapSSValueAsBinaryUint(addrArgs, "E");
  
      print680004bAddress(E, dst, config);
    }
    break;
  case addressMode68000PC16:
    {
      int E = getMapSSValueAsBinaryInt(addrArgs, "E");
  
      print680002bOffset(E, dst, config);
      dst->catC(dst, "(PC)");
    }
    break;
  case addressMode68000PC8:
    {
      int E = getMapSSValueAsBinaryInt(addrArgs, "E");
      unsigned int i = getMapSSValueAsBinaryUint(addrArgs, "i");
      int z = getMapSSValueAsBinaryInt(addrArgs, "z");
  
      print680001bOffset(E, dst, config);
      dst->catC(dst, "(PC,");
      
      /* Register type */
      if (i & 0x08) {
        dst->catC(dst, "A");
      }
      else {
        dst->catC(dst, "D");
      }
      
      /* Register number */
      dst->catInt(dst, (i & 0x07), "%d");
      
      /* Size */
      if (z) {
        dst->catC(dst, ".l");
      }
      else {
        dst->catC(dst, ".w");
      }
      
      dst->catC(dst, ")");
    }
    break;
  case addressMode68000Immed:
    {
      unsigned int E = getMapSSValueAsBinaryUint(addrArgs, "E");

      print680004bConstant(E, dst, config);
    }
    break;
  case addressMode68000Invalid:
  default:
    error("Impossible 68000 address mode");
    fatal();
    break;
  }
}
  
void printArg68000(SubOps68000* subOps, OpTarget68000 target,
                   String* dst, DismSettings* config) {
/*    fprintf(stderr, "target: %d\n", target); */
  
  switch (target) {
  case opTarget68000ConstB:
    print680001bConstant(getMapSSValueAsBinaryInt(subOps->args, "d"),
                         dst, config);
    break;
  case opTarget68000ConstW:
    print680002bConstant(getMapSSValueAsBinaryInt(subOps->args, "d"),
                         dst, config);
    break;
  case opTarget68000ConstL:
    print680004bConstant(getMapSSValueAsBinaryInt(subOps->args, "d"),
                         dst, config);
    break;
  case opTarget68000ConstBit:
    print680001bConstant(getMapSSValueAsBinaryInt(subOps->args, "b"),
                         dst, config);
    break;
  case opTarget68000ConstData3:
    print68000Data3Constant(getMapSSValueAsBinaryInt(subOps->args, "D"),
                            dst, config);
    break;
  case opTarget68000ConstData8:
    print680001bConstant(getMapSSValueAsBinaryInt(subOps->args, "d"),
                         dst, config);
    break;
  case opTarget68000ConstVector:
    dst->catC(dst, "#");
    dst->catInt(dst, getMapSSValueAsBinaryInt(subOps->args, "v"), "%d");
    break;
  case opTarget68000ConstCCC:
    print680001bConstant(getMapSSValueAsBinaryInt(subOps->args, "c"),
                         dst, config);
    break;
  case opTarget68000CCR:
    dst->catC(dst, "ccr");
    break;
  case opTarget68000Data16AS:
  case opTarget68000Data16AD:
    print680002bConstant(getMapSSValueAsBinaryInt(subOps->args, "d"),
                         dst, config);
    dst->catC(dst, "(A");
    if (target == opTarget68000Data16AS) {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "S"), "%d");
    }
    else {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "D"), "%d");
    }
    dst->catC(dst, ")");
    break;
  case opTarget68000DS:
  case opTarget68000DD:
    dst->catC(dst, "D");
    if (target == opTarget68000DS) {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "S"), "%d");
    }
    else {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "D"), "%d");
    }
    break;
  case opTarget68000Address:
    print68000AddressFromMode(subOps->addressA, subOps->addrAInfo,
                              dst, config);
    break;
  case opTarget68000AddressE:
    print68000AddressFromMode(subOps->addressE, subOps->addrEInfo,
                              dst, config);
    break;
  case opTarget68000AddressF:
    print68000AddressFromMode(subOps->addressF, subOps->addrFInfo,
                              dst, config);
    break;
  case opTarget68000AS:
  case opTarget68000AD:
    dst->catC(dst, "A");
    if (target == opTarget68000AS) {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "S"), "%d");
    }
    else {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "D"), "%d");
    }
    break;
  case opTarget68000RegList:
    dst->catC(dst, "regs");
    break;
  case opTarget68000ASInc:
  case opTarget68000ADInc:
    dst->catC(dst, "(A");
    if (target == opTarget68000ASInc) {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "S"), "%d");
    }
    else {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "D"), "%d");
    }
    dst->catC(dst, ")+");
    break;
  case opTarget68000ASDec:
  case opTarget68000ADDec:
    dst->catC(dst, "-(A");
    if (target == opTarget68000ASDec) {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "S"), "%d");
    }
    else {
      dst->catInt(dst, getMapSSValueAsBinaryUint(subOps->args, "D"), "%d");
    }
    dst->catC(dst, ")");
    break;
  case opTarget68000Label:
    {
      int labelFirst = getMapSSValueAsBinaryInt(subOps->args, "l");
      if (labelFirst != 0) {
        print680001bOffset(getMapSSValueAsBinaryInt(subOps->args, "l"),
                             dst, config);
      }
      else {
        print680002bOffset(getMapSSValueAsBinaryInt(subOps->args, "L"),
                             dst, config);
      }
    }
    break;
  case opTarget68000None:
  default:
    
    break;
  }
  
}

/* 68000 Opcode printing override */
void printOpcode68000(Opcode* obj, String* dst, DismSettings* config) {
/*  fprintf(stderr, "%04X %s\n", obj->pos_, obj->info_->name); */

  SubOps68000* subOps = (SubOps68000*)(obj->data_);
/*  const char* argStr = (const char*)(obj->info_->data);
  OpTarget68000 srcArgType = srcArgType68000(argStr);
  OpTarget68000 dstArgType = dstArgType68000(argStr); */
  
  /* Print src op if it exists */
  if (subOps->src != opTarget68000None) {
    printArg68000(subOps, subOps->src,
                  dst, config);
    dst->catC(dst, ",");
  }
  
  /* Print dst op if it exists */
  if (subOps->dst != opTarget68000None) {
    printArg68000(subOps, subOps->dst,
                  dst, config);
  }
  
/*  fprintf(stderr, "success\n"); */
}

/*================================================================
  =================== No longer used -- remove ===================
  ================================================================*/
OpRead68000Result readOpArg68000(Opcode* obj, BufferStream* stream,
                                 OpTarget68000 argType, int offset,
                                 DismSettings* config, MapSS* args) {
/*  verifyRemainingSpace(stream, pos, amount); */
  OpRead68000Result result;
  result.length = 0;
  result.success = 1;
  
  stream->seekOff(stream, offset);
  
  /* Read the appropriate op data.
     Action is only necessary if there are additional arguments placed
     after a variable-length address. Otherwise, they'll be automatically
     collated with the recognition string. */
  switch (argType) {
  /* read the extra data appropriately */
  case opTarget68000ConstB:
    if (!verifyRemainingSpace(stream, stream->tell(stream), 2)) {
      result.success = 0;
      break;
    }
    
    result.length = 2;
    /* Bart's disassembler claims the top half of the word doesn't actually
       matter in byte mode. He's probably right, but I expect real assemblers
       aren't going to put anything there, so we can use it as another mnemonic
       for data/code distinction. */
    collateOpArgs(stream, args, "00000000dddddddd");
    break;
  case opTarget68000ConstW:
    if (!verifyRemainingSpace(stream, stream->tell(stream), 2)) {
      result.success = 0;
      break;
    }
    
    result.length = 2;
    collateOpArgs(stream, args, "dddddddddddddddd");
    break;
  case opTarget68000ConstL:
    if (!verifyRemainingSpace(stream, stream->tell(stream), 4)) {
      result.success = 0;
      break;
    }
    
    result.length = 4;
    collateOpArgs(stream, args, "dddddddddddddddddddddddddddddddd");
    break;
  case opTarget68000ConstBit:
    if (!verifyRemainingSpace(stream, stream->tell(stream), 2)) {
      result.success = 0;
      break;
    }
    
    result.length = 2;
    /* The note on ignored bits above probably applies here as well, though I
       haven't checked. */
    collateOpArgs(stream, args, "00000000000bbbbb");
    break;
  case opTarget68000RegList:
    if (!verifyRemainingSpace(stream, stream->tell(stream), 2)) {
      result.success = 0;
      break;
    }
    
    result.length = 2;
    collateOpArgs(stream, args, "qqqqqqqqrrrrrrrr");
    break;
    
  /* do nothing; these are handled specially */
  case opTarget68000Address:
  case opTarget68000AddressE:
  case opTarget68000AddressF:
    
    break;
    
  /* No action necessary */
  case opTarget68000ConstData3:
  case opTarget68000ConstData8:
  case opTarget68000ConstVector:
  case opTarget68000ConstCCC:
  case opTarget68000CCR:
  case opTarget68000Data16AS:
  case opTarget68000Data16AD:
  case opTarget68000DS:
  case opTarget68000DD:
  case opTarget68000AS:
  case opTarget68000AD:
  case opTarget68000ASInc:
  case opTarget68000ADInc:
  case opTarget68000ASDec:
  case opTarget68000ADDec:
  case opTarget68000Label:
  case opTarget68000None:
    
    break;
  default:
    error("Illegal 68000 arg type\n");
    fatal();
    break;
  }
  
  stream->seekOff(stream, -offset);
  
  return result;
}

OpRead68000Result readExtraOpArg68000(Opcode* obj, BufferStream* stream,
                                 OpTarget68000 argType, int offset,
                                 DismSettings* config, MapSS* args) {
  /* This is needed solely for labels, where the size of the label varies
     depending on its content. */
  
  OpRead68000Result result;
  result.length = 0;
  result.success = 1;
  
  if (argType != opTarget68000Label) return result;
  
  stream->seekOff(stream, offset);
  
  /* If l == 0, a 16-bit address follows */
  if (getMapSSValueAsBinaryUint(args, "l") == 0) {
    /* Insert as L */
    collateOpArgs(stream, args, "LLLLLLLLLLLLLLLL");
    
    result.length = 2;
  }
  
  stream->seekOff(stream, -offset);
  
  return result;
}

/* 68000 Opcode readStep override */
unsigned int readStepOpcode68000(struct Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args) {
/*  fprintf(stderr, "starting: %06X %s %s\n",
      stream->tell(stream), obj->info_->name,
      (const char*)(obj->info_->data)); */
                         
  SubOps68000* subOps = (SubOps68000*)(obj->data_);
  subOps->args = args;
  
  const char* argStr = (const char*)(obj->info_->data);
  OpTarget68000 srcArgType = srcArgType68000(argStr);
  subOps->src = srcArgType;
  OpTarget68000 dstArgType = dstArgType68000(argStr);
  subOps->dst = dstArgType;
  
  unsigned int streamBase = stream->tell(stream);
  unsigned int total = strlen(obj->info_->recString) / k_bitsPerByte;
  
  /* If a, f, or e exist, we must read them */
  if ((srcArgType == opTarget68000AddressE)
      && (dstArgType == opTarget68000AddressF)) {
/*    fprintf(stderr, "%04X\n", stream->tell(stream)); */
      
    Address68000Struct addrResultE;
    addrResultE = read68000Address(getMapSSValue(subOps->args, "e"), stream,
                                  streamBase + total, config,
                                  obj->info_,
                                  0);
    if (!(addrResultE.success)) {
/*      fprintf(stderr, "fail1\n"); */
      return 0;
    }
    
    subOps->addressE = addrResultE.addr;
    subOps->addrESize = addrResultE.addrSize;
    subOps->addrEInfo = addrResultE.addrInfo;
    
    total += addrResultE.addrSize;
    
    /* F's address mode is stored with a and n reversed */
    String originalF = getMapSSValue(subOps->args, "f");
    String reversedF;
    initString(&reversedF);
    int i;
    for (i = 0; i < 3; i++) {
      reversedF.catData(&reversedF, originalF.cStr(&originalF) + i + 3, 1);
    }
    for (i = 0; i < 3; i++) {
      reversedF.catData(&reversedF, originalF.cStr(&originalF) + i + 0, 1);
    }
    
    Address68000Struct addrResultF;
    addrResultF = read68000Address(reversedF, stream,
                                  streamBase + total, config,
                                  obj->info_,
                                  1);
    reversedF.destroy(&reversedF);
    if (!(addrResultF.success)) {
/*      fprintf(stderr, "fail2\n"); */
      return 0;
    }
    
    subOps->addressF = addrResultF.addr;
    subOps->addrFSize = addrResultF.addrSize;
    subOps->addrFInfo = addrResultF.addrInfo;
    
    total += addrResultF.addrSize;
    
/*    fprintf(stderr, "success\n"); */
  }
  else if ((srcArgType == opTarget68000Address)
      || (dstArgType == opTarget68000Address)) {
    Address68000Struct addrResultA;
    addrResultA = read68000Address(getMapSSValue(subOps->args, "a"), stream,
                                  streamBase + total, config,
                                  obj->info_,
                                  (dstArgType == opTarget68000Address));
    if (!(addrResultA.success)) {
      return 0;
    }
    
    total += addrResultA.addrSize;
/*    fprintf(stderr, "%d %d\n", total, addrResultA.addrSize); */
    
    subOps->addressA = addrResultA.addr;
    subOps->addrASize = addrResultA.addrSize;
    subOps->addrAInfo = addrResultA.addrInfo;
  }
  
  if (subOps->src != opTarget68000None) {
    OpRead68000Result readResult = readExtraOpArg68000(obj, stream,
                                subOps->src, total, config, subOps->args);
    if (!(readResult.success)) return 0;
    total += readResult.length;
  }
  
  if (subOps->dst != opTarget68000None) {
    OpRead68000Result readResult = readExtraOpArg68000(obj, stream,
                                subOps->dst, total, config, subOps->args);
    if (!(readResult.success)) return 0;
    total += readResult.length;
  }
  
  /* these are now unnecessary */
/*  if (subOps->src != opTarget68000None) {
    OpRead68000Result readResult = readOpArg68000(obj, stream, subOps->src,
                                       total, config, subOps->args);
    if (!(readResult.success)) return 0;
    total += readResult.length;
  }
  
  if (subOps->dst != opTarget68000None) {
    OpRead68000Result readResult = readOpArg68000(obj, stream, subOps->dst,
                                       total, config, subOps->args);
    if (!(readResult.success)) return 0;
    total += readResult.length;
  } */
  
/*  fprintf(stderr, "success\n"); */
  
  return total;
}

OpcodeSimilarity compare68000AddressFromMode(MapSS* addrArgsA,
                               MapSS* addrArgsB,
                               AddressMode68000Info* addrInfoA,
                               AddressMode68000Info* addrInfoB,
                               DismSettings* config) {
  
  unsigned int aA = getMapSSValueAsBinaryUint(addrArgsA, "a");
  unsigned int aB = getMapSSValueAsBinaryUint(addrArgsB, "a");
  
  if (aA != aB) return opcodeSimilarityNone;
  
  unsigned int nA = getMapSSValueAsBinaryUint(addrArgsA, "n");
  unsigned int nB = getMapSSValueAsBinaryUint(addrArgsB, "n");
  
  if (nA != nB) return opcodeSimilarityNone;
  
  switch (addrInfoA->mode) {
  case addressMode68000Dn:
  case addressMode68000An:
  case addressMode68000AnInd:
  case addressMode68000AnIndInc:
  case addressMode68000AnIndDec:
    return opcodeSimilaritySame;
    break;
  case addressMode68000Immed:
    {
      int EA = getMapSSValueAsBinaryInt(addrArgsA, "E");
      int EB = getMapSSValueAsBinaryInt(addrArgsB, "E");
  
      return compare680004bAddress(EA, EB, config);
    }
    break;
  case addressMode68000AnOff16:
    {
      int EA = getMapSSValueAsBinaryInt(addrArgsA, "E");
      int EB = getMapSSValueAsBinaryInt(addrArgsB, "E");
  
      return compare680002bOffset(EA, EB, config);
    }
    break;
  case addressMode68000AnOff8:
    {
      int EA = getMapSSValueAsBinaryInt(addrArgsA, "E");
      int iA = getMapSSValueAsBinaryInt(addrArgsA, "i");
      int zA = getMapSSValueAsBinaryInt(addrArgsA, "z");
      int EB = getMapSSValueAsBinaryInt(addrArgsB, "E");
      int iB = getMapSSValueAsBinaryInt(addrArgsB, "i");
      int zB = getMapSSValueAsBinaryInt(addrArgsB, "z");
      
      if (iA != iB) return opcodeSimilarityNone;
      if (zA != zB) return opcodeSimilarityNone;
  
      return compare680001bOffset(EA, EB, config);
    }
    break;
  case addressMode68000Addr16:
    {
      unsigned int EA = getMapSSValueAsBinaryUint(addrArgsA, "E");
      unsigned int EB = getMapSSValueAsBinaryUint(addrArgsB, "E");
  
      return compare680002bAddress(EA, EB, config);
    }
    break;
  case addressMode68000Addr32:
    {
      unsigned int EA = getMapSSValueAsBinaryUint(addrArgsA, "E");
      unsigned int EB = getMapSSValueAsBinaryUint(addrArgsB, "E");
  
      return compare680004bAddress(EA, EB, config);
    }
    break;
  case addressMode68000PC16:
    {
      unsigned int EA = getMapSSValueAsBinaryUint(addrArgsA, "E");
      unsigned int EB = getMapSSValueAsBinaryUint(addrArgsB, "E");
  
      return compare680002bAddress(EA, EB, config);
    }
    break;
  case addressMode68000PC8:
    {
      int EA = getMapSSValueAsBinaryInt(addrArgsA, "E");
      int iA = getMapSSValueAsBinaryInt(addrArgsA, "i");
      int zA = getMapSSValueAsBinaryInt(addrArgsA, "z");
      int EB = getMapSSValueAsBinaryInt(addrArgsB, "E");
      int iB = getMapSSValueAsBinaryInt(addrArgsB, "i");
      int zB = getMapSSValueAsBinaryInt(addrArgsB, "z");
      
      if (iA != iB) return opcodeSimilarityNone;
      if (zA != zB) return opcodeSimilarityNone;
  
      return compare680002bAddress(EA, EB, config);
    }
    break;
  case addressMode68000Invalid:
  default:
    error("Impossible 68000 address mode");
    fatal();
    break;
  }

  /* not possible, but needed to suppress compiler warning */
  return opcodeSimilarityNone;
}

OpcodeSimilarity compareSubop68000(SubOps68000* a, SubOps68000* b,
                                   OpTarget68000 targetA,
                                   OpTarget68000 targetB,
                                   DismSettings* config) {
/*  fprintf(stderr, "comparing: %d %d\n", targetA, targetB); */
                                   
  if (targetA != targetB) return opcodeSimilarityNone;
  
  switch (targetA) {
  case opTarget68000ConstB:
    return compare680001bConstant(
                         getMapSSValueAsBinaryInt(a->args, "d"),
                         getMapSSValueAsBinaryInt(b->args, "d"),
                         config);
    break;
  case opTarget68000ConstW:
    return compare680002bConstant(
                         getMapSSValueAsBinaryInt(a->args, "d"),
                         getMapSSValueAsBinaryInt(b->args, "d"),
                         config);
    break;
  case opTarget68000ConstL:
    return compare680004bConstant(
                         getMapSSValueAsBinaryInt(a->args, "d"),
                         getMapSSValueAsBinaryInt(b->args, "d"),
                         config);
    break;
  case opTarget68000ConstBit:
    return compare680001bConstant(
                         getMapSSValueAsBinaryInt(a->args, "b"),
                         getMapSSValueAsBinaryInt(b->args, "b"),
                         config);
    break;
  case opTarget68000ConstData3:
    return compare680001bConstant(
                         getMapSSValueAsBinaryInt(a->args, "D"),
                         getMapSSValueAsBinaryInt(b->args, "D"),
                         config);
    break;
  case opTarget68000ConstData8:
    return compare680001bConstant(
                         getMapSSValueAsBinaryInt(a->args, "d"),
                         getMapSSValueAsBinaryInt(b->args, "d"),
                         config);
    break;
  case opTarget68000ConstVector:
    return compare680001bConstant(
                         getMapSSValueAsBinaryInt(a->args, "v"),
                         getMapSSValueAsBinaryInt(b->args, "v"),
                         config);
    break;
  case opTarget68000ConstCCC:
    return compare680001bConstant(
                         getMapSSValueAsBinaryInt(a->args, "c"),
                         getMapSSValueAsBinaryInt(b->args, "c"),
                         config);
    break;
  case opTarget68000CCR:
    return opcodeSimilaritySame;
    break;
  case opTarget68000Data16AS:
    return greaterOpDifference(
      compare680002bConstant(
                         getMapSSValueAsBinaryInt(a->args, "d"),
                         getMapSSValueAsBinaryInt(b->args, "d"),
                         config),
      compare68000RegisterIndex(
                         getMapSSValueAsBinaryInt(a->args, "S"),
                         getMapSSValueAsBinaryInt(b->args, "S"),
                         config)
      );
    break;
  case opTarget68000Data16AD:
    return greaterOpDifference(
      compare680002bConstant(
                         getMapSSValueAsBinaryInt(a->args, "d"),
                         getMapSSValueAsBinaryInt(b->args, "d"),
                         config),
      compare68000RegisterIndex(
                         getMapSSValueAsBinaryInt(a->args, "D"),
                         getMapSSValueAsBinaryInt(b->args, "D"),
                         config)
      );
    break;
  case opTarget68000DS:
    return compare68000RegisterIndex(
                       getMapSSValueAsBinaryInt(a->args, "S"),
                       getMapSSValueAsBinaryInt(b->args, "S"),
                       config);
    break;
  case opTarget68000DD:
    return compare68000RegisterIndex(
                       getMapSSValueAsBinaryInt(a->args, "D"),
                       getMapSSValueAsBinaryInt(b->args, "D"),
                       config);
    break;
  case opTarget68000Address:
    return compare68000AddressFromMode(a->addressA, b->addressA,
                                       a->addrAInfo, b->addrAInfo, config);
    break;
  case opTarget68000AddressE:
    return compare68000AddressFromMode(a->addressE, b->addressE,
                                  a->addrEInfo, b->addrEInfo, config);
    break;
  case opTarget68000AddressF:
    return compare68000AddressFromMode(a->addressF, b->addressF,
                                  a->addrFInfo, b->addrFInfo, config);
    break;
  case opTarget68000AS:
    return compare68000RegisterIndex(
                       getMapSSValueAsBinaryInt(a->args, "S"),
                       getMapSSValueAsBinaryInt(b->args, "S"),
                       config);
    break;
  case opTarget68000AD:
    return compare68000RegisterIndex(
                       getMapSSValueAsBinaryInt(a->args, "D"),
                       getMapSSValueAsBinaryInt(b->args, "D"),
                       config);
    break;
  case opTarget68000RegList:
    return greaterOpDifference(
      compare680001bConstant(
                         getMapSSValueAsBinaryInt(a->args, "q"),
                         getMapSSValueAsBinaryInt(b->args, "q"),
                         config),
      compare680001bConstant(
                         getMapSSValueAsBinaryInt(a->args, "r"),
                         getMapSSValueAsBinaryInt(b->args, "r"),
                         config)
      );
    break;
  case opTarget68000ASInc:
  case opTarget68000ASDec:
    return compare68000RegisterIndex(
                       getMapSSValueAsBinaryInt(a->args, "S"),
                       getMapSSValueAsBinaryInt(b->args, "S"),
                       config);
    break;
  case opTarget68000ADInc:
  case opTarget68000ADDec:
    return compare68000RegisterIndex(
                       getMapSSValueAsBinaryInt(a->args, "D"),
                       getMapSSValueAsBinaryInt(b->args, "D"),
                       config);
    break;
  case opTarget68000Label:
    return compare680002bOffset(
                       getMapSSValueAsBinaryInt(a->args, "l"),
                       getMapSSValueAsBinaryInt(b->args, "l"),
                       config);
    break;
  case opTarget68000None:
    return opcodeSimilaritySame;
    break;
  default:
    error("Illegal 68000 subop comparison");
    fatal();
    break;
  }
  
  /* should never happen */
  return opcodeSimilarityNone;
}

OpcodeSimilarity compareOpcode68000(Opcode* obj, Opcode* other,
                         DismSettings* config) {
  SubOps68000* subOpsA = (SubOps68000*)(obj->data_);
  SubOps68000* subOpsB = (SubOps68000*)(other->data_);
  
  OpcodeSimilarity srcSim = compareSubop68000(subOpsA, subOpsB,
                                              subOpsA->src,
                                              subOpsB->src,
                                              config);
  
  OpcodeSimilarity dstSim = compareSubop68000(subOpsA, subOpsB,
                                              subOpsA->dst,
                                              subOpsB->dst,
                                              config);
  
  return greaterOpDifference(srcSim, dstSim);
}

void generateOpcode68000(OpInfo* opInfo, Opcode* dst,
                      DismSettings* config) {
  dst->compare = compareOpcode68000;
  dst->readStep = readStepOpcode68000;
  dst->printString = printOpcode68000;
  dst->printName = OpcodeprintNameWithSpace;
  
  dst->setData(dst, allocSubOps68000());
  dst->destroy = destructorSubOps68000Opcode;
}



void initModule68000(DismModule* obj) {
  initDismModule(obj);
  obj->byteAlignment_ = 2;
  
  OpInfoArray ops = { opcodes68000, sizeof(opcodes68000) / sizeof(OpInfo) };
  obj->opInfoArrays.pushBack(&(obj->opInfoArrays), ops);
}

DismModule* allocModule68000() {
  DismModule* obj = malloc(sizeof(DismModule));
  initModule68000(obj);
  return obj;
}

void freeModule68000(DismModule* obj) {
  free(obj);
}

