#ifndef CPD_OP_INFO_ARRAY_H
#define CPD_OP_INFO_ARRAY_H


#include "modules/OpInfo.h"

/**
 * Pointer to a raw array and its length.
 */
typedef struct {

  OpInfo* array;
  int size;

} OpInfoArray;


#endif
