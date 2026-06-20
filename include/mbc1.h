#ifndef LMGB_MBC1_H
#define LMGB_MBC1_H

#include <mbc.h>

namespace lmgb {
class mbc1 : public mbc {
public:
  mbc1(ROM_SIZES rom_size, RAM_SIZES ram_size, std::vector<byte> &rom_data)
      : mbc(rom_size, ram_size, rom_data) {
    selectedRom = 1;
    update_rom_offset();
  }

  byte read(word addr) override {
    if (addr <= 0x3fff) {
      return safe_rom_read(addr);
    }

    if (addr >= 0x4000 && addr <= 0x7fff) {
      return safe_rom_read(romOffset + (addr - 0x4000));
    }

    if (addr >= 0xa000 && addr <= 0xbfff) {
      if (!ramEnable || ramSize == 0) return 0xff;
      return safe_ram_read(ramOffset + (addr - 0xa000));
    }

    return 0xff;
  }

  void write(word addr, byte val) override {
    if (addr <= 0x1fff) {
      ramEnable = (val & 0x0f) == 0x0a;
      return;
    }

    if (addr >= 0x2000 && addr <= 0x3fff) {
      byte low = val & 0x1f;
      if (low == 0) low = 1;
      selectedRom = static_cast<byte>((selectedRom & 0x60) | low);
      update_rom_offset();
      return;
    }

    if (addr >= 0x4000 && addr <= 0x5fff) {
      byte two_bits = val & 0x03;
      if (advancedMode) {
        selectedRam = two_bits;
        ramOffset = (ramSize == 0) ? 0 : (selectedRam % ramSize) * ramBankSize;
      } else {
        selectedRom = static_cast<byte>((selectedRom & 0x1f) | (two_bits << 5));
        if ((selectedRom & 0x1f) == 0) selectedRom |= 1;
        update_rom_offset();
      }
      return;
    }

    if (addr >= 0x6000 && addr <= 0x7fff) {
      advancedMode = (val & 0x01) != 0;
      return;
    }

    if (addr >= 0xa000 && addr <= 0xbfff) {
      if (ramEnable && ramSize != 0) {
        safe_ram_write(ramOffset + (addr - 0xa000), val);
      }
      return;
    }
  }

private:
  bool advancedMode = false;

  void update_rom_offset() {
    if (romSize == 0) {
      romOffset = 0;
      return;
    }

    std::size_t bank = selectedRom % romSize;
    if (bank == 0 && romSize > 1) bank = 1;
    romOffset = bank * romBankSize;
  }
};
} // namespace lmgb

#endif
