#ifndef CPD_CODE_MAP_H
#define CPD_CODE_MAP_H


#include "util/Vector.h"

/**
 * A CodeMap marks sections of a buffer as code or data.
 * It is simply a buffer of the same size as the source in which bytes that
 * are considered code are set to 0xFF, and bytes that are considered data are
 * set to 0x00.
 */
GENERATE_VECTOR_DECLARATION(CodeMap, char);


#endif
