#ifndef CPD_AVL_TREES_H
#define CPD_AVL_TREES_H


#include "util/AvlTree.h"
#include "util/String.h"

/*GENERATE_AVL_TREE_DECLARATIONS_WITH_DESTRUCTORS(
    AvlTreeStringChar, String, char); */

GENERATE_AVL_TREE_DECLARATIONS_WITH_DESTRUCTORS(
    AvlTreeStringString, String, String);


#endif
