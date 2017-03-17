#include "util/Vectors.h"
#include "modules/Opcode.h"

GENERATE_VECTOR_DEFINITION(VectorChar, char);
GENERATE_VECTOR_DEFINITION(VectorInt, int);



GENERATE_VECTOR_MEMBER_DEFINITIONS(VectorOpcode, struct Opcode);
GENERATE_VECTOR_INIT_MAIN_DEFINITION(VectorOpcode, struct Opcode);
GENERATE_VECTOR_ALLOC_DEFINITION(VectorOpcode, struct Opcode);
GENERATE_VECTOR_FREE_DEFINITION(VectorOpcode, struct Opcode);

void VectorOpcodedestroyAll(VectorOpcode* obj) {
  /* Free all contained Opcodes */
  unsigned int i;
  for (i = 0; i < obj->size(obj); i++) {
    /* shallow copy is OK; we just want to deallocate the pointers */
    Opcode op = obj->get(obj, i);
    op.destroy(&op);
  }
}

void initVectorOpcode(VectorOpcode* obj) {
  /* Do default vector initialization */
  initMainVectorOpcode(obj);
  
  /* Override destructor */
  obj->destroyAll = VectorOpcodedestroyAll;
}



GENERATE_VECTOR_MEMBER_DEFINITIONS(VectorOpcodeP, struct Opcode*);
GENERATE_VECTOR_INIT_MAIN_DEFINITION(VectorOpcodeP, struct Opcode*);
GENERATE_VECTOR_ALLOC_DEFINITION(VectorOpcodeP, struct Opcode*);
GENERATE_VECTOR_FREE_DEFINITION(VectorOpcodeP, struct Opcode*);

void VectorOpcodePdestroyAll(VectorOpcodeP* obj) {
  /* Free all contained Opcodes */
  unsigned int i;
  for (i = 0; i < obj->size(obj); i++) {
    freeOpcode(obj->get(obj, i));
  }
}

void initVectorOpcodeP(VectorOpcodeP* obj) {
  /* Do default vector initialization */
  initMainVectorOpcodeP(obj);
  
  /* Override destructor */
  obj->destroyAll = VectorOpcodePdestroyAll;
}



GENERATE_VECTOR_DEFINITION(VectorOpInfoArray, OpInfoArray);
