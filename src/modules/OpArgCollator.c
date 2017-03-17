#include "modules/OpArgCollator.h"
#include "modules/Consts.h"
#include <string.h>
#include <ctype.h>

int collateOpArgs(BufferStream* src, MapSS* dst, const char* recString) {
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
          int nextBit = getStreamBit(src, j);
          if (nextBit) value.catC(&value, "1");
          else value.catC(&value, "0");
          lastJ = j;
        }
        
        ++j;
      }
      
/*      printf("%d\n", src->pos(src));
      printf("%s\n", value.cStr(&value)); */
      
      /* Add to argument map */
      dst->insert(dst, name, value);
      
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
  int remainderBits = i % k_bitsPerByte;
  return (i / k_bitsPerByte) + ((remainderBits != 0) ? 1 : 0);
}

int getStreamBit(BufferStream* src, int index) {
  int byteCount = index / k_bitsPerByte;
  char b = *(src->getcur(src) + byteCount);
  int mask = (0x80 >> (index % k_bitsPerByte));
  if (b & mask) return 1;
  else return 0;
}

