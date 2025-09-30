#ifndef LMGB_MEM_H
#define LMGB_MEM_H

#include "lmgb.h"

// include all the mbcs here for now
#include "mbc.h"
#include "mbc1.h"
#include "mbc_nombc.h"

#define MEM_LEN 0x10000

namespace lmgb {
class Memory {
  mbc mbc;

public:
  Memory();
  byte Read(word addr);
  void Write(word addr, byte val);
};
} // namespace lmgb

#endif
