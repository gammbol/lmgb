#include "mbc.h"

byte lmgb::mbc::read(word addr) {
  if (addr < 0 || addr > 0xffff)
    throw -1;

  return ((addr - 0xa000) + (selectedBank * RAMBankSize)) % 0xdfff;
}