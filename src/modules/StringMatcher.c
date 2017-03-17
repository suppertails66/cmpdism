#include "modules/StringMatcher.h"
#include "modules/Consts.h"
#include "util/Logger.h"
#include <stdlib.h>
#include <stdio.h>

int matchBinaryString(BufferStream* stream, char* str, int remaining) {
  /* Get length of string in bytes and bits */
  int bitLen = strlen(str);
  int byteLen = bitLen / k_bitsPerByte;
  
  /* Assume byte-aligned opcodes, and "pad" to the next byte if needed */
  if ((bitLen % 8) != 0) ++byteLen;

  /* Make sure enough bytes remain in the stream to match the string */
/*  if (stream->remaining(stream) < byteLen) return 0; */
  if (remaining < byteLen) return 0;
  
  /* Try to match digits in input string */
  int i;
  int bitPos = 0;
  for (i = 0; i < byteLen; i++) {
    char c = *(stream->getcur(stream) + i);
    
    /* Check bits in order from highest to lowest */
    int j;
    for (j = 0x80; j >= 0x01; j >>= 1) {
      
      int code = str[bitPos];
      
      switch (code) {
      /* Fail if bit is set */
      case (int)k_recognitionCode0:
        if ((c & j)) return 0;
        break;
      /* Fail if bit is not set */
      case (int)k_recognitionCode1:
        if (!(c & j)) return 0;
        break;
      /* Ignore bit */
/*      case (int)k_recognitionCodeAny: */
      default:
        break;
/*      default:
        error("Error parsing recognition string: ");
        error(str);
        fatal();
        break; */
      }
      
      /* Stop if all bits have been checked (if code is not byte-aligned) */
      if (++bitPos == bitLen) break;
    }
    
  }
  
  return 1;
}

int binaryStringToInt(const char* str, int len) {
  int result = binaryStringToUint(str, len);
  
  int signMask = (0x01 << (len - 1));
  if (result & signMask) {
    result = -((signMask << 1) - result);
  }
  
  return result;
}

unsigned int binaryStringToUint(const char* str, int len) {
  unsigned int result = 0;
  
  unsigned int mask = (0x01 << (len - 1));
  int i;
  for (i = 0; i < len; i++) {
    if (str[i] == '1') {
      result |= mask;
    }
    
    mask >>= 1;
  }
  
  return result;
}

