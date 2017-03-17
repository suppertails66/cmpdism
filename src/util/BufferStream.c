#include "util/BufferStream.h"

unsigned int BufferStreampos(BufferStream* obj) {
  return obj->pos_;
}

void BufferStreamseek(BufferStream* obj, unsigned int pos__) {
  obj->pos_ = pos__;
}

void BufferStreamseekOff(BufferStream* obj, int off) {
  obj->pos_ = obj->pos_ + off;
}

unsigned int BufferStreamtell(BufferStream* obj) {
  return obj->pos_;
}

unsigned int BufferStreamsize(BufferStream* obj) {
  return obj->buffer_->size(obj->buffer_);
}

unsigned int BufferStreamremaining(BufferStream* obj) {
  return (obj->size(obj) - obj->tell(obj));
}

char* BufferStreamgetcur(BufferStream* obj) {
  return obj->get(obj, obj->pos_);
}

char* BufferStreamget(BufferStream* obj, unsigned int offset) {
  return (obj->buffer_->data(obj->buffer_) + offset);
}

void BufferStreamload(BufferStream* obj, const char* filename) {
  if (obj->buffer_ != NULL) freeByteBuffer(obj->buffer_);
  obj->buffer_ = allocByteBuffer();
  obj->buffer_->load(obj->buffer_, filename);
}

void initBufferStream(BufferStream* obj) {
  obj->pos = BufferStreampos;
  obj->seek = BufferStreamseek;
  obj->seekOff = BufferStreamseekOff;
  obj->tell = BufferStreamtell;
  obj->size = BufferStreamsize;
  obj->remaining = BufferStreamremaining;
  obj->getcur = BufferStreamgetcur;
  obj->get = BufferStreamget;
  obj->load = BufferStreamload;

  obj->buffer_ = allocByteBuffer();
  obj->pos_ = 0;
}

BufferStream* allocBufferStream() {
  BufferStream* obj = malloc(sizeof(BufferStream));
  initBufferStream(obj);
  return obj;
}

BufferStream* allocBufferStreamFromFile(const char* filename) {
/*  BufferStream* obj = allocBufferStream();
  obj->buffer_->load(obj->buffer_, filename); */
  BufferStream* obj = allocBufferStream();
  obj->load(obj, filename);
  return obj;
}

void freeBufferStream(BufferStream* obj) {
  freeByteBuffer(obj->buffer_);
}
