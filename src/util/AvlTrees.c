#include "util/AvlTrees.h"

/*GENERATE_AVL_TREE_DEFINITIONS(
    AvlTreeStringChar, String, char); */
    
void destroyString(String* str) {
  str->destroy(str);
}

/*GENERATE_AVL_TREE_DEFINITIONS_WITH_DESTRUCTORS(
    AvlTreeStringChar, String, char, destroyString, NULL); */

GENERATE_AVL_TREE_DEFINITIONS_WITH_DESTRUCTORS(
    AvlTreeStringString, String, String, destroyString, destroyString);

