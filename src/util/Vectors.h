#ifndef CPD_VECTORS_H
#define CPD_VECTORS_H

#include "util/Vector.h"
#include "modules/OpInfoArray.h"

struct Opcode;

GENERATE_VECTOR_DECLARATION(VectorChar, char);
GENERATE_VECTOR_DECLARATION(VectorInt, int);
GENERATE_VECTOR_DECLARATION(VectorOpcode, struct Opcode);
GENERATE_VECTOR_DECLARATION(VectorOpcodeP, struct Opcode*);
GENERATE_VECTOR_DECLARATION(VectorOpInfoArray, OpInfoArray);


#endif
