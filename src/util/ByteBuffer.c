#include "util/ByteBuffer.h"
#include "util/FileUtils.h"
#include "util/Logger.h"
#include <stdlib.h>
#include <stdio.h>

const static int msgbufSize = 512;

void ByteBufferLoad(ByteBuffer* obj, const char* filename) {
  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    error("Could not open file: ");
    error(filename);
    fatal();
  }
  
  unsigned int sz = fileSize(fp);
  obj->buffer_->resize(obj->buffer_, sz);
  fread((void*)(obj->data(obj)), sizeof(char), sz, fp);
  fclose(fp);
}

VectorChar* ByteBufferBuffer(ByteBuffer* obj) {
  return obj->buffer_;
}

char* ByteBufferData(ByteBuffer* obj) {
  return obj->buffer_->data(obj->buffer_);
}

unsigned int ByteBufferSize(ByteBuffer* obj) {
  return obj->buffer_->size(obj->buffer_);
}

/*unsigned int ByteBufferPos(ByteBuffer* obj) {
  return obj->pos_;
}

void ByteBufferSetPos(ByteBuffer* obj, unsigned int pos__) {
  obj->pos_ = pos__;
} */

void ByteBufferDestroy(ByteBuffer* obj) {
  
}

void initByteBuffer(ByteBuffer* obj) {
  obj->load = ByteBufferLoad;
  obj->buffer = ByteBufferBuffer;
  obj->data = ByteBufferData;
  obj->size = ByteBufferSize;
/*  obj->pos = ByteBufferPos;
  obj->setPos = ByteBufferSetPos; */
  obj->destroy = ByteBufferDestroy;
  
  obj->buffer_ = allocVectorChar();
}

ByteBuffer* allocByteBuffer() {
  ByteBuffer* obj = malloc(sizeof(ByteBuffer));
  initByteBuffer(obj);
  return obj;
}

void freeByteBuffer(ByteBuffer* obj) {
  obj->destroy(obj);
  freeVectorChar(obj->buffer_);
  
  free(obj);
}

