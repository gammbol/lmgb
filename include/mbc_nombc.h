#ifndef LMGB_NOMBC_H
#define LMGB_NOMBC_H

#include "mbc.h"

namespace lmgb {
class nombc : public mbc {
public:
  nombc(ROM_SIZES rom_size, RAM_SIZES ram_size, std::vector<byte> &rom_data)
      : mbc(rom_size, ram_size, rom_data) {}

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
    case 0xb000: {
      if (ramSize != 0)
        ram[addr] = val;
      break;
    }
    }
  }

private:
  byte *loadRom(const char *path) override { return nullptr; }
  byte *loadRam(const char *path) override { return nullptr; }
  void saveRam(const char *path) override { return; }
};
} // namespace lmgb

#endif