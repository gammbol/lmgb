#ifndef LMGB_MEM_H
#define LMGB_MEM_H

#include "defs.h"

// include all the mbcs here for now
#include "mbc.h"
#include "mbc1.h"
#include "mbc_nombc.h"

// #define MEM_LEN 0x10000

namespace lmgb {

enum MBC_TYPES { ROM_ONLY, MBC1, MBC1_RAM };

enum ROM_SIZES {
  ROM_KIB_32,
  ROM_KIB_64,
  ROM_KIB_128,
  ROM_KIB_256,
  ROM_KIB_512
};

enum RAM_SIZES {
  RAM_NO_RAM,
  RAM_UNUSED,
  RAM_KIB_8,
  RAM_KIB_32,
  RAM_KIB_128,
  RAM_KIB_64
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
