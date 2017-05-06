#include "modules/Modules.h"
#include "modules/65C02/Module65C02.h"
#include "modules/6502/Module6502.h"
#include "modules/68000/Module68000.h"
#include "modules/Data/ModuleData.h"
#include "modules/HuC6280/ModuleHuC6280.h"
#include "modules/LR35902/ModuleLR35902.h"
#include "modules/MIPS1/ModuleMIPS1.h"
#include "modules/SH2/ModuleSH2.h"
#include "modules/Z80/ModuleZ80.h"

DismModuleInfo dismModules[] = {
  { "65c02", initModule65C02, allocModule65C02, freeModule65C02 },
  { "6502", initModule6502, allocModule6502, freeModule6502 },
  { "68000", initModule68000, allocModule68000, freeModule68000 },
  { "data", initModuleData, allocModuleData, freeModuleData },
  { "huc6280", initModuleHuC6280, allocModuleHuC6280, freeModuleHuC6280 },
  { "lr35902", initModuleLR35902, allocModuleLR35902, freeModuleLR35902 },
  { "mips1", initModuleMIPS1, allocModuleMIPS1, freeModuleMIPS1 },
  { "sh2", initModuleSH2, allocModuleSH2, freeModuleSH2 },
  { "z80", initModuleZ80, allocModuleZ80, freeModuleZ80 }
};

int numDismModules = sizeof(dismModules) / sizeof(DismModuleInfo);
