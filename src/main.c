#include "util/Logger.h"
#include "modules/DismSettings.h"
#include "modules/Modules.h"
#include <stdio.h>

void printProgramHelp() {
  printf("cmpdism: multi-architecture comparative disassembler\n"
"Usage: cmpdism <module> [options]\n\n"
  
"Available modules:"
  );
  
  int i;
  for (i = 0; i < numDismModules; i++)  {
    printf(" %s", dismModules[i].moduleName);
  }
  printf("\n\n");
  
  printf(
  
"Options:\n\n"
  
"  Substitute # symbols with 1 or 2 to specify the first or second input\n"
"  file, respectively.\n\n"
"  -i# <filename>\t\tSpecifies input filename(s).\n"
"  -s# <offset>\t\t\tSpecifies starting offset of disassembly.\n"
"  -f# <offset>\t\t\tSpecifies ending offset of disassembly.\n"
"  -o <offset>\t\t\tSpecifies load offset of data\n"
"  -r, --realign-len <length>\tNumber of ops which must be sequentially\n"
"  \t\t\t\tmatched before two op streams are considered\n"
"  \t\t\t\trealigned (default: 16).\n"
"  -m, --max-search-len <length>\tMaximum number of ops to search ahead to try\n"
"  \t\t\t\tto find a matching sequence (default: 128).\n"
"  --addr-width <length>\t\tWidth (padding) of printed op addresses.\n"
"  --middle-width <length>\tWidth (padding) of printed disassembly.\n"
"  --dual-separation <length>\tWidth (padding) of comparison columns.\n"
"  --addresses-distinct\t\tRegards changed addresses as a distinctive\n"
"\t\t\t\tdifference.\n"
"  --addresses-similar\t\tRegards changed addresses as a nondistinctive\n"
"\t\t\t\tdifference (default).\n"
"  --constants-distinct\t\tRegards changed constants as a distinctive\n"
"\t\t\t\tdifference (default).\n"
"  --constants-similar\t\tRegards changed constants as a nondistinctive\n"
"\t\t\t\tdifference.\n"
"  --raw-bytes\t\t\tPrints raw byte representatino of disassembled\n"
"\t\t\t\tcontent.\n"
"  --generate-codemaps\t\tAttempts to distinguish code from data.\n"
"  --codemap-seq-req\t\tSets number of sequential valid ops required\n"
"\t\t\t\tto consider a section as code.\n"
"\n"
"There are two program modes: one-file and two-file. If only the -i1\n"
"parameter is given, one-file mode (simple disassembly) is used. If both\n"
"-i1 and -i2 are given, two-file mode (comparative disassembly) is used.\n"
"\n"
"A typical command will look something like this:\n\n"
"  cmpdism 6502 -i1 file1.nes -i2 file2.nes\n\n"
"This will disassemble file1 and file2 as 6502 code, compare the "
"disassemblies,\n"
"and print the differences to standard output.\n\n"

"In comparative mode, the most important parameters are -r and -m.\n\n"

"* The higher -r is set, the less likely the comparison is to realign at an\n"
"incorrect position, leading to spurious change reports, but the more likely"
" it\n"
"is that a series of small changes will be misdetected as one large block.\n"
"* The higher -m is set, the more likely it is the streams will be mistakenly\n"
"realigned, but the less likely it is for added/removed code to be missed.\n\n"
  );
};

int main(int argc, char* argv[]) {

  initLogger();
  
  if (argc <= 1) {
    printProgramHelp();
    return 0;
  }
  
  const DismModuleInfo* moduleInfo = NULL;
  int i;
  for (i = 0; i < numDismModules; i++) {
    if (strcmp(argv[1], dismModules[i].moduleName) == 0) {
      moduleInfo = &(dismModules[i]);
    }
  }
  
  if (moduleInfo == NULL) {
    error("Unknown module: ");
    error(argv[1]);
    fatal();
  }
  
  DismModule module;
  moduleInfo->moduleInit(&module);
  
  DismSettings settings;
  initDismSettings(&settings);
  fillDismSettings(&settings, argc, argv);
  settings.module = &module;
  
  module.run(&module, settings);
 
  module.destroy(&module);
  
  return 0;
}
