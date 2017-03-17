#ifndef CPD_STRING_MATCHER_H
#define CPD_STRING_MATCHER_H


#include "util/BufferStream.h"

/**
 * Returns nonzero if the next bits in a stream match the binary recognition
 * string str.
 *
 * @see OpInfo
 */
int matchBinaryString(BufferStream* stream, char* str, int remaining);

int binaryStringToInt(const char* str, int len);
unsigned int binaryStringToUint(const char* str, int len);


#endif
