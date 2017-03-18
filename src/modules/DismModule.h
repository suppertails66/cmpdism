#ifndef CPD_DISM_MODULE_H
#define CPD_DISM_MODULE_H

#include "util/Vectors.h"
#include "modules/DismSettings.h"
#include "modules/DismStruct.h"
#include "modules/Opcode.h"

/**
 * Enum of OpcodeAlignmentData result types.
 */
typedef enum {
  opcodeAlignmentSame,
  opcodeAlignmentAdded,
  opcodeAlignmentRemoved,
  opcodeAlignmentTransformed
} OpcodeAlignmentType;

/**
 * Data struct for returning results from Opcode stream alignment functions.
 */
typedef struct {

  /**
   * New position in A.
   */
  unsigned int iA;

  /**
   * New position in B.
   */
  unsigned int iB;
  
  /**
   * Type of change detected.
   */
  OpcodeAlignmentType alignmentType;
  
  /**
   * Length of the transform block, if it exists.
   */
  int transformBlockLength;

} OpcodeAlignmentData;

/**
 * A module capable of performing disassembly and comparison tasks.
 * Default implementations are provided here which should suffice for most
 * purposes.
 */
typedef struct DismModule {

  /* PUBLIC */

  /**
   * Main execution function.
   * When called, this does any and all work specified by the DismSettings.
   */
  void (*run)(struct DismModule* obj, DismSettings config);

  /* PRIVATE */
  
  /**
   * Vector of arrays of ops used for current module.
   * Derived classes must, at a minimum, fill this with pointers to the
   * OpInfo arrays containing the ops used for disassembly.
   */
  VectorOpInfoArray opInfoArrays;
  
  /**
   * Additional parameter check function.
   */
  void (*checkAdditionalParams)(struct DismModule* obj, DismSettings config);
  
  /**
   * Parsing function.
   */
  void (*parse)(struct DismModule* obj, DismStruct* dismStruct);
  
  /**
   * Opcode stream generator function.
   */
  void (*disassemble)(struct DismModule* obj, DismStruct* dismStruct,
                      unsigned int start, unsigned int end);
  
  /**
   * Simple disassembly printer.
   * This prints the content of a single disassembly stream to a String.
   */
  void (*printDisassembly)(struct DismModule* obj, String* dst,
                           DismStruct* dismStruct);
  
  /**
   * Disassembly comparison printer.
   * This prints the compared content of two disassembly streams to a String.
   */
  void (*printComparedDisassembly)(struct DismModule* obj, String* dst,
                                   DismStruct* firstDismStruct,
                                   DismStruct* secondDismStruct);
  
  void (*generateCodeMap)(struct DismModule* obj, DismStruct* dismStruct,
                          DismSettings config);
                                   
  /**
   * Determines realignment positions and type of two Opcode streams.
   */
  OpcodeAlignmentData (*detectNewAlignment)(struct DismModule* obj,
                                   DismStruct* dismStructA,
                                   DismStruct* dismStructB,
                                   unsigned int iA,
                                   unsigned int iB,
                                   unsigned int limit,
                                   unsigned int seqLen,
                                   unsigned int maxDist);
                                   
  /**
   * Attempts to discover additions or removals in Opcode stream B.
   */
  OpcodeAlignmentData (*detectOpcodeAddOrRemove)(struct DismModule* obj,
                                   DismStruct* dismStructA,
                                   DismStruct* dismStructB,
                                   unsigned int iA,
                                   unsigned int iB,
                                   unsigned int limit,
                                   unsigned int seqLen,
                                   unsigned int maxDist);
                                   
  /**
   * Attempts to discover an addition to Opcode stream B.
   */
  OpcodeAlignmentData (*detectOpcodeAddition)(struct DismModule* obj,
                                   DismStruct* dismStructA,
                                   DismStruct* dismStructB,
                                   unsigned int iA,
                                   unsigned int iB,
                                   unsigned int limit,
                                   unsigned int seqLen,
                                   unsigned int maxDist);
                  
  /**
   * Returns nonzero if the Opcode token sequences at the given positions
   * and of the given length have matching IDs in A and B.
   */
  int (*matchSeq)(struct DismModule* obj,
                                   DismStruct* dismStructA,
                                   DismStruct* dismStructB,
                                   unsigned int iA,
                                   unsigned int iB,
                                   unsigned int len);
  
  /**
   * Reads the next op from the DismStruct's stream, and advances the stream.
   */
  void (*readNextOp)(struct DismModule* obj, DismStruct* dismStruct,
                          unsigned int remaining);
  
  int (*tryOpRead)(struct DismModule* obj, DismStruct* dismStruct,
                   OpInfoArray ops, unsigned int remaining);
  
  int (*matchOp)(struct DismModule* obj, DismStruct* dismStruct,
                 OpInfo* opInfo, unsigned int remaining);
                 
  int (*byteAlignment)(struct DismModule* obj);
  
  int (*matchSequentialOps)(struct DismModule* obj, int numOps,
                            DismStruct* dismStruct);
  
  /**
   * Enables automatic op argument collation if set.
   *
   * @see collateOpArgs()
   */
  int enableOpArgCollation_;
  
  /**
   * Pointer to additional data specific to each module.
   */
  void* data_;
  
  /**
   * Instruction byte alignment requirement.
   * 2 = 16-bit alignment, 4 = 32-bit alignment, etc. */
  int byteAlignment_;
  
  /**
   * Destructor.
   * 
   * Default implementation destroys opInfoArrays and frees data_. If data_
   * itself uses dynamically allocated memory, the implementation should
   * override this function to free it.
   *
   * @see destroyInternal()
   */
  void (*destroy)(struct DismModule* obj);
  
  /**
   * Internal destructor.
   * 
   * This is called by destroy(). Modules which dynamically allocate memory
   * within data_ should override this function to clean it up.
   * 
   * Default implementation does nothing.
   *
   * @see destroy()
   */
  void (*destroyInternal)(struct DismModule* obj);
  
} DismModule;

void DismModulerun(DismModule* obj, DismSettings config);

void DismModulecheckAdditionalParams(DismModule* obj, DismSettings config);

void DismModuleparse(DismModule* obj, DismStruct* dismStruct);

void DismModuledisassemble(DismModule* obj, DismStruct* dismStruct,
                           unsigned int start, unsigned int end);
                           
void DismModuleprintDisassembly(DismModule* obj, String* dst,
                                DismStruct* dismStruct);
                           
void DismModuleprintComparedDisassembly(DismModule* obj, String* dst,
                                DismStruct* firstDismStruct,
                                DismStruct* secondDismStruct);
  
void DismModulegenerateCodeMap(DismModule* obj, DismStruct* dismStruct,
                        DismSettings config);

OpcodeAlignmentData DismModuledetectNewAlignment(struct DismModule* obj,
                                 DismStruct* dismStructA,
                                 DismStruct* dismStructB,
                                 unsigned int iA,
                                 unsigned int iB,
                                 unsigned int limit,
                                 unsigned int seqLen,
                                 unsigned int maxDist);

OpcodeAlignmentData DismModuledetectOpcodeAddOrRemove(struct DismModule* obj,
                                 DismStruct* dismStructA,
                                 DismStruct* dismStructB,
                                 unsigned int iA,
                                 unsigned int iB,
                                 unsigned int limit,
                                 unsigned int seqLen,
                                 unsigned int maxDist);

OpcodeAlignmentData DismModuledetectOpcodeAddition(struct DismModule* obj,
                                 DismStruct* dismStructA,
                                 DismStruct* dismStructB,
                                 unsigned int iA,
                                 unsigned int iB,
                                 unsigned int limit,
                                 unsigned int seqLen,
                                 unsigned int maxDist);

int DismModulematchSeq(struct DismModule* obj,
                                 DismStruct* dismStructA,
                                 DismStruct* dismStructB,
                                 unsigned int iA,
                                 unsigned int iB,
                                 unsigned int len);

void DismModulereadNextOp(DismModule* obj, DismStruct* dismStruct,
                          unsigned int remaining);
  
int DismModuletryOpRead(DismModule* obj, DismStruct* dismStruct,
                        OpInfoArray ops, unsigned int remaining);
  
int DismModulematchOp(DismModule* obj, DismStruct* dismStruct,
                      OpInfo* opInfo, unsigned int remaining);
                 
int DismModulebyteAlignment(DismModule* obj);
  
int DismModulematchSequentialOps(DismModule* obj, int numOps,
                          DismStruct* dismStruct);
                      
void DismModuledestroy(DismModule* obj);
                      
void DismModuledestroyInternal(DismModule* obj);

void initDismModule(DismModule* obj);
DismModule* allocDismModule();
void freeDismModule(DismModule* obj);

void assignOpcodeIDs(VectorOpInfoArray* opInfoArrays);


#endif
