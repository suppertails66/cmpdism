#ifndef CPD_STRING_H
#define CPD_STRING_H

#include "util/Vectors.h"

typedef struct String {
  
  /* PUBLIC */
  
  void (*destroy)(struct String* obj);
  const char* (*cStr)(struct String* obj);
  void (*clear)(struct String* obj);
  unsigned int (*size)(struct String* obj);
  char (*get)(struct String* obj, unsigned int pos);
  void (*set)(struct String* obj, unsigned int pos, char val);
  void (*catC)(struct String* obj, const char* c);
  void (*catData)(struct String* obj, const char* data, unsigned int dataSize);
  void (*catString)(struct String* obj, struct String* str);
  void (*catInt)(struct String* obj, int val, const char* fmt);
  int (*compare)(struct String* obj, struct String* str);
  void (*fromInt)(struct String* obj, int val, const char* fmt);
  void (*padToSize)(struct String* obj, char c, int targetSize);
  
  /* PRIVATE */
  
  VectorChar* data_;
  
} String;
  
void Stringdestroy(String* obj);
const char* StringcStr(String* obj);
void Stringclear(String* obj);
unsigned int Stringsize(String* obj);
char Stringget(String* obj, unsigned int pos);
void Stringset(String* obj, unsigned int pos, char val);
void StringcatC(String* obj, const char* c);
void StringcatData(String* obj, const char* data, unsigned int dataSize);
void StringcatString(String* obj, String* str);
void StringcatInt(String* obj, int val, const char* fmt);
int Stringcompare(String* obj, String* str);
void StringfromInt(String* obj, int val, const char* fmt);
void StringpadToSize(String* obj, char c, int targetSize);

void initString(String* obj);
String* allocString();
void freeString(String* obj);


#endif
