#include "modules/DismSettings.h"
#include "modules/DismModule.h"
#include "util/StringConv.h"
#include <stdlib.h>

void initDismSettings(DismSettings* obj) {
  obj->argc = 0;
  obj->argv = NULL;
  obj->firstFile = NULL;
  obj->secondFile = NULL;
  obj->firstFileStartOffset = 0;
  obj->secondFileStartOffset = 0;
  obj->firstFileEndOffset = -1;
  obj->secondFileEndOffset = -1;
  obj->fileLoadAddr = 0;
  
  obj->firstFileExternalCodeMap = NULL;
  obj->secondFileExternalCodeMap = NULL;
  obj->requiredCodeMapResumeOps = 30;
  
  obj->loadAddress = NULL;
  
  obj->intelligentCodeDetection = 0;
  
  obj->singleSrcAddrW = 6;
  obj->singleShowRaw = 1;
  obj->singleMiddleWidth = 12;
  obj->dualSrcAddrW = 6;
  obj->dualShowRaw = 0;
  obj->dualMiddleWidth = 2;
  obj->dualSeparationW = 40;
  
  obj->addressChangesDistinct = 0;
  obj->constantChangesDistinct = 1;
  obj->requiredSequentialOps = 16;
  obj->maxChangeBlockSize = 128;
  
  obj->module = NULL;
}

DismSettings* allocDismSettings() {
  DismSettings* obj = malloc(sizeof(DismSettings));
  initDismSettings(obj);
  return obj;
}

void freeDismSettings(DismSettings* obj) {
  free(obj);
}

void fillDismSettings(DismSettings* obj, int argc, char* argv[]) {
  int i;
  
  obj->argc = argc;
  obj->argv = argv;
  
  /* Toggles */
  for (i = 2; i < argc; i++) {
    if (strcmp(argv[i], "--addresses-distinct") == 0) {
      obj->addressChangesDistinct = 1;
    }
    else if (strcmp(argv[i], "--addresses-similar") == 0) {
      obj->addressChangesDistinct = 0;
    }
    else if (strcmp(argv[i], "--constants-distinct") == 0) {
      obj->constantChangesDistinct = 1;
    }
    else if (strcmp(argv[i], "--constants-similar") == 0) {
      obj->constantChangesDistinct = 0;
    }
    else if (strcmp(argv[i], "--raw-bytes") == 0) {
      obj->singleShowRaw = 1;
      obj->dualShowRaw = 1;
    }
    else if (strcmp(argv[i], "--generate-codemaps") == 0) {
      obj->intelligentCodeDetection = 1;
    }
  }
  
  /* 1-argument parameters */
  for (i = 2; i < argc - 1; i++) {
    if (strcmp(argv[i], "-i1") == 0) {
      obj->firstFile = argv[i + 1];
    }
    else if (strcmp(argv[i], "-i2") == 0) {
      obj->secondFile = argv[i + 1];
    }
    else if (strcmp(argv[i], "-s1") == 0) {
      obj->firstFileStartOffset = cStringToInt(argv[i + 1]);
    }
    else if (strcmp(argv[i], "-s2") == 0) {
      obj->secondFileStartOffset = cStringToInt(argv[i + 1]);
    }
    else if (strcmp(argv[i], "-f1") == 0) {
      obj->firstFileEndOffset = cStringToInt(argv[i + 1]);
    }
    else if (strcmp(argv[i], "-f2") == 0) {
      obj->secondFileEndOffset = cStringToInt(argv[i + 1]);
    }
    else if (strcmp(argv[i], "-o") == 0) {
      obj->fileLoadAddr = cStringToInt(argv[i + 1]);
    }
    else if ((strcmp(argv[i], "-r") == 0)
             || (strcmp(argv[i], "--realign-len") == 0)) {
      obj->requiredSequentialOps = cStringToInt(argv[i + 1]);
    }
    else if ((strcmp(argv[i], "-m") == 0)
             || (strcmp(argv[i], "--max-search-len") == 0)) {
      obj->maxChangeBlockSize = cStringToInt(argv[i + 1]);
    }
    else if (strcmp(argv[i], "--addr-width") == 0) {
      obj->singleSrcAddrW = cStringToInt(argv[i + 1]);
      obj->dualSrcAddrW = cStringToInt(argv[i + 1]);
    }
    else if (strcmp(argv[i], "--middle-width") == 0) {
      obj->singleMiddleWidth = cStringToInt(argv[i + 1]);
      obj->dualMiddleWidth = cStringToInt(argv[i + 1]);
    }
    else if (strcmp(argv[i], "--dual-separation") == 0) {
      obj->dualSeparationW = cStringToInt(argv[i + 1]);
    }
    else if (strcmp(argv[i], "--codemap-seq-req") == 0) {
      obj->requiredCodeMapResumeOps = cStringToInt(argv[i + 1]);
    }
  }
  
}
