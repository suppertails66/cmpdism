#include "util/String.h"
#include <stdio.h>

void Stringdestroy(String* obj) {
  freeVectorChar(obj->data_);
}

const char* StringcStr(String* obj) {
  return obj->data_->data(obj->data_);
}

void Stringclear(String* obj) {
  obj->data_->clear(obj->data_);
  
  /* null terminator */
  obj->data_->pushBack(obj->data_, 0);
}

unsigned int Stringsize(String* obj) {
  /* account for null terminator */
  return obj->data_->size(obj->data_) - 1; 
/*  return strlen(obj->data_->getP(obj->data_, 0)); */
}

char Stringget(String* obj, unsigned int pos) {
  return obj->data_->get(obj->data_, pos);
}

void Stringset(String* obj, unsigned int pos, char val) {
  obj->data_->set(obj->data_, pos, val);
}

void StringcatC(String* obj, const char* c) {
/*  unsigned int oldSize = obj->size(obj);
  unsigned int newSize = oldSize + strlen(c);
  obj->data_->resize(obj->data_, newSize + 1);
  strcpy(obj->data_->data(obj->data_) + oldSize, c); */
  
  obj->catData(obj, c, strlen(c));
}

void StringcatData(String* obj, const char* data, unsigned int dataSize) {
/*  unsigned int oldSize = obj->size(obj);
  unsigned int newSize = oldSize + dataSize;
  obj->data_->resize(obj->data_, newSize + 1);
  memcpy(obj->data_->data(obj->data_) + oldSize, data, dataSize);
  obj->data_->set(obj->data_, newSize, 0); */
  
  unsigned int oldSize = obj->size(obj);
  unsigned int newSize = oldSize + dataSize;
  
  /* Do we need to recap the vector? */
  if (obj->data_->capacity(obj->data_) < newSize + 1) {
/*    obj->data_->resize(obj->data_, newSize + 1); */
    /* If we have to recap, double the size of the vector and add the
       size of the concatenated data to make sure we have enough room
       and leave space for expansion */
    obj->data_->recap(obj->data_,
                      (obj->data_->capacity(obj->data_) * 2) + dataSize + 1);
  }
  
  memcpy(obj->data_->data(obj->data_) + oldSize, data, dataSize);
  obj->data_->set(obj->data_, newSize, 0);
  
  /* Update size */
  obj->data_->setSize(obj->data_, newSize + 1);
}

void StringcatString(String* obj, String* str) {
/*  obj->catC(obj, str->cStr(str)); */
  obj->catData(obj, str->cStr(str), str->size(str));
}

void StringcatInt(String* obj, int val, const char* fmt) {
  String temp;
  initString(&temp);
  temp.fromInt(&temp, val, fmt);
  obj->catString(obj, &temp);
  temp.destroy(&temp);
}

void StringcatChar(String* obj, char val) {
  char buf[2];
  memset(buf, 0, 2);
  buf[0] = val;
  obj->catC(obj, buf);
}

int Stringcompare(String* obj, String* str) {
  /* this isn't stdlib compliant, but is probably good enough for us */

  if (obj->size(obj) != str->size(str)) return 1;
  
  int i;
  for (i = 0; i < str->size(str); i++) {
    if (obj->get(obj, i) != str->get(str, i)) return i;
  }
  
  return 0;
}

void StringfromInt(String* obj, int val, const char* fmt) {
  int required = snprintf(NULL, 0, fmt, val);
  
  if (required + 1 >= obj->data_->capacity(obj->data_)) {
    obj->data_->resize(obj->data_, required + 1);
  }
  else {
    obj->data_->setSize(obj->data_, required + 1);
  }
  
  snprintf(obj->data_->data(obj->data_), required + 1, fmt, val);
  
/*  int required = snprintf(NULL, 0, fmt, val);
  
  obj->data_->resize(obj->data_, required + 1);
  snprintf(obj->data_->data(obj->data_), obj->data_->size(obj->data_), fmt, val); */
}

void StringpadToSize(String* obj, char c, int targetSize) {
  if (obj->size(obj) >= targetSize) return;
  
  unsigned int oldSize = obj->size(obj);
  
  /* Check capacity */
  if (obj->data_->capacity(obj->data_) < targetSize + 1) {
    /* Not enough: resize to what we need */
    obj->data_->resize(obj->data_, targetSize + 1);
  }
  else {
    /* Enough: increase nominal size */
    obj->data_->setSize(obj->data_, targetSize + 1);
  }
  
  memset(obj->data_->getP(obj->data_, oldSize), c, targetSize - oldSize);
  obj->data_->set(obj->data_, targetSize, 0);
  
/*  if (obj->size(obj) >= targetSize) return;

  obj->data_->set(obj->data_, obj->size(obj), c);

  while (obj->size(obj) < targetSize) {
    obj->data_->pushBack(obj->data_, c);
  }
  
  obj->data_->pushBack(obj->data_, 0); */
}

void initString(String* obj) {
  obj->destroy = Stringdestroy;
  obj->cStr = StringcStr;
  obj->clear = Stringclear;
  obj->size = Stringsize;
  obj->get = Stringget;
  obj->set = Stringset;
  obj->catC = StringcatC;
  obj->catData = StringcatData;
  obj->catString = StringcatString;
  obj->catInt = StringcatInt;
  obj->catChar = StringcatChar;
  obj->compare = Stringcompare;
  obj->fromInt = StringfromInt;
  obj->padToSize = StringpadToSize;
  
  obj->data_ = allocVectorChar();
  
  /* null terminator */
  obj->clear(obj);
}

String* allocString() {
  String* obj = malloc(sizeof(String));
  initString(obj);
  return obj;
}

void freeString(String* obj) {
  obj->destroy(obj);

  free(obj);
}
