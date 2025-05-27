#include "mem.h"

byte lmgb::Memory::Read(word addr) {
  if (addr < 0 || addr > 0xFFFF)
    return 0;
  return mem[addr];
}

void lmgb::Memory::Write(word addr, byte val) {
  if (addr < 0 || addr > 0xFFFF)
    return;
  mem[addr] = val;
}
