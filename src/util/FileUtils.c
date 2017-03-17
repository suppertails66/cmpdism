#include "util/FileUtils.h"
#include <stdlib.h>

unsigned int fileSize(FILE* fp) {
  if (fp == NULL) return 0;
  
  long int pos = ftell(fp);
  fseek(fp, 0, SEEK_END);
  long int end = ftell(fp);
  fseek(fp, pos, SEEK_SET);
  
  return (unsigned int)end;
}
