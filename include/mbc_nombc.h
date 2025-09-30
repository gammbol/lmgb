#ifndef LMGB_NOMBC_H
#define LMGB_NOMBC_H

#include "mbc.h"

namespace lmgb {
class nombc : public mbc {
public:
  nombc(word romSize, word ramSize, byte *rom, byte *ram);

  byte read(word addr) override {
    switch (addr & 0xf000) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
      return rom[addr];

    case 0xa000:
    case 0xb000:
      return ram[addr];

    default:
      return 0;
    }
  }

  void write(word addr, byte val) override {
    switch (addr & 0xf000) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
      return;

    case 0xa000:
    case 0xb000:
      ram[addr] = val;
    }
  }
};
} // namespace lmgb

#endif