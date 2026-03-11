#ifndef LMGB_H
#define LMGB_H

#include <fstream>
#include <vector>
#include <optional>

#include <enums.h>

// gameboy elements
#include <cpu.h>
#include <renderer.h>

namespace lmgb {
class gb {
  char game_title[16];
  MBC_TYPES mbc_type;
  ROM_SIZES rom_size;
  RAM_SIZES ram_size;

  std::vector<lmgb::byte> rom_data;

  cpu *lmgb_cpu;
  renderer *rndr;
  

public:
  gb(const char *path);
  ~gb();

  void Step();
};

} // namespace lmgb

#endif
