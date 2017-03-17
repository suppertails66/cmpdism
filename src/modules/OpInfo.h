#ifndef CPD_OP_INFO_H
#define CPD_OP_INFO_H


#include "modules/DismSettings.h"
struct Opcode;

/**
 * Enum of property bitflags for ops.
 */
enum OpFlags {
  /**
   * Indicates no op flags are set.
   */
  opFlagsNone           = 0x00,
  /**
   * Flags an op as "suspicious" during code/data map generation.
   * This has the following effects:
   * * When the code detector detects a data block, it will consider
   *   any sequence of immediately preceding "suspicious" ops as misdetected
   *   data, and will backtrack through them and mark them as such until it
   *   reaches a non-suspicious op or the start of the bytestream.
   * * When the detector is trying to find the end of a data block, it will
   *   not count suspicious opcodes toward the total of valid ops (nor will
   *   it consider them as interrupting the sequence it is currently checking;
   *   they are simply ignored).
   */
  opFlagsSuspicious     = 0x01,
};

/**
 * Data struct containing identifying information for an op.
 * These are properties that apply to all ops of a particular type.
 * The Opcode class represents particular instances of these ops within
 * a bytestream.
 * @see Opcode
 */
typedef struct OpInfo {

  /**
   * Name (assembly symbol) of the op.
   * This is displayed as the op name in the disassembled output.
   */
  char* name;
  
  /**
   * The op's recognition string.
   * This is used to identify the op when disassembling. It is a binary-
   * formatted C-string that uniquely identifies this op within its own
   * module.
   *
   * The following symbols are allowed in the recognition string:
   *
   * 0 - Indicates a bit that must be unset to produce this op.
   * 1 - Indicates a bit that must be set to produce this op.
   * x - Indicates a bit whose state doesn't matter (but must exist).
   *
   * The recognition string is evaluated from left to right, where the leftmost
   * characters correspond to the highest bits. If all mandatory bits (0 or 1)
   * match, and there is sufficient data in the stream that all x bits exist,
   * the op is considered matched.
   *
   * Ops are assumed to be byte-aligned. If the recognition string's
   * length is not divisible by 8, it will effectively be padded with x bits
   * to the nearest multiple of 8.
   *
   * Example recognition strings:
   * "11101010"         -- matches EA, the 6502 NOP
   * "01100000xxxxxxxx" -- matches 60 NN, the 68000 BRA
   * "1000xxx100000xxx" -- matches the 68000 SBCD Ds, Dd
   */
  char* recString;
  
  /**
   * Op property flags.
   */
  int flags;
  
  /**
   * Pointer to an Opcode generator function for this op.
   * An Opcode generator takes an OpInfo as an argument and returns a
   * newly allocated Opcode representing an instance of the op specified
   * by the passed OpInfo.
   */
  void (*generateOpcode)(struct OpInfo* opInfo, struct Opcode* dst,
                        DismSettings* config);

  /**
   * The internal ID of the op, which must be unique within its module.
   * This is an arbitrary value which has nothing to do with the op's
   * actual encoding, but is used to perform top-level matching between
   * Opcodes.
   * 
   * If two Opcodes have the same ID, then it is assumed they are of the
   * same type and may be safely evaluated for subcode similarity.
   */
  int id;
  
  void* data;

} OpInfo;


#endif
