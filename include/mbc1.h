#ifndef LMGB_MBC1_H
#define LMGB_MBC1_H

#include <mbc.h>

namespace lmgb {
class mbc1 : public mbc {
public:
  // unsure about here
  // real thinking needed
  mbc1(ROM_SIZES roms, RAM_SIZES rams) : rom_size(roms), ram_size(rams) {
    advancedMode = false;
  }

  ROM_SIZES rom_size;
  RAM_SIZES ram_size;

  byte read(word addr) override;
  void write(word addr, byte val) override;

private:
  bool advancedMode = false;

  byte *loadRom(const char *path) override;
  byte *loadRam(const char *path) override;
  void saveRam(const char *path) override;
};
} // namespace lmgb

#endif