#ifndef CPD_STRING_CONV_H
#define CPD_STRING_CONV_H


/* Converts a string to an int.
   Strings beginning with "0x" are treated as hexadecimal, strings beginning
   with "0" are treated as octal, and anything else is treated as decimal.
   Negative numbers aren't supported because they aren't needed at the
   moment. */
int cStringToInt(const char* str);


#endif
