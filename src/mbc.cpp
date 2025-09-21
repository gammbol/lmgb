#include "mbc.h"

#include <exception>

lmgb::mbc::mbc(const word romSize, const word ramSize, byte *rom, byte *ram)
    : romSize(romSize), ramSize(ramSize), rom(rom), ram(ram) {
  selectedRom = 1;
  selectedRam = 0;
  romOffset = selectedRam * romBankSize;
  ramOffset = selectedRam * ramBankSize;
  ramEnable = false;
}

byte lmgb::mbc::read(const word addr) {
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
    return ram[addr - 0xa000 + ramOffset];

  default:
    throw std::exception("Invalid mbc read");
  }
}

void lmgb::mbc::write(word addr, byte val) {
  if (addr < 0 || addr > 0xffff)
    throw std::exception("Invalid addres");

  switch (addr & 0xf000) {
  case 0x0000:
  case 0x1000:
    ramEnable = val == 0xa;
    break;

  case 0x2000:
  case 0x3000:
    selectedRom = val ? 1 : val & 0x1f;

    // TODO: range validation
  case 0x4000:
  case 0x5000:
    selectedRom = (val & 0x60) | selectedRom;
    selectedRam = val & 0x0f;

  default:
    throw std::exception("Invalid mbc write");
  }
}

// byte lmgb::mbc2::read(word addr) {

// }