#ifndef LMGB_MBC_H
#define LMGB_MBC_H

#include "mem.h"

namespace lmgb {

const int ROMBankSize = 0x4000;
const int RAMBankSize = 0x2000;

class mbc {
public:
  mbc(byte *ROM, word ROMSize, byte *RAM, word RAMSize)
      : ROM(ROM), ROMSize(ROMSize), RAM(RAM), RAMSize(RAMSize) {}

  byte read(word addr);
  byte write(word addr, byte val);

protected:
  char selectedBank;
  byte *ROM; // rom of the game file
  byte *RAM; // ram of the game file

  word ROMSize; // size in blocks
  word RAMSize; // size in blocks
};
} // namespace lmgb

#endif