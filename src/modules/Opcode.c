#include "modules/Opcode.h"
#include "modules/DismStruct.h"
#include "modules/Consts.h"
#include <stdlib.h>

OpcodeSimilarity greaterOpSimilarity(
    OpcodeSimilarity first, OpcodeSimilarity second) {
  return ((int)first < (int)second) ? first : second;
}

OpcodeSimilarity greaterOpDifference(
    OpcodeSimilarity first, OpcodeSimilarity second) {
  return ((int)first > (int)second) ? first : second;
}

int OpcodeisFunctionallyCongruent(struct Opcode* obj, struct Opcode* other) {
  return 1;
}

unsigned int Opcoderead(Opcode* obj, BufferStream* stream,
                        DismSettings* config, MapSS* args,
                        int streamBasePos) {
  obj->pos_ = stream->pos(stream);
  obj->loadAddr_ = stream->pos(stream) - streamBasePos + config->fileLoadAddr;
  return obj->readStep(obj, stream, config, args);
}
                        
void Opcodeprint(Opcode* obj, String* dst,
                 BufferStream* stream,
                 DismSettings* config,
                 int srcAddrW,
                 int showRaw,
                 int middleWidth) {
  String temp, fmt;
  initString(&temp);
  initString(&fmt);
  
  /* Address */
  
  /* Convert srcAddrW to decimal string so we can use it in the format
     string */
  temp.fromInt(&temp, srcAddrW, "%d");
  fmt.catC(&fmt, "%0");
  fmt.catString(&fmt, &temp);
  fmt.catC(&fmt, "X");
  /* Print address with the specified padding */
  temp.fromInt(&temp, obj->loadAddr_, fmt.cStr(&fmt));
  dst->catC(dst, temp.cStr(&temp));
  dst->catC(dst, " ");
  
  /* Raw bytes */
  
  temp.clear(&temp);
  int middleRemaining = middleWidth;
  
  if (showRaw) {
    int bytelen = strlen(obj->info_->recString) / k_bitsPerByte;
    if (bytelen == 0) ++bytelen;
    
    int i;
    for (i = 0; i < bytelen; i++) {
      unsigned char b
        = *((unsigned char*)(stream->get(stream, obj->pos_ + i)));
      temp.catInt(&temp, (unsigned int)b, " %02X");
    }
    
    dst->catC(dst, temp.cStr(&temp));
    middleRemaining -= (3 * bytelen);
    temp.clear(&temp);
  }
  
  /* Middle padding */
  
  int i;
  for (i = 0; i < middleRemaining; i++) {
    dst->catC(dst, " ");
  }
  
  /* Name */
  
  obj->printName(obj, dst, config);
  
  /* Disassembly */
  
  obj->printString(obj, dst, config);
  
  temp.destroy(&temp);
  fmt.destroy(&fmt);
}

void OpcodeprintName(Opcode* obj, String* dst,
                  DismSettings* config) {
  dst->catC(dst, obj->info_->name);
  /* can't put space here in general -- many architectures have additional
     specifiers, e.g. move.b or move.w */
/*  dst->catC(dst, " "); */
}

void OpcodeprintNameWithSpace(Opcode* obj, String* dst,
                  DismSettings* config) {
  dst->catC(dst, obj->info_->name);
  dst->catC(dst, " ");
}
  
void* Opcodedata(Opcode* obj) {
  return obj->data_;
}
  
void OpcodesetData(Opcode* obj, void* data__) {
  obj->data_ = data__;
}

unsigned int Opcodepos(Opcode* obj) {
  return obj->pos_;
}

unsigned int OpcodeloadAddr(Opcode* obj) {
  return obj->loadAddr_;
}

OpInfo* Opcodeinfo(Opcode* obj) {
  return obj->info_;
}

void Opcodedestroy(Opcode* obj) {
  
}

void initOpcode(Opcode* obj) {
  obj->compare = NULL;
  obj->isFunctionallyCongruent = OpcodeisFunctionallyCongruent;
  obj->read = Opcoderead;
  obj->data = Opcodedata;
  obj->setData = OpcodesetData;
  obj->pos = Opcodepos;
  obj->loadAddr = OpcodeloadAddr;
  obj->info = Opcodeinfo;
  obj->readStep = NULL;
  obj->print = Opcodeprint;
  obj->printName = OpcodeprintName;
  obj->printString = NULL;
  obj->destroy = Opcodedestroy;
  obj->info_ = NULL;
  obj->data_ = NULL;
}

Opcode* allocOpcode() {
  Opcode* obj = malloc(sizeof(Opcode));
  initOpcode(obj);
  return obj;
}

void freeOpcode(Opcode* obj) {
  obj->destroy(obj);
  
  free(obj->data_);
  free(obj);
}

