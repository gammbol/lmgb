#ifndef LMGB_MBC1_H
#define LMGB_MBC1_H

#include "mbc.h"

namespace lmgb {
class mbc1 : public mbc {
public:
  mbc1(word romSize, word ramSize, byte *rom, byte *ram)
      : mbc(romSize, ramSize, rom, ram) {
    advancedMode = false;
  }

  byte read(word addr) override {
    switch (addr & 0xf000) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
      return rom[addr];

    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
      return rom[addr - 0x4000 + romOffset];

    case 0xa000:
    case 0xb000:
      if (ramEnable) {
        return ram[addr - 0xa000 + ramOffset];
      }

    default:
      return -1;
    }
  }
  void write(word addr, byte val) override {
    switch (addr & 0xf000) {
    case 0x0000:
    case 0x1000:
      ramEnable = val == 0xa;
      break;

    case 0x2000:
    case 0x3000:
      selectedRom = val & 0x1f;
      if (!advancedMode && selectedRom == 0) {
        selectedRom = 1;
      }
      break;

    case 0x4000:
    case 0x5000:
      selectedRom = (val << 5) + selectedRom;
      selectedRam = val & 0x0f;
      if (advancedMode) {
        selectedRam = val & 0x0f;
      }
      break;

    default:
      return;
    }
  }

protected:
  bool advancedMode;
};
} // namespace lmgb

#endif