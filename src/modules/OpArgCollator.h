#ifndef CPD_OP_ARG_COLLATOR_H
#define CPD_OP_ARG_COLLATOR_H


#include "util/String.h"
#include "util/Maps.h"
#include "util/BufferStream.h"

/* Maximum possible recognition string length.
   Used to size the buffer used for endianness conversions. */
const extern int maxInstructionByteLength;

/**
 * Collects the parameters of an op into a MapSS using its recognition string.
 * 
 * An example is probably easiest to understand.
 *
 * recString: "101011aa0010bb10cccc11cc"
 * Stream:     101011110010101001101111 (AF 2A 6F)
 * 
 * Calling this function will add to dst the following three entries:
 *   dst["a"] = "11"
 *   dst["b"] = "10"
 *   dst["c"] = "011011"
 *
 * As shown, each sequence of letters becomes an appropriately named entry
 * in the map. Non-adjacent sequences are concatenated in the order they
 * appear from left to right.
 *
 * It is currently assumed that non-adjacent sequences will never be
 * interrupted by other named sequences, so e.g. "10aabbaa" will not be read
 * correctly (b will be skipped).
 *
 * @return Number of bytes read from the stream.
 */
int collateOpArgsFull(BufferStream* src, MapSS* dst, const char* recString,
                      int reverseEnd, int reverseBits);
                      
int collateOpArgs(BufferStream* src, MapSS* dst, const char* recString);

int collateOpArgsBuffer(char* src, MapSS* dst, const char* recString,
                        int reverseEnd, int );

int getStreamBit(BufferStream* src, int index);

int getBufferBit(char* src, int index);


#endif
