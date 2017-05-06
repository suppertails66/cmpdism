#include "util/ByteConv.h"

int fromBytes(const char* src,
              int size,
              Endianness endianness,
              Signedness signedness) {
  int result = 0;
  
  int i;
  switch (endianness) {
  case littleEnd:
    for (i = 0; i < size; i++) {
      result 
        |= (((const unsigned char*)src)[i]) << (i * 8);
    }
    break;
  case bigEnd:
    for (i = 0; i < size; i++) {
      result 
        |= (((const unsigned char*)src)[i]) << ((size - i - 1) * 8);
    }
    break;
  default:
    break;
  }
  
  // Convert signed values from two's complement
  if (signedness == sign) {
    // If highest bit is set, number is negative
    int shift = (1 << ((size * 8) - 1));
    if (result & shift) {
      result -= (shift << 1);
    }
  }
  
  return result;
}

void toBytes(char* dst,
             int value,
             int size,
             Endianness endianness,
             Signedness signedness) {
  // Convert negative values to native two's complement representation
  if (value < 0
      && signedness == sign) {
    value += ((1 << (size * 8)));
  }
  
  int i;
  switch (endianness) {
  case littleEnd:
    for (i = 0; i < size; i++) {
      int shift = (i * 8);
      ((unsigned char*)dst)[i]
        = (unsigned char)((value & (0xFF << shift)) >> shift);
    }
    break;
  case bigEnd:
    for (i = 0; i < size; i++) {
      int shift = (i * 8);
      ((unsigned char*)dst)[size - i - 1]
        = (unsigned char)((value & (0xFF << shift)) >> shift);
    }
    break;
  default:
    break;
  }
}

int reverseEndianness(int value, int numbytes) {
  int rev = 0;
  int i;
  for (i = 0; i < numbytes; i++) {
    int srcshift = i * 8;
    int dstshift = (numbytes - i - 1) * 8;
    
    int srcmask = 0xFF << srcshift;
    int b = (value & srcmask) >> srcshift;
    rev |= (b << dstshift);
  }
  
  return rev;
}


