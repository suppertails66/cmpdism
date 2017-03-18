#include "modules/DismModule.h"
#include "modules/StringMatcher.h"
#include "modules/DataOpInfo.h"
#include "modules/OpArgCollator.h"
#include "util/Logger.h"
#include <stdlib.h>

void DismModulerun(DismModule* obj, DismSettings config) {
  /* Nothing to do if no input files */
  if (config.firstFile == NULL) return;

  DismStruct* firstDismStruct = NULL;
  DismStruct* secondDismStruct = NULL;
  
  /* Assign IDs to the opcodes in use */
  assignOpcodeIDs(&(obj->opInfoArrays));

  /* Open the first file (which must be specified in order for this
     function to get called in the first place */
  firstDismStruct = allocDismStruct();
  firstDismStruct->settings = config;
  firstDismStruct->stream->load(firstDismStruct->stream,
                                config.firstFile);
  
  /* One-file mode */
  if (config.secondFile == NULL) {
    /* Disassemble the first file */
    obj->disassemble(obj, firstDismStruct,
                     config.firstFileStartOffset, config.firstFileEndOffset);
    
    /* Print disassembly */
    String disassemblyString;
    initString(&disassemblyString);
    obj->printDisassembly(obj, &disassemblyString, firstDismStruct);
    printf("%s", disassemblyString.cStr(&disassemblyString));
    disassemblyString.destroy(&disassemblyString);
  }
  /* Two-file mode */
  else {
    /* Open the second file */
    secondDismStruct = allocDismStruct();
    secondDismStruct->settings = config;
    secondDismStruct->stream->load(secondDismStruct->stream,
                                  config.secondFile);
    
    /* Generate code maps if needed */
    if (config.intelligentCodeDetection) {
      obj->generateCodeMap(obj, firstDismStruct, config);
      obj->generateCodeMap(obj, secondDismStruct, config);
    }
    
/*    FILE* first = fopen("codemap1.bin", "wb");
    fwrite((char*)(firstDismStruct->codeMap->data(firstDismStruct->codeMap)),
           sizeof(char),
           firstDismStruct->codeMap->size(firstDismStruct->codeMap),
           first); */
                              
    /* Disassemble the first file */
    obj->disassemble(obj, firstDismStruct,
                     config.firstFileStartOffset, config.firstFileEndOffset);
                              
    /* Disassemble the second file */
    obj->disassemble(obj, secondDismStruct,
                     config.secondFileStartOffset, config.secondFileEndOffset);
    
    /* Print comparative disassembly */
    String disassemblyString;
    initString(&disassemblyString);
    obj->printComparedDisassembly(obj, &disassemblyString,
                                  firstDismStruct, secondDismStruct);
    printf("%s", disassemblyString.cStr(&disassemblyString));
    disassemblyString.destroy(&disassemblyString);
  }
  
  /* Free disassembly structures */
  if (firstDismStruct != NULL) freeDismStruct(firstDismStruct);
  if (secondDismStruct != NULL) freeDismStruct(secondDismStruct);
}

void DismModulecheckAdditionalParams(DismModule* obj, DismSettings config) {
  
}


void DismModuleparse(DismModule* obj, DismStruct* dismStruct) {
  
}

void DismModulegenerateCodeMap(DismModule* obj, DismStruct* dismStruct,
                        DismSettings config) {
    
  unsigned int start = 0;
  unsigned int end = -1;
  BufferStream* stream = dismStruct->stream;
  VectorOpcode* opcodes = &(dismStruct->opcodes);
  
  CodeMap* codeMap = allocCodeMap();
  codeMap->resize(codeMap, stream->size(stream));
  /* Assume everything is code to begin with.
     This is extremely important -- otherwise, readNextOp() will reject
     everything (because it's marked as data) */
  codeMap->fill(codeMap, 0xFF);
  
  dismStruct->codeMap = codeMap;
/*  DismSettings* config = &(dismStruct->settings); */
  
  /* Examine the specified portion of the file */
  unsigned int limit = (end != -1) ? end : stream->size(stream);
  stream->seek(stream, start);
  /* Generate opcodes until we reach the end of the specified section */
  while (stream->tell(stream) < limit) {
    obj->readNextOp(obj, dismStruct, (limit - stream->tell(stream)));
    
    /* Did we generate a data opcode? */
    Opcode* lastOp = opcodes->getP(opcodes, opcodes->size(opcodes) - 1);
    if (lastOp->info(lastOp)->id == DATA_OP_ID) {
      /* Go backwards through the opcodes until we reach the start or
         find something that isn't flagged as suspicious; we will consider
         this the last valid instruction.
         This also covers any issues of byte alignment. */
      int lastGoodOp;
      Opcode* lastGoodOpP = NULL;
      for (lastGoodOp = opcodes->size(opcodes) - 2;
           lastGoodOp >= 0;
           lastGoodOp--) {
        lastGoodOpP = opcodes->getP(opcodes, lastGoodOp);
        OpInfo* opInfo = lastGoodOpP->info(lastGoodOpP);
        if ((opInfo->id !=  DATA_OP_ID)
            && !(opInfo->flags & opFlagsSuspicious)) break;
      }
      
      /* Mark the discovered block as data */
      int blockEnd = lastOp->pos(lastOp) + 1;
      int blockStart = (lastGoodOpP == NULL) ? 0
          : lastGoodOpP->pos(lastGoodOpP);
      memset(codeMap->data(codeMap) + blockStart, 0x00, blockEnd - blockStart);
      
      /* We're now in a data block.
         Everything we see should be treated as data until we encounter
         the specified number of consecutive valid opcodes. */
      
      /* Match the needed number of opcodes */
      while ((stream->tell(stream) < limit)
             && !(obj->matchSequentialOps(obj, config.requiredCodeMapResumeOps,
                                       dismStruct))) {
        
      }
      
      /* We're now back at a code section (or done, if we reached the end
         of the stream */
    }
    /* Op was valid: mark as code */
    else {
      int blockEnd = stream->pos(stream);
      int blockStart = lastOp->pos(lastOp);
      memset(codeMap->data(codeMap) + blockStart, 0xFF, blockEnd - blockStart);
    }
  }
  
  /* Throw away our invalid opcode stream and reset everything */
  opcodes->clear(opcodes);
  stream->seek(stream, 0);
}

void DismModuledisassemble(DismModule* obj, DismStruct* dismStruct,
                           unsigned int start, unsigned int end) {
  BufferStream* stream = dismStruct->stream;
/*  DismSettings* config = &(dismStruct->settings);
  VectorOpcode* opcodes = &(dismStruct->opcodes);
  CodeMap* codeMap = dismStruct->codeMap; */
  
  /* Examine the specified portion of the file */
  unsigned int limit = (end != -1) ? end : stream->size(stream);
  stream->seek(stream, start);
  /* Generate opcodes until we reach the end of the specified section */
  while (stream->tell(stream) < limit) {
    obj->readNextOp(obj, dismStruct, (limit - stream->tell(stream)));
  }
  
}
                           
void DismModuleprintComparedDisassembly(DismModule* obj, String* dst,
                                DismStruct* firstDismStruct,
                                DismStruct* secondDismStruct) {
  /* 
    - read ops sequentially
    - if op IDs match, print disassemblies side-by-side and continue
    - if op IDs don't match, figure out how to realign stream
      - try to find if the pending sequence in A is located somewhere
        further ahead in B (i.e. content was added in B)
        - print B only until we reach the resumption point
      - try the above with A and B interchanged (i.e. content was removed
        in B)
        - print A only until we reach the resumption point
      - if both of those fail, assume we are at the start of a transformed
        block
        - advance A and B by one token and repeat the search procedure
          outlined above until it succeeds. At that point, all the
          preceding content from the initial detection to the success
          point is the transformed block, possibly followed by an addition
          or removal section
  */
  
  VectorOpcode* opcodesA = &(firstDismStruct->opcodes);
  VectorOpcode* opcodesB = &(secondDismStruct->opcodes);
  unsigned int opcodesASize = (opcodesA->size(opcodesA));
  unsigned int opcodesBSize = (opcodesB->size(opcodesB));
  
  String tempString;
  initString(&tempString);
  
  /* Stop at end of shorter stream */
  unsigned int limit = (opcodesASize < opcodesBSize)
                         ? opcodesASize
                         : opcodesBSize;
  unsigned int iA = 0;
  unsigned int iB = 0;
  
  while ((iA < limit) && (iB < limit)) {
    
    Opcode* opcodeA = opcodesA->getP(opcodesA, iA);
    Opcode* opcodeB = opcodesB->getP(opcodesB, iB);
    OpInfo* opInfoA = opcodeA->info(opcodeA);
    OpInfo* opInfoB = opcodeB->info(opcodeB);
    
    /* Are the current Opcodes of the same type? */
    if (opInfoA->id == opInfoB->id) {
      /* Yes: print both, advance both streams, and continue */
      
      tempString.clear(&tempString);
      tempString.catC(&tempString, "  ");
      opcodeA->print(opcodeA, &tempString, firstDismStruct->stream,
                    &(firstDismStruct->settings),
                    firstDismStruct->settings.dualSrcAddrW,
                    firstDismStruct->settings.dualShowRaw,
                    firstDismStruct->settings.dualMiddleWidth);
      tempString.padToSize(&tempString, ' ',
                            firstDismStruct->settings.dualSeparationW);
      opcodeB->print(opcodeB, &tempString, secondDismStruct->stream,
                    &(firstDismStruct->settings),
                    firstDismStruct->settings.dualSrcAddrW,
                    firstDismStruct->settings.dualShowRaw,
                    firstDismStruct->settings.dualMiddleWidth);
      
      /* Add indicator if a significant change has occurred */
      if ((opInfoA->id != DATA_OP_ID)) {
/*        fprintf(stderr, "here1\n"); */
        OpcodeSimilarity similarity
          = opcodeA->compare(opcodeA, opcodeB, &(firstDismStruct->settings));
        if (similarity == opcodeSimilarityDistinct) {
          tempString.catC(&tempString, " ; !!!");
        }
      }
      else {
        OpcodeSimilarity similarity
          = opcodeA->compare(opcodeA, opcodeB, &(firstDismStruct->settings));
        if (similarity == opcodeSimilarityDistinct) {
          tempString.catC(&tempString, " ; ???");
        }
      }
      
/*      dst->catString(dst, &tempString);
      dst->catC(dst, "\n"); */
      printf(tempString.cStr(&tempString));
      printf("\n");
      
      ++iA;
      ++iB;
      continue;
    }
    
/*    printf("%04X %04X\n", iA, iB); */
/*    printf("%04X %04X\n", opcodeA->pos(opcodeA), opcodeB->pos(opcodeB)); */
    
    /* Opcodes are of different types: stream realignment needed */
    OpcodeAlignmentData alignment = obj->detectNewAlignment(obj,
                      firstDismStruct,
                      secondDismStruct,
                      iA, iB,
                      limit,
                      firstDismStruct->settings.requiredSequentialOps,
                      firstDismStruct->settings.maxChangeBlockSize);
    
    /* Check if we hit the limit */
    if ((alignment.alignmentType == opcodeAlignmentTransformed)
        && ((alignment.iA >= limit) && (alignment.iB >= limit))) {
      
      /* Print remaining shared content */
      while ((iA < opcodesASize) && (iB < opcodesBSize)) {
    
        Opcode* opcodeA = opcodesA->getP(opcodesA, iA);
        Opcode* opcodeB = opcodesB->getP(opcodesB, iB);
        
        tempString.clear(&tempString);
        tempString.catC(&tempString, "Z ");
        opcodeA->print(opcodeA, &tempString, firstDismStruct->stream,
                      &(firstDismStruct->settings),
                      firstDismStruct->settings.dualSrcAddrW,
                      firstDismStruct->settings.dualShowRaw,
                      firstDismStruct->settings.dualMiddleWidth);
        tempString.padToSize(&tempString, ' ',
                              firstDismStruct->settings.dualSeparationW);
        opcodeB->print(opcodeB, &tempString, secondDismStruct->stream,
                      &(firstDismStruct->settings),
                      firstDismStruct->settings.dualSrcAddrW,
                      firstDismStruct->settings.dualShowRaw,
                      firstDismStruct->settings.dualMiddleWidth);
        
/*        dst->catString(dst, &tempString);
        dst->catC(dst, "\n"); */
        
        printf(tempString.cStr(&tempString));
        printf("\n");
        
        ++iA;
        ++iB;
      }
      
      /* Done */
      break;
    }
                      
    /* Deal with transform block, if present */
    if (alignment.transformBlockLength != 0) {
      
      /* Print both */
      unsigned int i;
      for (i = 0; i < alignment.transformBlockLength; i++) {
        Opcode* opcodeANew = opcodesA->getP(opcodesA, iA + i);
        Opcode* opcodeBNew = opcodesB->getP(opcodesB, iB + i);
      
        tempString.clear(&tempString);
        tempString.catC(&tempString, "* ");
        opcodeANew->print(opcodeANew, &tempString, firstDismStruct->stream,
                      &(firstDismStruct->settings),
                      firstDismStruct->settings.dualSrcAddrW,
                      firstDismStruct->settings.dualShowRaw,
                      firstDismStruct->settings.dualMiddleWidth);
        tempString.padToSize(&tempString, ' ',
                              firstDismStruct->settings.dualSeparationW);
        opcodeBNew->print(opcodeBNew, &tempString, firstDismStruct->stream,
                      &(firstDismStruct->settings),
                      firstDismStruct->settings.dualSrcAddrW,
                      firstDismStruct->settings.dualShowRaw,
                      firstDismStruct->settings.dualMiddleWidth);
                      
/*        dst->catString(dst, &tempString);
        dst->catC(dst, "\n"); */
        
        printf(tempString.cStr(&tempString));
        printf("\n");
      }
      
      /* Skip over transform block */
      iA += alignment.transformBlockLength;
      iB += alignment.transformBlockLength;
      
/*      printf("y: %d %d\n", iA, iB); */
    
/*      iA = alignment.iA;
      iB = alignment.iB;
      printf("%d %d\n", alignment.iA, alignment.iB);
      continue; */
    }
    
    VectorOpcode* targetOpcodes
      = (alignment.alignmentType == opcodeAlignmentAdded)
          ? opcodesB : opcodesA;
    DismStruct* targetDismStruct
      = (alignment.alignmentType == opcodeAlignmentAdded)
          ? secondDismStruct : firstDismStruct;
    unsigned int iNew
      = (alignment.alignmentType == opcodeAlignmentAdded)
          ? iB : iA;
    unsigned int catchupLen
      = (alignment.alignmentType == opcodeAlignmentAdded)
          ? (alignment.iB - iB) : (alignment.iA - iA);
/*    printf("values: %d %d\n", iA, iB); */
    unsigned int i;
    for (i = 0; i < catchupLen; i++) {
      Opcode* opcodeNew = targetOpcodes->getP(targetOpcodes, iNew + i);
      
      tempString.clear(&tempString);
      /* Print B */
      if (alignment.alignmentType == opcodeAlignmentAdded) {
        tempString.catC(&tempString, "+ ");
        tempString.padToSize(&tempString, ' ',
                              firstDismStruct->settings.dualSeparationW);
      }
      /* Print A */
      else {
        tempString.catC(&tempString, "- ");
      }
    
      opcodeNew->print(opcodeNew, &tempString, targetDismStruct->stream,
                    &(firstDismStruct->settings),
                    firstDismStruct->settings.dualSrcAddrW,
                    firstDismStruct->settings.dualShowRaw,
                    firstDismStruct->settings.dualMiddleWidth);
                      
/*      dst->catString(dst, &tempString);
      dst->catC(dst, "\n"); */
      
      printf(tempString.cStr(&tempString));
      printf("\n");
    }
    
    /* Move to new alignment positions */
    iA = alignment.iA;
    iB = alignment.iB;
  }
  
  /* Print remaining in longer stream */
  
  VectorOpcode* targetOpcodes
    = (iB <= limit) ? opcodesB : opcodesA;
  DismStruct* targetDismStruct
    = (iB <= limit) ? secondDismStruct : firstDismStruct;
  unsigned int iNew
    = (iB <= limit) ? iB : iA;
  unsigned int catchupLen
    = (iB <= limit) ? (limit - iB) : (limit - iA);
  unsigned int i;
/*  fprintf(stderr, "%d %d %d %d %d\n", iA, iB, limit, iNew, catchupLen); */
  for (i = 0; i < catchupLen; i++) {
    Opcode* opcodeNew = targetOpcodes->getP(targetOpcodes, iNew + i);
    
    tempString.clear(&tempString);
    /* Print B */
    if (iB < limit) {
      tempString.catC(&tempString, "  ");
      tempString.padToSize(&tempString, ' ',
                            firstDismStruct->settings.dualSeparationW);
    }
    /* Print A */
    else {
      tempString.catC(&tempString, "  ");
    }
  
    opcodeNew->print(opcodeNew, &tempString, targetDismStruct->stream,
                  &(firstDismStruct->settings),
                  firstDismStruct->settings.dualSrcAddrW,
                  firstDismStruct->settings.dualShowRaw,
                  firstDismStruct->settings.dualMiddleWidth);
                    
/*    dst->catString(dst, &tempString);
    dst->catC(dst, "\n"); */
    
    printf(tempString.cStr(&tempString));
    printf("\n");
  }
  
  tempString.destroy(&tempString);
}

OpcodeAlignmentData DismModuledetectNewAlignment(DismModule* obj,
                                 DismStruct* dismStructA,
                                 DismStruct* dismStructB,
                                 unsigned int iA,
                                 unsigned int iB,
                                 unsigned int limit,
                                 unsigned int seqLen,
                                 unsigned int maxDist) {
  
  OpcodeAlignmentData result;
  memset(&result, 0, sizeof(OpcodeAlignmentData));
  
  /* Until we find a position in A with correspondence in B or vice versa,
     we are in a transformation block and advance through A and B one byte
     at a time, re-running the search algorithm from each new position. */
  int transformLen = -1;
  do {
    ++transformLen;
    result = obj->detectOpcodeAddOrRemove(obj, dismStructA, dismStructB,
                                        iA, iB,
                                        limit, seqLen, maxDist);
    ++iA;
    ++iB;
  } while ((iA < limit) && (iB < limit)
           && (result.alignmentType == opcodeAlignmentTransformed));
           
  /* If we hit the limit, indicate as much in results */
  if (result.alignmentType == opcodeAlignmentTransformed) {
    result.iA = limit;
    result.iB = limit;
  }
  
  /* Mark transform block length */
  result.transformBlockLength = transformLen;
  
  return result;
}

OpcodeAlignmentData DismModuledetectOpcodeAddOrRemove(struct DismModule* obj,
                                 DismStruct* dismStructA,
                                 DismStruct* dismStructB,
                                 unsigned int iA,
                                 unsigned int iB,
                                 unsigned int limit,
                                 unsigned int seqLen,
                                 unsigned int maxDist) {
  OpcodeAlignmentData result;
  OpcodeAlignmentData temp;
  memset(&result, 0, sizeof(OpcodeAlignmentData));
  memset(&temp, 0, sizeof(OpcodeAlignmentData));
  
  result.alignmentType = opcodeAlignmentTransformed;
  result.iA = iA;
  result.iB = iB;
  
  /* Material added in B? */
  temp = obj->detectOpcodeAddition(obj, dismStructA, dismStructB,
                                     iA, iB,
                                     limit, seqLen, maxDist);
  /* Yes: we're done */
  if (temp.alignmentType == opcodeAlignmentAdded) {
    result.alignmentType = opcodeAlignmentAdded;
    result.iA = temp.iA;
    result.iB = temp.iB;
    return result;
  }
  
  /* Material removed in B? */
  temp = obj->detectOpcodeAddition(obj, dismStructB, dismStructA,
                                     iB, iA,
                                     limit, seqLen, maxDist);
  /* Yes: we're done */
  if (temp.alignmentType == opcodeAlignmentAdded) {
    result.alignmentType = opcodeAlignmentRemoved;
    result.iA = temp.iB;
    result.iB = temp.iA;
    return result;
  }
  
  return result;
}

OpcodeAlignmentData DismModuledetectOpcodeAddition(struct DismModule* obj,
                                 DismStruct* dismStructA,
                                 DismStruct* dismStructB,
                                 unsigned int iA,
                                 unsigned int iB,
                                 unsigned int limit,
                                 unsigned int seqLen,
                                 unsigned int maxDist) {
  OpcodeAlignmentData result;
  result.alignmentType = opcodeAlignmentSame;
  
/*  VectorOpcode* opcodesB = &(dismStructB->opcodes);
  unsigned int opcodesBSize = (opcodesB->size(opcodesB)); */
  
  /* Actual amount of data that might be checked */
  unsigned int realDist = seqLen + maxDist;
  
  /* Determine the opcode index where we stop checking.
     If there is enough room in the opcode stream, we go to the maximum
     distance allowed by the settings. Otherwise, we truncate the check window
     to fit within the stream. */
  limit = ((iB + realDist) < limit)
            ? (iB + maxDist)
            : (limit - seqLen);
            
  /* Check each opcode for realignment */
  unsigned int i;
  for (i = iB; i < limit; i++) {
    if (obj->matchSeq(obj, dismStructA, dismStructB, iA, i, seqLen)) {
      result.iA = iA;
      result.iB = i;
      result.alignmentType = opcodeAlignmentAdded;
      result.transformBlockLength = 0;
      return result;
    }
  }
  
  /* Couldn't realign: failure  */
  result.alignmentType = opcodeAlignmentSame;
  return result;
}

int DismModulematchSeq(struct DismModule* obj,
                                 DismStruct* dismStructA,
                                 DismStruct* dismStructB,
                                 unsigned int iA,
                                 unsigned int iB,
                                 unsigned int len) {
  VectorOpcode* opcodesA = &(dismStructA->opcodes);
  VectorOpcode* opcodesB = &(dismStructB->opcodes);
  
  /* If there aren't enough opcodes remaining in either of the Opcode
     streams, no match */
  if ((len > (opcodesA->size(opcodesA) - iA))
      || (len > (opcodesB->size(opcodesB) - iB))) {
    return 0;
  }
  
  unsigned int i;
  for (i = 0; i < len; i++) {
    Opcode* opA = opcodesA->getP(opcodesA, iA + i);
    Opcode* opB = opcodesB->getP(opcodesB, iB + i);
    
    OpInfo* opInfoA = opA->info(opA);
    OpInfo* opInfoB = opB->info(opB);
    
    /* If ops are not same type, they're not the same */
    if (opInfoA->id != opInfoB->id) return 0;
    
    /* If both opcodes are data (.db), and their values are different,
       they're not the same */
    if ((opInfoA->id == DATA_OP_ID)
        && (opA->compare(opA, opB, &(dismStructA->settings))
              != opcodeSimilaritySame)) return 0;
    
    /* If ops have mismatches in subops, they're not the same */
/*    if (!(opA->isFunctionallyCongruent(opA, opB))) return 0;*/
  }
  
  return 1;
}
                           
void DismModuleprintDisassembly(DismModule* obj, String* dst,
                                DismStruct* dismStruct) {
  VectorOpcode* opcodes = &(dismStruct->opcodes);
  unsigned int limit = opcodes->size(opcodes);
  
  unsigned int i;
  String temp;
  initString(&temp);
  for (i = 0; i < limit; i++) {
    Opcode* opcode = dismStruct->opcodes.getP(&(dismStruct->opcodes), i);
    
    opcode->print(opcode, &temp, dismStruct->stream, &(dismStruct->settings),
                  dismStruct->settings.singleSrcAddrW,
                  dismStruct->settings.singleShowRaw,
                  dismStruct->settings.singleMiddleWidth);
    
    dst->catString(dst, &temp);
    dst->catC(dst, "\n");
    
    temp.clear(&temp);
  }
    
  temp.destroy(&temp);
}

void DismModulereadNextOp(DismModule* obj, DismStruct* dismStruct,
                          unsigned int remaining) {
  
  /* Check each set of supplied opcodes */
  int success = 0;
  /* If we're using a codemap and this byte is marked as data, don't
     try to read an opcode */
  if ((dismStruct->codeMap != NULL)
      && !(dismStruct->codeMap->get(dismStruct->codeMap,
                                  dismStruct->stream->pos(
                                      dismStruct->stream)))) {
    
  }
  else {
    unsigned int limit = obj->opInfoArrays.size(&(obj->opInfoArrays));
    unsigned int i;
    for (i = 0; i < limit; i++) {
      success = obj->tryOpRead(obj, dismStruct,
                               obj->opInfoArrays.get(&(obj->opInfoArrays), i),
                               remaining);
      if (success) break;
    }
  }
  
  /* No matching op found: next byte must be data */
  if (!success) {
    /* Enforce byte alignment by padding with multiple data ops if needed */
    int streamPos = dismStruct->stream->pos(dismStruct->stream);
    int realignment = obj->byteAlignment_ - (streamPos % obj->byteAlignment_);
    unsigned int i;
    for (i = 0; i < realignment; i++) {
      Opcode opcode;
      initOpcode(&opcode);
      dataOpInfo.generateOpcode(
          &dataOpInfo, &opcode, &(dismStruct->settings));
      opcode.info_ = &dataOpInfo;
      int offset = opcode.read(
          &opcode, dismStruct->stream, &(dismStruct->settings), NULL);
      dismStruct->stream->seekOff(dismStruct->stream, offset);
      dismStruct->opcodes.pushBack(&(dismStruct->opcodes), opcode);
    }
  }
  
/*  String temp;
  initString(&temp);
  Opcode* last = dismStruct->opcodes.getP(&(dismStruct->opcodes), 
                    dismStruct->opcodes.size(&(dismStruct->opcodes)) - 1);
  temp.catC(&temp, last->info(last)->name);
  temp.catC(&temp, " ");
  last->printString(last, &temp, &(dismStruct->settings));
  printf("%s\n", temp.cStr(&temp));
  temp.destroy(&temp); */
}
  
int DismModuletryOpRead(DismModule* obj, DismStruct* dismStruct,
                        OpInfoArray ops, unsigned int remaining) {
  /* Try to match each opcode */
  int i;
  for (i = 0; i < ops.size; i++) {
    if (obj->matchOp(obj, dismStruct, &(ops.array[i]), remaining)) {
    
      /* ----------------temporary---------------- */
/*      if (ops.array[i].generateOpcode == NULL) {
        printf("%s\n", ops.array[i].name);
        continue;
      } */
      
      MapSS* args = NULL;
      if (obj->enableOpArgCollation_) {
/*        initMapSS(args); */
        args = allocMapSS();
        collateOpArgs(dismStruct->stream, args, ops.array[i].recString);
      }
      
/*      String findstr;
      initString(&findstr);
      findstr.catC(&findstr, "x");
      String resultstr;
      initString(&resultstr);
      AvlTreeStringStringNode* resultN = args.find(&args, findstr);
      if (resultN != NULL) {
        printf("%s\n", resultN->value.cStr(&(resultN->value)));
      }
      findstr.destroy(&findstr);
      resultstr.destroy(&resultstr); */
    
      Opcode opcode;
      initOpcode(&opcode);
      opcode.info_ = &(ops.array[i]);
      ops.array[i].generateOpcode(
          &(ops.array[i]), &opcode, &(dismStruct->settings));
      
      /* Attempt to read the opcode (which could potentially fail if the
         op was misdetected under certain circumstances) */
      int result;
      
      result = opcode.read(
        &opcode, dismStruct->stream, &(dismStruct->settings),
          (obj->enableOpArgCollation_) ? args : NULL);
      
      if (result) {
        /* Add Opcode to disassembly stream */
        dismStruct->opcodes.pushBack(&(dismStruct->opcodes), opcode);
        dismStruct->stream->seekOff(dismStruct->stream, result);
      }
      else {
        /* Failure (misdetection): destroy pending Opcode */
        opcode.destroy(&opcode);
      }
      
      /* Destroy collated args if enabled AND not claimed by op
         (handled by call to destroy() above */
/*      if (obj->enableOpArgCollation_ && !result) {
        freeMapSS(args);
      } */
      
      /* If we successfully read an op, we're done */
      if (result) return result;
    }
  }
  
  /* No op matched */
  return 0;
}
  
int DismModulematchOp(DismModule* obj, DismStruct* dismStruct,
                      OpInfo* opInfo, unsigned int remaining) {
  return matchBinaryString(dismStruct->stream,
                                 opInfo->recString,
                                 remaining);
}
                 
int DismModulebyteAlignment(DismModule* obj) {
  return obj->byteAlignment_;
}
  
int DismModulematchSequentialOps(DismModule* obj, int numOps,
                          DismStruct* dismStruct) {
  BufferStream* stream = dismStruct->stream;
  VectorOpcode* opcodes = &(dismStruct->opcodes);
  CodeMap* codeMap = dismStruct->codeMap;
/*  DismSettings* config = &(dismStruct->settings); */
  
  unsigned int limit = stream->size(stream);
  int startingPos = stream->pos(stream);
  
  int count = 0;
  while (count < numOps) {
    /* Fail if we reach the end of the stream */
    if (stream->tell(stream) >= limit) return 0;
    
    obj->readNextOp(obj, dismStruct, (limit - stream->tell(stream)));
    
    Opcode* lastOp = opcodes->getP(opcodes, opcodes->size(opcodes) - 1);
    if ((lastOp->info(lastOp)->id == DATA_OP_ID)) {
      /* We were wrong: everything we just read was actually data */
      memset(codeMap->data(codeMap) + startingPos, 0x00,
             stream->pos(stream) - startingPos);
      return 0;
    }
    
    /* Mark read op as code */
    memset(codeMap->data(codeMap) + lastOp->pos(lastOp), 0xFF,
           stream->pos(stream) - lastOp->pos(lastOp));
    
    /* Do not count op toward total if it's flagged as suspicious */
    if (!(lastOp->info(lastOp)->flags & opFlagsSuspicious)) {
      ++count;
    }
  }
  
  /* Found enough ops: success */
  return 1;
}
                      
void DismModuledestroy(DismModule* obj) {
  obj->opInfoArrays.destroyAll(&obj->opInfoArrays);
  obj->destroyInternal(obj);
  if (obj->data_ != NULL) free(obj->data_);
}
                      
void DismModuledestroyInternal(DismModule* obj) {
  
}

void initDismModule(DismModule* obj) {
  initVectorOpInfoArray(&(obj->opInfoArrays));
  obj->run = DismModulerun;
  obj->checkAdditionalParams = DismModulecheckAdditionalParams;
  obj->parse = DismModuleparse;
  obj->disassemble = DismModuledisassemble;
  obj->printDisassembly = DismModuleprintDisassembly;
  obj->printComparedDisassembly = DismModuleprintComparedDisassembly;
  obj->generateCodeMap = DismModulegenerateCodeMap;
  obj->detectNewAlignment = DismModuledetectNewAlignment;
  obj->detectOpcodeAddOrRemove = DismModuledetectOpcodeAddOrRemove;
  obj->detectOpcodeAddition = DismModuledetectOpcodeAddition;
  obj->matchSeq = DismModulematchSeq;
  obj->readNextOp = DismModulereadNextOp;
  obj->tryOpRead = DismModuletryOpRead;
  obj->matchOp = DismModulematchOp;
  obj->byteAlignment = DismModulebyteAlignment;
  obj->matchSequentialOps = DismModulematchSequentialOps;
  obj->destroy = DismModuledestroy;
  obj->destroyInternal = DismModuledestroyInternal;
  obj->enableOpArgCollation_ = 1;
  obj->byteAlignment_ = 1;
  obj->data_ = NULL;
}

DismModule* allocDismModule() {
  DismModule* obj = malloc(sizeof(DismModule));
  initDismModule(obj);
  return obj;
}

void freeDismModule(DismModule* obj) {
  obj->destroy(obj);
  
  free(obj);
}

void assignOpcodeIDs(VectorOpInfoArray* opInfoArrays) {
  unsigned int id = 0;
  unsigned int limit = opInfoArrays->size(opInfoArrays);
  unsigned int i;
  for (i = 0; i < limit; i++) {
    
    OpInfoArray* opInfoArray = opInfoArrays->getP(opInfoArrays, i);
    OpInfo* array = opInfoArray->array;
    unsigned int numOps = opInfoArray->size;
    
    int j;
    for (j = 0; j < numOps; j++) {
      array[j].id = id++;
    }
    
  }
}
