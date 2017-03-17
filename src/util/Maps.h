#ifndef CPD_MAPS_H
#define CPD_MAPS_H


#include "util/AvlTrees.h"

/* lazy */
typedef AvlTreeStringString MapSS;
typedef AvlTreeStringStringNode MapSSNode;

void initMapSS(MapSS* obj);
MapSS* allocMapSS();
void freeMapSS(MapSS* obj);

MapSSNode* getMapSSNode(MapSS* storage, const char* key);

/* because I got sick of doing explicit inits for search Strings
   key MUST be in storage! */
String getMapSSValue(MapSS* storage, const char* key);
const char* getMapSSValueCStr(MapSS* storage, const char* key);

int getMapSSValueAsBinaryInt(MapSS* storage, const char* key);
unsigned int getMapSSValueAsBinaryUint(MapSS* storage, const char* key);


#endif
