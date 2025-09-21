#ifndef LMGB_MBC_H
#define LMGB_MBC_H

#include "lmgb.h"

namespace lmgb {

const int romBankSize = 0x4000;
const int ramBankSize = 0x2000;

// TODO: handle no ram scenario
class mbc {
public:
  mbc(word romSize, word ramSize, byte *rom, byte *ram);

  virtual byte read(word addr);
  virtual void write(word addr, byte val);

protected:
  bool ramEnable;

  byte selectedRom;
  byte selectedRam;
  word romOffset;
  word ramOffset;

  word romSize; // number of banks
  word ramSize; // number of banks

  byte *rom;
  byte *ram;
};

} // namespace lmgb

#endif