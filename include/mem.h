#ifndef LMGB_MEM_H
#define LMGB_MEM_H

#include "defs.h"

// include all the mbcs here for now
#include "mbc.h"
#include "mbc1.h"
#include "mbc_nombc.h"

// #define MEM_LEN 0x10000

namespace lmgb {

enum MBC_TYPES {
  ROM_ONLY,
  MBC1,
  MBC1_RAM
};

enum ROM_SIZES {
  KIB_32,
  KIB_64,
  KIB_128,
  KIB_256,
  KIB_512
};

enum RAM_SIZES {
  NO_RAM,
  UNUSED,
  KIB_8,
  KIB_32,
  KIB_128,
  KIB_64
};

class mem {
  lmgb::mbc *mbc;

public:
  mem();
  byte Read(word addr);
  void Write(word addr, byte val);
};
} // namespace lmgb

#endif
