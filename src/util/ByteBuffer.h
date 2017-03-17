#ifndef CPD_BYTE_BUFFER_H
#define CPD_BYTE_BUFFER_H

#include "util/Vectors.h"

/**
 * Container format for files read from (and to be written to) disk.
 */
typedef struct ByteBuffer {

  /* PUBLIC */
  
  void (*load)(struct ByteBuffer* obj, const char* filename);
  VectorChar* (*buffer)(struct ByteBuffer* obj);
  char* (*data)(struct ByteBuffer* obj);
  unsigned int (*size)(struct ByteBuffer* obj);
/*  unsigned int (*pos)(struct ByteBuffer* obj);
  void (*setPos)(struct ByteBuffer* obj, unsigned int pos__); */
  
  /* PRIVATE */
  
  void (*destroy)(struct ByteBuffer* obj);
  unsigned int pos_;
  VectorChar* buffer_;
} ByteBuffer;

void ByteBufferLoad(ByteBuffer* obj, const char* filename);
VectorChar* ByteBufferBuffer(ByteBuffer* obj);
char* ByteBufferData(ByteBuffer* obj);
unsigned int ByteBufferSize(ByteBuffer* obj);
/*unsigned int ByteBufferPos(ByteBuffer* obj);
void ByteBufferSetPos(ByteBuffer* obj, unsigned int pos__); */
void ByteBufferDestroy(ByteBuffer* obj);

void initByteBuffer(ByteBuffer* obj);
ByteBuffer* allocByteBuffer();
void freeByteBuffer(ByteBuffer* obj);


#endif
