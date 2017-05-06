#ifndef CPD_BYTE_CONV_H
#define CPD_BYTE_CONV_H


typedef enum Endianness {
  littleEnd,
  bigEnd
} Endianness;

typedef enum Signedness {
  sign,
  nosign
} Signedness;

int fromBytes(const char* src,
              int size,
              Endianness endianness,
              Signedness signedness);

void toBytes(char* dst,
             int value,
             int size,
             Endianness endianness,
             Signedness signedness);

int reverseEndianness(int value, int numbytes);


#endif
