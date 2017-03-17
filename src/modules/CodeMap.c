#include "modules/CodeMap.h"
#include "modules/Consts.h"

GENERATE_VECTOR_MEMBER_DEFINITIONS(CodeMap, char);
GENERATE_VECTOR_FREE_DEFINITION(CodeMap, char);
GENERATE_VECTOR_INIT_MAIN_DEFINITION(CodeMap, char);
GENERATE_VECTOR_ALLOC_DEFINITION(CodeMap, char);

void initCodeMap(CodeMap* obj) {
  /* Do default vector initialization */
  initMainCodeMap(obj);
  
  /* Initialize map to all code */
  obj->fill(obj, k_codeMapCode);
}
