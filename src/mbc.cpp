#include "mbc.h"

lmgb::mbc::mbc(const word romSize, const word ramSize, byte *rom, byte *ram)
    : romSize(romSize), ramSize(ramSize), rom(rom), ram(ram) {
  selectedRom = 1;
  selectedRam = 0;
  romOffset = selectedRam * romBankSize;
  ramOffset = selectedRam * ramBankSize;
  ramEnable = false;
}

lmgb::mbc1::mbc1(word romSize, word ramSize, byte *rom, byte *ram)
    : mbc(romSize, ramSize, rom, ram) {
  advancedMode = false;
}

byte lmgb::mbc1::read(const word addr) {
  if (addr < 0 || addr > 0xffff)
    throw std::exception("Invalid address");

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
    throw std::exception("Invalid mbc read");
  }
}

void lmgb::mbc1::write(word addr, byte val) {
  if (addr < 0 || addr > 0xffff)
    throw std::exception("Invalid address");

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
    throw std::exception("Invalid mbc write");
  }
}
