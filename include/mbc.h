#ifndef LMGB_MBC_H
#define LMGB_MBC_H

#include "defs.h"

namespace lmgb {

const int romBankSize = 0x4000;
const int ramBankSize = 0x2000;

// TODO: handle no ram scenario
class mbc {
public:
  // mbc() = delete;
  // virtual ~mbc() = 0;

  virtual byte read(word addr) = 0;
  virtual void write(word addr, byte val) = 0;

protected:
  bool ramEnable = false;

  byte selectedRom = 0;
  byte selectedRam = 0;
  word romOffset = 0;
  word ramOffset = 0;

  word romSize = 0; // number of banks
  word ramSize = 0; // number of banks

  byte *rom = nullptr;
  byte *ram = nullptr;

  virtual byte *loadRom(const char *path) = 0;
  virtual byte *loadRam(const char *path) = 0;

  virtual void saveRam(const char *path) = 0;
};

} // namespace lmgb

#endif
