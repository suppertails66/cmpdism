#ifndef CPD_VECTOR_H
#define CPD_VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VECTOR_INITIAL_SLOTS 8

/* Vectors generated by these macros are rather fragile if dynamic memory
   gets involved. For classes that contain dynamically allocated members,
   do not shrink the vector using resize() without handling any needed
   deallocations manually, or else memory will be leaked. In the future,
   destroy() should instead be made to destroy only a single contained
   object so this can be handled automatically.
   
   Deallocation will be performed on all contained objects using destroy()
   when the vector is freed. Make sure this function is appropriately
   overriden if necessary. */
 
#define GENERATE_VECTOR_INIT_DECLARATION(VECTOR_NAME, VECTOR_TYPE) \
void initMain ## VECTOR_NAME(VECTOR_NAME* vector); \
void init ## VECTOR_NAME(VECTOR_NAME* vector);

#define GENERATE_VECTOR_ALLOC_DECLARATION(VECTOR_NAME, VECTOR_TYPE) \
VECTOR_NAME* alloc ## VECTOR_NAME();

#define GENERATE_VECTOR_FREE_DECLARATION(VECTOR_NAME, VECTOR_TYPE) \
void free ## VECTOR_NAME(VECTOR_NAME* vector);


#define GENERATE_VECTOR_CONSTRUCTOR_DECLARATIONS(VECTOR_NAME, VECTOR_TYPE) \
GENERATE_VECTOR_INIT_DECLARATION(VECTOR_NAME, VECTOR_TYPE); \
GENERATE_VECTOR_ALLOC_DECLARATION(VECTOR_NAME, VECTOR_TYPE); \
GENERATE_VECTOR_FREE_DECLARATION(VECTOR_NAME, VECTOR_TYPE);



#define GENERATE_VECTOR_MEMBER_DECLARATIONS(VECTOR_NAME, VECTOR_TYPE) \
VECTOR_TYPE VECTOR_NAME ## Get(VECTOR_NAME* vector, unsigned int index); \
VECTOR_TYPE* VECTOR_NAME ## GetP(VECTOR_NAME* vector, unsigned int index); \
void VECTOR_NAME ## Set(VECTOR_NAME* vector, unsigned int index, \
                        VECTOR_TYPE value); \
void VECTOR_NAME ## PushBack(VECTOR_NAME* vector, VECTOR_TYPE s); \
void VECTOR_NAME ## Resize(VECTOR_NAME* vector, unsigned int newSize); \
void VECTOR_NAME ## Recap(VECTOR_NAME* vector, unsigned int newCap); \
void VECTOR_NAME ## Clear(VECTOR_NAME* vector); \
void VECTOR_NAME ## Fill(VECTOR_NAME* vector, VECTOR_TYPE s); \
void VECTOR_NAME ## DestroyAll(VECTOR_NAME* vector); \
VECTOR_TYPE* VECTOR_NAME ## Data(VECTOR_NAME* vector); \
int VECTOR_NAME ## Size(VECTOR_NAME* vector); \
int VECTOR_NAME ## Capacity(VECTOR_NAME* vector); \
void VECTOR_NAME ## setSize(VECTOR_NAME* vector, unsigned int size__); \


#define GENERATE_VECTOR_STRUCT_DECLARATION(VECTOR_NAME, VECTOR_TYPE) \
  typedef struct VECTOR_NAME { \
    VECTOR_TYPE (*get)(struct VECTOR_NAME* vector, unsigned int index); \
    VECTOR_TYPE* (*getP)(struct VECTOR_NAME* vector, unsigned int index); \
    void (*set)(struct VECTOR_NAME* vector, unsigned int index, \
                VECTOR_TYPE value); \
    void (*pushBack)(struct VECTOR_NAME* vector, VECTOR_TYPE s); \
    void (*resize)(struct VECTOR_NAME* vector, unsigned int newSize); \
    void (*recap)(struct VECTOR_NAME* vector, unsigned int newcap); \
    void (*clear)(struct VECTOR_NAME* vector); \
    void (*fill)(struct VECTOR_NAME* vector, VECTOR_TYPE s); \
    VECTOR_TYPE* (*data)(struct VECTOR_NAME* vector); \
    int (*size)(struct VECTOR_NAME* vector); \
    int (*capacity)(struct VECTOR_NAME* vector); \
    void (*setSize)(struct VECTOR_NAME* vector, unsigned int size__); \
      \
    void (*destroyAll)(struct VECTOR_NAME* vector); \
      \
    VECTOR_TYPE* data_; \
    unsigned int size_; \
    unsigned int capacity_; \
      \
  } VECTOR_NAME;


#define GENERATE_VECTOR_DECLARATION(VECTOR_NAME, VECTOR_TYPE) \
GENERATE_VECTOR_STRUCT_DECLARATION(VECTOR_NAME, VECTOR_TYPE); \
GENERATE_VECTOR_MEMBER_DECLARATIONS(VECTOR_NAME, VECTOR_TYPE); \
GENERATE_VECTOR_CONSTRUCTOR_DECLARATIONS(VECTOR_NAME, VECTOR_TYPE);



#define GENERATE_VECTOR_MEMBER_DEFINITIONS(VECTOR_NAME, VECTOR_TYPE) \
 \
VECTOR_TYPE VECTOR_NAME ## Get(VECTOR_NAME* vector, unsigned int index) { \
  return vector->data_[index]; \
} \
 \
VECTOR_TYPE* VECTOR_NAME ## GetP(VECTOR_NAME* vector, unsigned int index) { \
  return &(vector->data_[index]); \
} \
 \
void VECTOR_NAME ## Set(VECTOR_NAME* vector, unsigned int index, \
                        VECTOR_TYPE value) { \
  vector->data_[index] = value; \
} \
 \
void VECTOR_NAME ## PushBack(VECTOR_NAME* vector, VECTOR_TYPE s) { \
  if (vector->size_ >= vector->capacity_) { \
    unsigned int newCapacity = vector->capacity_ * 2; \
    VECTOR_TYPE* newbuf = malloc(sizeof(VECTOR_TYPE) * newCapacity); \
    memcpy(newbuf, vector->data_, vector->capacity_ * sizeof(VECTOR_TYPE)); \
    free(vector->data_); \
    vector->data_ = newbuf; \
    vector->capacity_ = newCapacity; \
  } \
 \
  vector->data_[(vector->size_)++] = s; \
} \
 \
void VECTOR_NAME ## Resize(VECTOR_NAME* vector, unsigned int newSize) { \
  vector->recap(vector, newSize); \
  vector->size_ = newSize; \
} \
 \
void VECTOR_NAME ## Recap(VECTOR_NAME* vector, unsigned int newCap) { \
  VECTOR_TYPE* buffer = malloc(newCap * sizeof(VECTOR_TYPE)); \
  memcpy(buffer, vector->data_, \
         (newCap < vector->size_) ? newCap : vector->size_); \
  free(vector->data_); \
  vector->data_ = buffer; \
  vector->capacity_ = newCap; \
} \
 \
void VECTOR_NAME ## Clear(VECTOR_NAME* vector) { \
  VECTOR_NAME ## DestroyAll(vector); \
  free(vector->data_); \
  vector->data_ = malloc(VECTOR_INITIAL_SLOTS * sizeof(VECTOR_TYPE)); \
  vector->size_ = 0; \
  vector->capacity_ = VECTOR_INITIAL_SLOTS; \
} \
 \
void VECTOR_NAME ## Fill(VECTOR_NAME* vector, VECTOR_TYPE s) { \
  int i; \
  for (i = 0; i < vector->size_; i++) { \
    vector->set(vector, i, s); \
  } \
} \
 \
void VECTOR_NAME ## DestroyAll(VECTOR_NAME* vector) { \
  \
} \
 \
VECTOR_TYPE* VECTOR_NAME ## Data(VECTOR_NAME* vector) { \
  return vector->data_; \
} \
 \
int VECTOR_NAME ## Size(VECTOR_NAME* vector) { \
  return vector->size_; \
} \
 \
int VECTOR_NAME ## Capacity(VECTOR_NAME* vector) { \
  return vector->capacity_; \
} \
void VECTOR_NAME ## setSize(VECTOR_NAME* vector, unsigned int size__) { \
  vector->size_ = size__; \
} 

#define GENERATE_VECTOR_INIT_MAIN_DEFINITION(VECTOR_NAME, VECTOR_TYPE) \
void initMain ## VECTOR_NAME(VECTOR_NAME* vector) { \
  vector->get = VECTOR_NAME ## Get; \
  vector->getP = VECTOR_NAME ## GetP; \
  vector->set = VECTOR_NAME ## Set; \
  vector->pushBack = VECTOR_NAME ## PushBack; \
  vector->resize = VECTOR_NAME ## Resize; \
  vector->recap = VECTOR_NAME ## Recap; \
  vector->clear = VECTOR_NAME ## Clear; \
  vector->fill = VECTOR_NAME ## Fill; \
  vector->data = VECTOR_NAME ## Data; \
  vector->size = VECTOR_NAME ## Size; \
  vector->capacity = VECTOR_NAME ## Capacity; \
  vector->setSize = VECTOR_NAME ## setSize; \
   \
  vector->destroyAll = VECTOR_NAME ## DestroyAll; \
    \
  vector->data_ = NULL; \
  vector->clear(vector); \
}

#define GENERATE_VECTOR_INIT_DEFINITION(VECTOR_NAME, VECTOR_TYPE) \
void init ## VECTOR_NAME(VECTOR_NAME* vector) { \
  initMain ## VECTOR_NAME(vector); \
}

#define GENERATE_VECTOR_ALLOC_DEFINITION(VECTOR_NAME, VECTOR_TYPE) \
VECTOR_NAME* alloc ## VECTOR_NAME() { \
  VECTOR_NAME* vector = malloc(sizeof(VECTOR_NAME)); \
  init ## VECTOR_NAME(vector); \
  return vector; \
} \

#define GENERATE_VECTOR_FREE_DEFINITION(VECTOR_NAME, VECTOR_TYPE) \
void free ## VECTOR_NAME(VECTOR_NAME* vector) { \
  vector->destroyAll(vector); \
  free(vector->data_); \
  free(vector); \
}

#define GENERATE_VECTOR_CONSTRUCTOR_DEFINITIONS(VECTOR_NAME, VECTOR_TYPE) \
GENERATE_VECTOR_INIT_MAIN_DEFINITION(VECTOR_NAME, VECTOR_TYPE); \
GENERATE_VECTOR_INIT_DEFINITION(VECTOR_NAME, VECTOR_TYPE); \
GENERATE_VECTOR_ALLOC_DEFINITION(VECTOR_NAME, VECTOR_TYPE); \
GENERATE_VECTOR_FREE_DEFINITION(VECTOR_NAME, VECTOR_TYPE);

#define GENERATE_VECTOR_DEFINITION(VECTOR_NAME, VECTOR_TYPE) \
GENERATE_VECTOR_MEMBER_DEFINITIONS(VECTOR_NAME, VECTOR_TYPE); \
GENERATE_VECTOR_CONSTRUCTOR_DEFINITIONS(VECTOR_NAME, VECTOR_TYPE);


#endif
