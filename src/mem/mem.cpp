#include "mem.h"

uint8_t lmgb::Memory::Read(uint16_t addr) {
  if (addr < 0 || addr > 0xFFFF)
    return 0;
  return mem[addr];
}

void lmgb::Memory::Write(uint16_t addr, uint8_t val) {
  if (addr < 0 || addr > 0xFFFF)
    return;
  mem[addr] = val;
}
