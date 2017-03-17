#ifndef CPD_DISM_SETTINGS_H
#define CPD_DISM_SETTINGS_H


struct DismModule;
struct DismStruct;

/**
 * Data struct containing various configurable settings for disassembly.
 * Most or all of these can be set by the user via command line switches.
 */
typedef struct {

  /*---------- GENERAL SETTINGS ----------*/

  /**
   * Command line parameter count from main().
   */
  int argc;
  
  /**
   * Command lines parameters from main().
   * Modules can check these for module-specific settings.
   */
  char** argv;
  
  /**
   * Name of the first input file.
   */
  char* firstFile;
  
  /**
   * Name of the second input file.
   * May be the null string, in which case comparison mode is implicitly
   * disabled.
   */
  char* secondFile;
  
  /**
   * Starting offset of disassembly within the first file.
   */
  unsigned int firstFileStartOffset;
  
  /**
   * Starting offset of disassembly within the second file.
   */
  unsigned int secondFileStartOffset;
  
  /**
   * Ending offset of disassembly within the first file.
   */
  unsigned int firstFileEndOffset;
  
  /**
   * Ending offset of disassembly within the second file.
   */
  unsigned int secondFileEndOffset;
  
  /*---------- CODEMAP SETTINGS ----------*/
  
  /**
   * If not the null string, specifies the name of an codemap to load from
   * an external file to use for the first file.
   * If this option is used, intelligent code detection and parsing are
   * disabled.
   */
  char* firstFileExternalCodeMap;
  
  /**
   * If not the null string, specifies the name of an codemap to load from
   * an external file to use for the second file.
   * If this option is used, intelligent code detection and parsing are
   * disabled.
   */
  char* secondFileExternalCodeMap;
  
  /*---------- PARSE SETTINGS ----------*/
  
  /**
   * Load address of data, expressed as a hexadecimal-formatted string.
   * Load address use disabled if the null string.
   */
  char* loadAddress;
  
  /*---------- OP TOKENIZATION SETTINGS ----------*/
  
  /**
   * Enables intelligent code detection if set.
   * If on, the op tokenizer will attempt to distinguish between data and
   * code areas using various heuristics. Otherwise, it will simply treat
   * everything as code.
   */
  int intelligentCodeDetection;
  
  /*---------- PRINT SETTINGS ----------*/
  
  /**
   * Number of characters to print in source addresses.
   */
  int singleSrcAddrW;
  int singleShowRaw;
  int singleMiddleWidth;
  
  int dualSrcAddrW;
  int dualShowRaw;
  int dualMiddleWidth;
  int dualSeparationW;
  
  /*---------- COMPARISON SETTINGS ----------*/
  
  /**
   * If nonzero, address changes are considered meaningful when comparing ops.
   */
  int addressChangesDistinct;
  
  /**
   * If nonzero, constant changes are considered meaningful when comparing ops.
   */
  int constantChangesDistinct;
  
  /**
   * Number of sequential ops that must be matched to consider two blocks of
   * code "the same".
   * Higher values tend to increase accuracy, but will increase the likelihood
   * of multiple additions or removals over a small area being detected as
   * a block transformation instead. 
   */
  int requiredSequentialOps;
  
  /**
   * Maximum size for added/removed code blocks to check for.
   * Specifically, this is the number of bytes that will be searched ahead
   * for a matching op sequence.
   * Higher values increase detection rate at the expense of speed.
   */
  int maxChangeBlockSize;
  
  struct DismModule* module;

} DismSettings;

void initDismSettings(DismSettings* obj);
DismSettings* allocDismSettings();
void freeDismSettings(DismSettings* obj);

void fillDismSettings(DismSettings* obj, int argc, char* argv[]);


#endif
