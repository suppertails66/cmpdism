#include "modules/OpArgCollator.h"
#include "modules/Consts.h"
#include <string.h>
#include <ctype.h>

/* Maximum possible recognition string length.
   Used to size the buffer used for endianness conversions. */
const int maxInstructionByteLength = 32;

int collateOpArgsBuffer(char* src, MapSS* dst, const char* recString,
                        int reverseEnd, int reverseBits) {
  int bitlen = strlen(recString);
  int bytelen = bitlen / k_bitsPerByte;
  
  /* If we're reversing endianness, do so in a new buffer, then update src */
  char srcbuf[maxInstructionByteLength];
  if (reverseEnd) {
    int i;
    for (i = 0; i < bytelen; i++) {
      srcbuf[i] = src[bytelen - i - 1];
    }
    
    src = srcbuf;
  }
                        
  int i = 0;
  while (recString[i] != 0) {
  
    /* Argument bit(s) */
    if (isalpha(recString[i])) {
      String name;
      String value;
      initString(&name);
      initString(&value);
      
      /* Set argument name */
      name.catData(&name, recString + i, 1);
    
      /* Fetch all corresponding bits in recognition string */
      int j = i;
      int lastJ = j;
      while (recString[j] != 0) {
        if (recString[j] == recString[i]) {
          int nextBit = getBufferBit(src, j);
          if (nextBit) value.catC(&value, "1");
          else value.catC(&value, "0");
          lastJ = j;
        }
        
        ++j;
      }
      
/*      printf("%d\n", src->pos(src));
      printf("%s\n", value.cStr(&value)); */
      
      /* Reverse argument bit order if requested */
      if (reverseBits) {
        String rev;
        initString(&rev);
        for (j = value.size(&value) - 1; j >= 0; j--) {
          rev.catData(&rev, value.cStr(&value) + j, 1);
        }
        
        /* Destroy un-reversed string */
        value.destroy(&value);
        
        /* Add to argument map */
        dst->insert(dst, name, rev);
      }
      else {
        /* Add to argument map */
        dst->insert(dst, name, value);
      }
      
      /* Skip over argument.
         Note that this ignores any other argument(s) that came in between
         non-adjacent sections!
         This is done to avoid redundant checks for existing names. */
      i = lastJ + 1;
    }
    /* Recognition bit */
    else {
      /* Skip */
      ++i;
    }
  }
  
  /* Compute actual byte length, accounting for byte alignment */
  int remainderBits = bitlen % k_bitsPerByte;
  return bytelen + ((remainderBits != 0) ? 1 : 0);
}

int collateOpArgs(BufferStream* src, MapSS* dst, const char* recString) {
  return collateOpArgsFull(src, dst, recString, 0, 0);
}

int collateOpArgsFull(BufferStream* src, MapSS* dst, const char* recString,
                      int reverseEnd, int reverseBits) {
  return collateOpArgsBuffer(src->getcur(src), dst, recString,
                             reverseEnd, reverseBits);
}

int getStreamBit(BufferStream* src, int index) {
  return getBufferBit(src->getcur(src), index);
}

int getBufferBit(char* src, int index) {
  int byteCount = index / k_bitsPerByte;
  char b = *(src + byteCount);
  int mask = (0x80 >> (index % k_bitsPerByte));
  if (b & mask) return 1;
  else return 0;
}

