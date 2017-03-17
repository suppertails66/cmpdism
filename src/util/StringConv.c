#include "util/StringConv.h"
#include <stdio.h>
#include <string.h>

typedef enum {
  
  stringConvBaseDec,
  stringConvBaseHex,
  stringConvBaseOct
  
} StringConvBase;

int cStringToInt(const char* str) {
  int len = strlen(str);
  if (len == 0) return 0;
  
  /* Check base */
  
  StringConvBase base = stringConvBaseDec;
  
  if ((len >= 2)
      && (str[0] == '0')
      && ((str[1] == 'x')
          || (str[1] == 'X'))
      ) {
    base = stringConvBaseHex;
  }
  else if ((len >= 1)
           && (str[0] == '0')) {
    base = stringConvBaseOct;
  }
  
  int result;
  switch (base) {
  case stringConvBaseHex:
    sscanf(str + 2, "%x", &result);
    break;
  case stringConvBaseOct:
    sscanf(str + 1, "%o", &result);
    break;
  default:
    sscanf(str, "%d", &result);
    break;
  }
  
  return result;
}
