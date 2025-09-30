#ifndef LMGB_MEM_H
#define LMGB_MEM_H

#include "lmgb.h"
#include "mbc.h"

#define MEM_LEN 0x10000

namespace lmgb {
class Memory {
  mbc mbc;

public:
  byte Read(word addr);
  void Write(word addr, byte val);
};
} // namespace lmgb

#endif
