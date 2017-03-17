#ifndef CPD_OPCODE_H
#define CPD_OPCODE_H


#include "modules/OpInfo.h"
#include "modules/DismSettings.h"
#include "util/BufferStream.h"
#include "util/String.h"
#include "util/Maps.h"

struct DismStruct;

/**
 * Enum of "similarity level" for ops compared using Opcode::compare().
 */
typedef enum {
  opcodeSimilaritySame,      /**< 100% identical */
  opcodeSimilarityNear,      /**< Differ by e.g. a JMP target */
  opcodeSimilarityDistinct,  /**< Differ by e.g. an immediate constant */
  opcodeSimilarityNone       /**< Opcode types differ */
} OpcodeSimilarity;

OpcodeSimilarity greaterOpSimilarity(
    OpcodeSimilarity first, OpcodeSimilarity second);

OpcodeSimilarity greaterOpDifference(
    OpcodeSimilarity first, OpcodeSimilarity second);

/**
 * Class representing an instance of an op.
 * Opcodes represent particular instances of an op within a bytestream.
 * @see OpInfo
 */
typedef struct Opcode {

  /* PUBLIC */
  
  /**
   * Pointer to comparison function for this Opcode.
   * A comparison function takes two Opcodes as parameters and returns
   * a value from the OpcodeSimilarity enum indicating the sameness of
   * the Opcodes.
   *
   * Implementations may assume that when this function is called, both Opcodes
   * are of the same type (i.e. they have the same info_->id), and therefore
   * have data_ fields that point to the same type of struct.
   */
  OpcodeSimilarity (*compare)(struct Opcode* obj, struct Opcode* other,
                         DismSettings* config);
  
  /**
   * Returns nonzero if obj and other are functionally congruent, and false
   * otherwise.
   * Implementations may assume the two Opcodes have the same ID.
   * 
   * "Functional congruence" means that the tasks performed by the ops are
   * the same, though not necessarily the data involved in that task. For
   * example, JMP $8089 and JMP $FFB5 are functionally congruent because
   * they differ only by the data (address) involved. However,
   * MOVE.W #$0016,D0 and MOVE.B #$16,D0 are not congruent, because MOVE.B
   * and MOVE.W are distinct operations.
   *
   * The critical point here is to discriminate between ops that could
   * possibly be performing the same task, just with altered constants or
   * offsets, versus ops that could not possibly (or are extremely unlikely
   * to) be performing the same task.
   *
   * The default implementation returns nonzero.
   */
  int (*isFunctionallyCongruent)(struct Opcode* obj, struct Opcode* other);
  
  /**
   * Pointer to read function for this Opcode.
   * Given a BufferStream, this generates a valid Opcode from the data at
   * the current position, if possible.
   * Calls readStep();
   *
   * @param obj The obj on which the function was called.
   * @param stream The stream to read from (positioned at the target read
   * position).
   * @param config Pointer to the program settings.
   * @param args Pointer to a MapSS containing the automatically collated
   * arguments from this op, or NULL if this feature is disabled by the
   * current module.
   * @return The number of bytes read from the stream, or 0 upon failure.
   * @see readStep()
   * @see collateOpArgs()
   */
  unsigned int (*read)(struct Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args);
  
  void* (*data)(struct Opcode* obj);
  
  void (*setData)(struct Opcode* obj, void* data__);
  
  unsigned int (*pos)(struct Opcode* obj);
  
  OpInfo* (*info)(struct Opcode* obj);

  /* PRIVATE */
  
  /**
   * Pointer to read "step" function for this Opcode.
   * This performs op-specific handling, such as allocating the data_
   * field, during a call to read().
   *
   * Implementations may assume that when this function is called:
   * * the stream does, in fact, point to a valid op of this type, as long
   *   as no variable-length subcodes are present (see below).
   * * there are at least as many bits remaining in the BufferStream as
   *   were specified in op's recognition string. For many architectures,
   *   this is sufficient to negate any need to verify the amount of data
   *   remaining in the stream. However, if an op has a variable-length
   *   subcode -- such as for scripting systems in which strings are used
   *   as direct parameters to ops -- it may be necessary to verify that
   *   enough data remains in the stream, and return 0 if the op was
   *   misdetected.
   *
   * @return The number of bytes read from the stream, or 0 upon failure.
   * @see read()
   */
  unsigned int (*readStep)(struct Opcode* obj, BufferStream* stream,
                         DismSettings* config, MapSS* args);
  
  /**
   * Prints disassembled op to a string using the given formatting parameters.
   *
   * @param obj Instance the function was called on.
   * @param dst Destination String for the output.
   * @param dismStruct The DismStruct that contains the Opcode.
   * @param srcAddrW Minimum width in characters of source addresses. Shorter
   * addresses will be zero-padded.
   * @param showRaw If nonzero, prints raw byte representation of op.
   * @param middleWidth Specifies the padding between the address/raw bytes
   * and the disassembly itself.
   */
  void (*print)(struct Opcode* obj, String* dst,
                BufferStream* stream,
                DismSettings* config,
                int srcAddrW,
                int showRaw,
                int middleWidth);
  
  /**
   * Prints all op-specific content -- that is, everything other than the
   * name -- to the given String.
   * Following the standard offset -> name -> parameters layout for a
   * disassembly, this contains the parameters. Everything else is handled
   * generically.
   */
  void (*printString)(struct Opcode* obj, String* dst,
                         DismSettings* config);
  
  /**
   * Prints the op name.
   *
   * Default implementation does not put a space after the op name in order
   * to allow additional specifiers if necessary (e.g. move.b).
   */
  void (*printName)(struct Opcode* obj, String* dst,
                    DismSettings* config);
  
  /**
   * Pointer to destructor function for this Opcode.
   * This should free any memory allocated for the data_ field.
   */
  void (*destroy)(struct Opcode* obj);
  
  /**
   * Pointer to an OpInfo containing general data for this op.
   */
  OpInfo* info_;
  
  /**
   * Pointer to an info struct of a type dependent on the op's OpInfo ID.
   * This is used to store any extra data necessary to represent the op,
   * such as offsets, constants, etc.
   *
   * If the info_->id field has the same value for two Opcode instances,
   * they are assumed to be of the same type and safe for comparison with
   * compare().
   */
  void* data_;
  
  /**
   * The position at which the Opcode was read from the bytestream.
   */
  unsigned int pos_;

} Opcode;


int OpcodeisFunctionallyCongruent(struct Opcode* obj, struct Opcode* other);

/**
 * Default implementation for read().
 */
unsigned int Opcoderead(Opcode* obj, BufferStream* stream,
                        DismSettings* config, MapSS* args);
                        
void Opcodeprint(Opcode* obj, String* dst,
                 BufferStream* stream,
                 DismSettings* config,
                 int srcAddrW,
                 int showRaw,
                 int middleWidth);

void OpcodeprintName(Opcode* obj, String* dst,
                  DismSettings* config);

void OpcodeprintNameWithSpace(Opcode* obj, String* dst,
                  DismSettings* config);
  
void* Opcodedata(Opcode* obj);
  
void OpcodesetData(Opcode* obj, void* data__);

unsigned int Opcodepos(Opcode* obj);

OpInfo* Opcodeinfo(Opcode* obj);

/**
 * Default destructor for Opcodes.
 * Does nothing.
 */
void Opcodedestroy(Opcode* obj);

/**
 * Base initializer for Opcodes.
 */
void initOpcode(Opcode* obj);

/**
 * Base allocator for Opcodes.
 */
Opcode* allocOpcode();

/**
 * Base deallocator for Opcodes.
 */
void freeOpcode(Opcode* obj);


#endif
