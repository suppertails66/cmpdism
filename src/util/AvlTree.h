#ifndef CPD_AVL_TREE_H
#define CPD_AVL_TREE_H


#include <stdlib.h>
#include <math.h>

/* This AVL tree is somewhat based upon the C++ sample implementation
   provided in Goodrich, Tamassia, and Mount's Data Structure & Algorithms
   in C++ (2nd ed.). As their example was frankly rather poor and lacked
   a definition for the key restructure() function -- and indeed, the way
   their class is designed, any implementation of that function would be
   needlessly inefficient -- much of this is original. */
   
/* Note that the remove() function isn't yet implemented. */


/* --------------------- Declarations --------------------- */


#define GENERATE_AVL_TREE_NODE_STRUCT(TREE_NAME, KEY_TYPE, VALUE_TYPE) \
  typedef struct TREE_NAME ## Node { \
    struct TREE_NAME ## Node* parent_; \
    struct TREE_NAME ## Node* left_; \
    struct TREE_NAME ## Node* right_; \
    int height_; \
   \
    KEY_TYPE key; \
    VALUE_TYPE value; \
   \
  } TREE_NAME ## Node; \
  
  
  
#define GENERATE_AVL_TREE_NODE_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
int getHeight ## TREE_NAME ## Node(TREE_NAME ## Node* node); \
 \
void resetHeight ## TREE_NAME ## Node(TREE_NAME ## Node* node); \
 \
int isBalanced ## TREE_NAME ## Node(TREE_NAME ## Node* node); \
 \
void destroyAll ## TREE_NAME ## Node(TREE_NAME ## Node* node, \
  void (*destroyKey_)(KEY_TYPE* key), \
  void (*destroyValue_)(VALUE_TYPE* value));

#define GENERATE_AVL_TREE_NODE_INIT_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
TREE_NAME ## Node* alloc ## TREE_NAME ## Node(); \
 \
TREE_NAME ## Node* alloc ## TREE_NAME ## NodeFull( \
    TREE_NAME ## Node* parent, \
    TREE_NAME ## Node* left, \
    TREE_NAME ## Node* right, \
    KEY_TYPE key, \
    VALUE_TYPE value); \
 \
void free ## TREE_NAME ## Node(TREE_NAME ## Node* obj);



#define GENERATE_AVL_TREE_STRUCT(TREE_NAME, KEY_TYPE, VALUE_TYPE) \
typedef struct TREE_NAME { \
 \
  void (*insert)(struct TREE_NAME* obj, KEY_TYPE key, VALUE_TYPE value); \
  void (*remove)(struct TREE_NAME* obj, KEY_TYPE key); \
  TREE_NAME ## Node* (*find)(struct TREE_NAME* obj, KEY_TYPE key); \
  void (*destroy)(struct TREE_NAME* obj); \
   \
  int (*compare_)(KEY_TYPE key1, KEY_TYPE key2); \
   \
  int (*equals_)(KEY_TYPE key1, KEY_TYPE key2); \
   \
  void (*rebalance_)(struct TREE_NAME* obj, \
                     TREE_NAME ## Node* node); \
  void (*destroyKey_)(KEY_TYPE* key); \
  void (*destroyValue_)(VALUE_TYPE* value); \
   \
  TREE_NAME ## Node* (*rotateLeft_)( \
                     struct TREE_NAME* obj, \
                     TREE_NAME ## Node* low, \
                     TREE_NAME ## Node* high); \
   \
  TREE_NAME ## Node* (*rotateRight_)( \
                     struct TREE_NAME* obj, \
                     TREE_NAME ## Node* low, \
                     TREE_NAME ## Node* high); \
 \
  TREE_NAME ## Node* root_; \
  unsigned int size_; \
 \
} TREE_NAME; \



#define GENERATE_AVL_TREE_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
void TREE_NAME ## insert(TREE_NAME* obj, KEY_TYPE key, VALUE_TYPE value); \
 \
void TREE_NAME ## remove(TREE_NAME* obj, KEY_TYPE key); \
 \
TREE_NAME ## Node* TREE_NAME ## find( \
    TREE_NAME* obj, KEY_TYPE key); \
   \
void TREE_NAME ## rebalance(TREE_NAME* obj, \
                                TREE_NAME ## Node* node); \
void TREE_NAME ## destroy(TREE_NAME* obj); \
   \
TREE_NAME ## Node* TREE_NAME ## rotateLeft( \
                     TREE_NAME* obj, \
                     TREE_NAME ## Node* low, \
                     TREE_NAME ## Node* high); \
   \
TREE_NAME ## Node* TREE_NAME ## rotateRight( \
                     TREE_NAME* obj, \
                     TREE_NAME ## Node* low, \
                     TREE_NAME ## Node* high);

#define GENERATE_AVL_TREE_INIT_FUNCTION_DECLARATION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE, PREFIX) \
void PREFIX ## init ## TREE_NAME(TREE_NAME* obj, \
                           int (*compare__)(KEY_TYPE key1, KEY_TYPE key2), \
                           int (*equals__)(KEY_TYPE key1, KEY_TYPE key2));

#define GENERATE_AVL_TREE_ALLOC_FUNCTION_DECLARATION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
TREE_NAME* alloc ## TREE_NAME( \
                           int (*compare__)(KEY_TYPE key1, KEY_TYPE key2), \
                           int (*equals__)(KEY_TYPE key1, KEY_TYPE key2));

#define GENERATE_AVL_TREE_FREE_FUNCTION_DECLARATION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
void free ## TREE_NAME(TREE_NAME* obj);

#define GENERATE_AVL_TREE_INIT_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
  GENERATE_AVL_TREE_INIT_FUNCTION_DECLARATION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE,); \
  GENERATE_AVL_TREE_ALLOC_FUNCTION_DECLARATION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_FREE_FUNCTION_DECLARATION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE);


#define GENERATE_AVL_TREE_DECLARATIONS(TREE_NAME, KEY_TYPE, VALUE_TYPE) \
  GENERATE_AVL_TREE_NODE_STRUCT(TREE_NAME, KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_NODE_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_NODE_INIT_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_STRUCT(TREE_NAME, KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_INIT_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE);

#define GENERATE_AVL_TREE_DECLARATIONS_WITH_DESTRUCTORS(TREE_NAME, \
    KEY_TYPE, VALUE_TYPE) \
  GENERATE_AVL_TREE_NODE_STRUCT(TREE_NAME, KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_NODE_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_NODE_INIT_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_STRUCT(TREE_NAME, KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_INIT_FUNCTION_DECLARATION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE, x); \
  GENERATE_AVL_TREE_INIT_FUNCTION_DECLARATIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE);


/* --------------------- Definitions --------------------- */



#define GENERATE_AVL_TREE_NODE_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
int getHeight ## TREE_NAME ## Node(TREE_NAME ## Node* node) { \
  if (node == NULL) return 0; \
   \
  return node->height_; \
} \
 \
void resetHeight ## TREE_NAME ## Node(TREE_NAME ## Node* node) { \
  if (node == NULL) return; \
   \
  int leftHeight = getHeight ## TREE_NAME ## Node(node->left_); \
  int rightHeight = getHeight ## TREE_NAME ## Node(node->right_); \
  node->height_ = ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1; \
} \
 \
int isBalanced ## TREE_NAME ## Node(TREE_NAME ## Node* node) { \
  int balance = fabs(getHeight ## TREE_NAME ## Node(node->left_) - \
                       getHeight ## TREE_NAME ## Node(node->right_)); \
  return (balance <= 1); \
} \
 \
void destroyAll ## TREE_NAME ## Node(TREE_NAME ## Node* node, \
  void (*destroyKey_)(KEY_TYPE* key), \
  void (*destroyValue_)(VALUE_TYPE* value)) { \
  \
  if (node != NULL) {\
    if (destroyKey_ != NULL) destroyKey_(&(node->key)); \
    if (destroyValue_ != NULL) destroyValue_(&(node->value)); \
    if (node->left_ != NULL) destroyAll ## TREE_NAME ## Node( \
        node->left_, destroyKey_, destroyValue_); \
    if (node->right_ != NULL) destroyAll ## TREE_NAME ## Node( \
        node->right_, destroyKey_, destroyValue_); \
  } \
}

#define GENERATE_AVL_TREE_NODE_ALLOC_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
TREE_NAME ## Node* alloc ## TREE_NAME ## Node() { \
  TREE_NAME ## Node* obj = malloc(sizeof(TREE_NAME ## Node)); \
  return obj; \
} \

#define GENERATE_AVL_TREE_NODE_ALLOC_FULL_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
TREE_NAME ## Node* alloc ## TREE_NAME ## NodeFull( \
    TREE_NAME ## Node* parent, \
    TREE_NAME ## Node* left, \
    TREE_NAME ## Node* right, \
    KEY_TYPE key, \
    VALUE_TYPE value) { \
  TREE_NAME ## Node* obj = alloc ## TREE_NAME ## Node(); \
  obj->parent_ = parent; \
  obj->left_ = left; \
  obj->right_ = right; \
  obj->height_ = 0; \
  obj->key = key; \
  obj->value = value; \
  return obj; \
}

#define GENERATE_AVL_TREE_NODE_FREE_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
void free ## TREE_NAME ## Node(TREE_NAME ## Node* obj) { \
  free(obj); \
}

#define GENERATE_AVL_TREE_NODE_INIT_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
  GENERATE_AVL_TREE_NODE_ALLOC_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_NODE_ALLOC_FULL_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_NODE_FREE_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE);







#define GENERATE_AVL_TREE_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
void TREE_NAME ## insert(TREE_NAME* obj, KEY_TYPE key, VALUE_TYPE value) { \
  TREE_NAME ## Node* newNode = NULL; \
  if (obj->root_ == NULL) { \
    newNode = alloc ## TREE_NAME ## NodeFull(NULL, NULL, NULL, key, value); \
    obj->root_ = newNode; \
  } \
  else { \
     \
    TREE_NAME ## Node* searcher = obj->root_; \
    while (newNode == NULL) { \
       \
      /* New key is less than check key */ \
      if (obj->compare_(key, searcher->key)) { \
       \
        /* Insert position found */ \
        if (searcher->left_ == NULL) { \
          newNode = alloc ## TREE_NAME ## NodeFull( \
              searcher, NULL, NULL, key, value); \
          searcher->left_ = newNode; \
          break; \
        } \
        else { \
          searcher = searcher->left_; \
        } \
         \
      } \
      /* New key is greater than check key */ \
      else { \
       \
        /* Insert position found */ \
        if (searcher->right_ == NULL) { \
          newNode = alloc ## TREE_NAME ## NodeFull( \
              searcher, NULL, NULL, key, value); \
          searcher->right_ = newNode; \
          break; \
        } \
        else { \
          searcher = searcher->right_; \
        } \
         \
      } \
       \
    } \
     \
  } \
   \
  resetHeight ## TREE_NAME ## Node(newNode); \
  obj->rebalance_(obj, newNode); \
  ++(obj->size_); \
} \
 \
void TREE_NAME ## remove(TREE_NAME* obj, KEY_TYPE key) { \
  TREE_NAME ## Node* node = obj->find(obj, key); \
   \
  /* Not in tree */ \
  if (node == NULL) return; \
   \
  TREE_NAME ## Node* parent = node->parent_; \
  if (obj->destroyKey_ != NULL) obj->destroyKey_(&(node->key)); \
  if (obj->destroyValue_ != NULL) obj->destroyValue_(&(node->value)); \
  free ## TREE_NAME ## Node(node); \
  --(obj->size_); \
   \
  obj->rebalance_(obj, parent); \
} \
 \
TREE_NAME ## Node* TREE_NAME ## find( \
    TREE_NAME* obj, KEY_TYPE key) { \
   \
  /* Check if tree is empty */ \
  if (obj->root_ == NULL) return NULL; \
   \
  TREE_NAME ## Node* searcher = obj->root_; \
  while (!(obj->equals_(key, searcher->key))) { \
     \
    /* Search key is less than check key */ \
    if (obj->compare_(key, searcher->key)) { \
     \
      if (searcher->left_ == NULL) { \
        return NULL; \
      } \
      else { \
        searcher = searcher->left_; \
      } \
       \
    } \
    /* Search key is greater than check key */ \
    else { \
     \
      /* Insert position found */ \
      if (searcher->right_ == NULL) { \
        return NULL; \
      } \
      else { \
        searcher = searcher->right_; \
      } \
       \
    } \
     \
  } \
   \
  return searcher; \
} \
   \
void TREE_NAME ## rebalance(TREE_NAME* obj, \
                                TREE_NAME ## Node* node) { \
  while (1) { \
    resetHeight ## TREE_NAME ## Node(node); \
    if (!isBalanced ## TREE_NAME ## Node(node)) { \
       \
      TREE_NAME ## Node* leftChild = node->left_; \
      TREE_NAME ## Node* rightChild = node->right_; \
       \
      if (getHeight ## TREE_NAME ## Node(leftChild) \
            >= getHeight ## TREE_NAME ## Node(rightChild)) { \
        if (getHeight ## TREE_NAME ## Node(leftChild->left_) \
              >= getHeight ## TREE_NAME ## Node(leftChild->right_)) { \
          /* b */ \
          node = obj->rotateRight_(obj, leftChild, node); \
        } \
        else { \
          /* d */ \
          node = obj->rotateLeft_(obj, leftChild->right_, leftChild); \
          node = obj->rotateRight_(obj, node, node->parent_); \
        } \
      } \
      else { \
        if (getHeight ## TREE_NAME ## Node(rightChild->left_) \
            >= getHeight ## TREE_NAME ## Node(rightChild->right_)) { \
          /* c */ \
          node = obj->rotateRight_(obj, rightChild->left_, rightChild); \
          node = obj->rotateLeft_(obj, node, node->parent_); \
        } \
        else { \
          /* a */ \
          node = obj->rotateLeft_(obj, rightChild, node); \
        } \
      } \
       \
      resetHeight ## TREE_NAME ## Node(node->left_); \
      resetHeight ## TREE_NAME ## Node(node->right_); \
      resetHeight ## TREE_NAME ## Node(node); \
    } \
     \
    if (node->parent_ == NULL) { break; } \
     \
    node = node->parent_; \
  } \
   \
  /* Update root */ \
  obj->root_ = node; \
} \
  \
void TREE_NAME ## destroy(TREE_NAME* obj) { \
  if (obj->root_ != NULL) destroyAll ## TREE_NAME ## Node(obj->root_, \
                            obj->destroyKey_, obj->destroyValue_); \
} \
   \
TREE_NAME ## Node* TREE_NAME ## rotateLeft( \
                     TREE_NAME* obj, \
                     TREE_NAME ## Node* low, \
                     TREE_NAME ## Node* high) { \
  TREE_NAME ## Node* lowL = (low != NULL) ? low->left_ : NULL; \
  TREE_NAME ## Node* highP = (high != NULL) ? high->parent_ : NULL; \
   \
  /* Swap parent/child relationship of high and low */ \
  low->left_ = high; \
  high->parent_ = low; \
   \
  /* Make low's left child high's right */ \
  high->right_ = lowL; \
  if (lowL != NULL) { \
    lowL->parent_ = high; \
  } \
   \
  /* Make low the child of high's parent */ \
  low->parent_ = highP; \
  if (highP != NULL) { \
    if (highP->right_ == high) { \
      highP->right_ = low; \
    } \
    else { \
      highP->left_ = low; \
    } \
  } \
   \
  return low; \
} \
   \
TREE_NAME ## Node* TREE_NAME ## rotateRight( \
                     TREE_NAME* obj, \
                     TREE_NAME ## Node* low, \
                     TREE_NAME ## Node* high) { \
  TREE_NAME ## Node* lowR = (low != NULL) ? low->right_ : NULL;  \
  TREE_NAME ## Node* highP = (high != NULL) ? high->parent_ : NULL; \
   \
  /* Swap parent/child relationship of high and low */ \
  low->right_ = high; \
  high->parent_ = low; \
   \
  /* Make low's right child high's left */ \
  high->left_ = lowR; \
  if (lowR != NULL) { \
    lowR->parent_ = high; \
  } \
   \
  /* Make low the child of high's parent */ \
  low->parent_ = highP; \
  if (highP != NULL) { \
    if (highP->right_ == high) { \
      highP->right_ = low; \
    } \
    else { \
      highP->left_ = low; \
    } \
  } \
   \
  return low; \
}

#define GENERATE_AVL_TREE_INIT_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE, PREFIX) \
void PREFIX ## init ## TREE_NAME(TREE_NAME* obj, \
                           int (*compare__)(KEY_TYPE key1, KEY_TYPE key2), \
                           int (*equals__)(KEY_TYPE key1, KEY_TYPE key2)) { \
  obj->insert = TREE_NAME ## insert; \
  obj->remove = TREE_NAME ## remove; \
  obj->find = TREE_NAME ## find; \
  obj->rebalance_ = TREE_NAME ## rebalance; \
  obj->rotateLeft_ = TREE_NAME ## rotateLeft; \
  obj->rotateRight_ = TREE_NAME ## rotateRight; \
  obj->destroy = TREE_NAME ## destroy; \
  obj->destroyKey_ = NULL; \
  obj->destroyValue_ = NULL; \
  obj->compare_ = compare__; \
  obj->equals_ = equals__; \
  obj->root_ = NULL; \
  obj->size_ = 0; \
}

#define GENERATE_AVL_TREE_ALLOC_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
TREE_NAME* alloc ## TREE_NAME( \
                           int (*compare__)(KEY_TYPE key1, KEY_TYPE key2), \
                           int (*equals__)(KEY_TYPE key1, KEY_TYPE key2)) { \
  TREE_NAME* obj = malloc(sizeof(TREE_NAME)); \
  init ## TREE_NAME(obj, compare__, equals__); \
  return obj; \
}

#define GENERATE_AVL_TREE_FREE_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
void free ## TREE_NAME(TREE_NAME* obj) { \
  obj->destroy(obj);\
  \
  free(obj); \
}

#define GENERATE_AVL_TREE_INIT_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
  GENERATE_AVL_TREE_INIT_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE,); \
  GENERATE_AVL_TREE_ALLOC_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_FREE_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE);

#define GENERATE_AVL_TREE_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE) \
  GENERATE_AVL_TREE_NODE_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_NODE_INIT_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_INIT_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE);

/* Alternate form that allows destructors to be defined for key
   and/or value types with dynamically allocated memory that must
   be freed when removed from the tree.
   If one of these isn't needed, substitute NULL instead. */
#define GENERATE_AVL_TREE_DEFINITIONS_WITH_DESTRUCTORS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE, KEY_DESTRUCTOR, VALUE_DESTRUCTOR) \
  GENERATE_AVL_TREE_NODE_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_NODE_INIT_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_FUNCTION_DEFINITIONS(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_INIT_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE, x); \
  void init ## TREE_NAME(TREE_NAME* obj, \
                             int (*compare__)(KEY_TYPE key1, KEY_TYPE key2), \
                             int (*equals__)(KEY_TYPE key1, KEY_TYPE key2)) { \
    xinit ## TREE_NAME(obj, compare__, equals__); \
    obj->destroyKey_ = KEY_DESTRUCTOR; \
    obj->destroyValue_ = VALUE_DESTRUCTOR; \
  } \
  GENERATE_AVL_TREE_ALLOC_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE); \
  GENERATE_AVL_TREE_FREE_FUNCTION_DEFINITION(TREE_NAME, \
          KEY_TYPE, VALUE_TYPE);


#endif
