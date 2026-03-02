#include <mbc1.h>

byte lmgb::mbc1::read(word addr) {
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

void lmgb::mbc1::write(word addr, byte val) {
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

// CARTRIDGE LOADING
byte *lmgb::mbc1::loadRom(const char *path) {
  
}