#include "mem.h"

lmgb::mem::mem() { mbc = new mbc1(); }

byte lmgb::mem::Read(word addr) {
  switch (addr & 0xf000) {
  case 0x0000:
  case 0x1000:
  case 0x2000:
  case 0x3000:
  case 0x4000:
  case 0xa000:
  case 0xb000:
    return mbc->read(addr);

  default:
    return -1;
  }
}

void lmgb::mem::Write(word addr, byte val) {
  switch (addr & 0xf000) {
  case 0x0000:
  case 0x1000:
  case 0x2000:
  case 0x3000:
  case 0x4000:
  case 0x5000:
  case 0x6000:
  case 0x7000:
  case 0xa000:
  case 0xb000:
    mbc->write(addr, val);

  default:
    return;
  }
}
