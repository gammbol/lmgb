#ifndef LMGB_NOMBC_H
#define LMGB_NOMBC_H

#include <mbc.h>

namespace lmgb {
class nombc : public mbc {
public:
  nombc(ROM_SIZES rom_size, RAM_SIZES ram_size, std::vector<byte> &rom_data)
      : mbc(rom_size, ram_size, rom_data) {}

  byte read(word addr) override {
    if (addr <= 0x7fff) {
      return safe_rom_read(addr);
    }

    if (addr >= 0xa000 && addr <= 0xbfff) {
      return safe_ram_read(addr - 0xa000);
    }

    return 0xff;
  }

  void write(word addr, byte val) override {
    if (addr >= 0xa000 && addr <= 0xbfff) {
      safe_ram_write(addr - 0xa000, val);
    }
  }
};
} // namespace lmgb

#endif
