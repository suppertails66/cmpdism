#ifndef CPD_MODULE_68000_H
#define CPD_MODULE_68000_H


#include "modules/DismModule.h"
#include "modules/Opcode.h"

/**
 * Disassembly module for Motorola 68000.
 */

/**
 * Enum of 68000 addressing modes.
 */
typedef enum AddressMode68000 {
  
  addressMode68000Dn,
  addressMode68000An,
  addressMode68000AnInd,
  addressMode68000AnIndInc,
  addressMode68000AnIndDec,
  addressMode68000AnOff16,
  addressMode68000AnOff8,
  addressMode68000Addr16,
  addressMode68000Addr32,
  addressMode68000PC16,
  addressMode68000PC8,
  addressMode68000Immed,
  addressMode68000Invalid
  
} AddressMode68000;

/**
 * Enum of 68000 op targets (sources and destinations).
 */
typedef enum OpTarget68000 {

  opTarget68000None = 0,
  opTarget68000ConstB,
  opTarget68000ConstW,
  opTarget68000ConstL,
  opTarget68000ConstBit,
  opTarget68000ConstData3,
  opTarget68000ConstData8,
  opTarget68000ConstVector,
  opTarget68000ConstCCC,
  opTarget68000CCR,
  opTarget68000Data16AS,
  opTarget68000Data16AD,
  opTarget68000DS,
  opTarget68000DD,
  opTarget68000Address,
  opTarget68000AddressE,
  opTarget68000AddressF,
  opTarget68000AS,
  opTarget68000AD,
  opTarget68000RegList,
  opTarget68000ASDec,
  opTarget68000ASInc,
  opTarget68000ADDec,
  opTarget68000ADInc,
  opTarget68000Label

} OpTarget68000;

/**
 * Container for information about an addressing mode.
 */
typedef struct AddressMode68000Info {
  
  AddressMode68000 mode;
  int size;
  char* argStr;
  
} AddressMode68000Info;
 
/**
 * Opcode sub-struct for holding subops.
 */
typedef struct {

  /* The overall arguments for the op */
  MapSS* args;
  
  OpTarget68000 src;
  OpTarget68000 dst;
  
  /* Ops for address a (if present) */
  MapSS* addressA;
  AddressMode68000Info* addrAInfo;
  int addrASize;
  
  /* Ops for address e (if present) */
  MapSS* addressE;
  AddressMode68000Info* addrEInfo;
  int addrESize;
  
  /* Ops for address f (if present) */
  MapSS* addressF;
  AddressMode68000Info* addrFInfo;
  int addrFSize;

} SubOps68000;

/**
 * Struct for returning length + success pairs when reading Opcodes.
 */
typedef struct {
  
  int length;
  int success;
  
} OpRead68000Result;

/* extern const int numOpTargets68000; */

/* Op list */

extern OpInfo opcodes68000[];

/* Init functions */

void initModule68000(DismModule* obj);
DismModule* allocModule68000();
void freeModule68000(DismModule* obj);

SubOps68000* allocSubOps68000();
void freeSubOps68000(SubOps68000* obj);
void destructorSubOps68000Opcode(Opcode* obj);


#endif
