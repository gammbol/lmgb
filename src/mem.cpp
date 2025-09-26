#include "mem.h"

byte lmgb::Memory::Read(word addr) {
  if (addr < 0 || addr > 0xFFFF)
    return 0;

  switch (addr & 0xf000) {
  case 0x0000:
  case 0x1000:
  case 0x2000:
  case 0x3000:
  case 0x4000:
  case 0xa000:
    // lmgb::mbc::read(addr);
    break;
  }
  return mem[addr];
}

void lmgb::Memory::Write(word addr, byte val) {
  if (addr < 0 || addr > 0xFFFF)
    return;
  mem[addr] = val;
}
