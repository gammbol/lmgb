#ifndef LMGB_MBC_H
#define LMGB_MBC_H

#include "defs.h"

namespace lmgb {

const int romBankSize = 0x4000;
const int ramBankSize = 0x2000;

// TODO: handle no ram scenario
class mbc {
public:
  mbc(word romSize, word ramSize, byte *rom, byte *ram)
      : romSize(romSize), ramSize(ramSize), rom(rom), ram(ram) {
    selectedRom = 1;
    selectedRam = 0;
    romOffset = selectedRam * romBankSize;
    ramOffset = selectedRam * ramBankSize;
    ramEnable = false;
  }

  virtual byte read(word addr) = 0;
  virtual void write(word addr, byte val) = 0;

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

  virtual byte *loadRom(const char *path) = 0;
  virtual byte *loadRam(const char *path) = 0;

  virtual void saveRam(const char *path) = 0;
};

} // namespace lmgb

#endif
