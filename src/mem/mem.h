#ifndef MEM_H
#define MEM_H

#include "lmgb.h"

#define MEM_LEN 0x10000

namespace lmgb {
  class Memory {
    byte mem[MEM_LEN];
    public:
      byte Read(word addr);
      void Write(word addr, byte val);
  };
}

#endif
