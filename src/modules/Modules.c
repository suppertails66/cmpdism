#include "modules/Modules.h"
#include "modules/65C02/Module65C02.h"
#include "modules/6502/Module6502.h"
#include "modules/68000/Module68000.h"
#include "modules/HuC6280/ModuleHuC6280.h"
#include "modules/MIPS1/ModuleMIPS1.h"
#include "modules/SH2/ModuleSH2.h"

DismModuleInfo dismModules[] = {
  { "65c02", initModule65C02, allocModule65C02, freeModule65C02 },
  { "6502", initModule6502, allocModule6502, freeModule6502 },
  { "68000", initModule68000, allocModule68000, freeModule68000 },
  { "huc6280", initModuleHuC6280, allocModuleHuC6280, freeModuleHuC6280 },
  { "mips1", initModuleMIPS1, allocModuleMIPS1, freeModuleMIPS1 },
  { "sh2", initModuleSH2, allocModuleSH2, freeModuleSH2 }
};

int numDismModules = sizeof(dismModules) / sizeof(DismModuleInfo);
