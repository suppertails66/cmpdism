#include "util/Maps.h"
#include "util/Logger.h"
#include "util/String.h"
#include "modules/StringMatcher.h"
#include <stdlib.h>

int stringCompare(String first, String second) {
/*          printf("%s %s %d\n", first.cStr(&first), second.cStr(&second), (strcmp(first.cStr(&first), second.cStr(&second)) < 0)); */
  return (strcmp(first.cStr(&first), second.cStr(&second)) < 0);
}

int stringEquals(String first, String second) {
  return (strcmp(first.cStr(&first), second.cStr(&second)) == 0);
}

void initMapSS(MapSS* obj) {
  initAvlTreeStringString(obj, stringCompare, stringEquals);
}

MapSS* allocMapSS() {
  MapSS* obj = malloc(sizeof(MapSS));
  initMapSS(obj);
  return obj;
}

void freeMapSS(MapSS* obj) {
  obj->destroy(obj);

  free(obj);
}

MapSSNode* getMapSSNode(MapSS* storage, const char* key) {
  String nStr;
  initString(&nStr);
  nStr.catC(&nStr, key);
  MapSSNode* findNode = storage->find(storage, nStr);
  nStr.destroy(&nStr);
  return findNode;
}

String getMapSSValue(MapSS* storage, const char* key) {
  MapSSNode* findNode = getMapSSNode(storage, key);
  
  if (findNode == NULL) {
    error("Bad getMapSSValue() key: ");
    error(key);
    fatal();
  }
  
  String value = findNode->value;
  return value;
}

const char* getMapSSValueCStr(MapSS* storage, const char* key) {
  String value = getMapSSValue(storage, key);
  return value.cStr(&value);
}

int getMapSSValueAsBinaryInt(MapSS* storage, const char* key) {
  String EValue = getMapSSValue(storage, key);
  return binaryStringToInt(EValue.cStr(&EValue), EValue.size(&EValue));
}

unsigned int getMapSSValueAsBinaryUint(MapSS* storage, const char* key) {
  String EValue = getMapSSValue(storage, key);
  return binaryStringToUint(EValue.cStr(&EValue), EValue.size(&EValue));
}

