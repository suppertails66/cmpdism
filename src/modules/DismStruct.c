#include "modules/DismStruct.h"
#include <stdlib.h>

void initDismStruct(DismStruct* obj) {
  obj->stream = allocBufferStream();
/*  obj->codeMap = allocCodeMap(); */
  obj->codeMap = NULL;
  initVectorOpcode(&(obj->opcodes));
}

DismStruct* allocDismStruct() {
  DismStruct* obj = malloc(sizeof(DismStruct));
  initDismStruct(obj);
  return obj;
}

void freeDismStruct(DismStruct* obj) {
  freeBufferStream(obj->stream);
  if (obj->codeMap != NULL) freeCodeMap(obj->codeMap);
  obj->opcodes.destroyAll(&(obj->opcodes));

  free(obj);
}
