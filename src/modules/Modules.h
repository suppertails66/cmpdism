#ifndef CPD_MODULES_H
#define CPD_MODULES_H


#include "modules/DismModule.h"

typedef struct {

  char* moduleName;
  
  void (*moduleInit)(DismModule* obj);
  DismModule* (*moduleAlloc)();
  void (*moduleFree)(DismModule* obj);

} DismModuleInfo;

extern DismModuleInfo dismModules[];
extern int numDismModules;


#endif
