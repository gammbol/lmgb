#ifndef LMGB_H
#define LMGB_H

#include <fstream>
#include <vector>

#include <enums.h>

// gameboy elements
#include "cpu.h"
#include "interrupts.h"
#include "timer.h"

namespace lmgb {
class gb {
  char game_title[16];
  MBC_TYPES mbc_type;
  ROM_SIZES rom_size;
  RAM_SIZES ram_size;

  std::vector<byte> rom_data;

  lmgb::cpu *cpu;

public:
  gb(const char *path);
  // ~gb();  
};

} // namespace lmgb

#endif
