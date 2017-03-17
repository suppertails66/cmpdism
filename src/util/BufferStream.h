#ifndef CPD_BUFFER_STREAM_H
#define CPD_BUFFER_STREAM_H


#include "util/ByteBuffer.h"

typedef struct BufferStream {

  /* PUBLIC */

  unsigned int (*pos)(struct BufferStream* obj);
  void (*seek)(struct BufferStream* obj, unsigned int pos__);
  void (*seekOff)(struct BufferStream* obj, int off);
  unsigned int (*tell)(struct BufferStream* obj);
  unsigned int (*size)(struct BufferStream* obj);
  unsigned int (*remaining)(struct BufferStream* obj);
  char* (*getcur)(struct BufferStream* obj);
  char* (*get)(struct BufferStream* obj, unsigned int offset);
  void (*load)(struct BufferStream* obj, const char* filename);
  
  /* PRIVATE */
  
  ByteBuffer* buffer_;
  unsigned int pos_;

} BufferStream;

unsigned int BufferStreampos(BufferStream* obj);
void BufferStreamseek(BufferStream* obj, unsigned int pos__);
void BufferStreamseekOff(BufferStream* obj, int off);
unsigned int BufferStreamtell(BufferStream* obj);
unsigned int BufferStreamsize(BufferStream* obj);
unsigned int BufferStreamremaining(BufferStream* obj);
char* BufferStreamgetcur(BufferStream* obj);
char* BufferStreamget(BufferStream* obj, unsigned int offset);
void BufferStreamload(BufferStream* obj, const char* filename);

void initBufferStream(BufferStream* obj);
BufferStream* allocBufferStream();
BufferStream* allocBufferStreamFromFile(const char* filename);
void freeBufferStream(BufferStream* obj);


#endif
