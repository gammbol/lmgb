#ifndef LMGB_MBC_H
#define LMGB_MBC_H

#include "mem.h"

namespace lmgb {

const int ROMBankSize = 0x4000;
const int RAMBankSize = 0x2000;

class mbc {
  char type;
  char selectedBank;

public:
  mbc();
};
} // namespace lmgb

#endif