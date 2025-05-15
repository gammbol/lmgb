#ifndef MEM_H
#define MEM_H

#include "lmgb.h"

namespace lmgb {
  class Memory {
    byte mem[0x10000];
    public:
      byte Read(word addr);
      void Write(word addr, byte val);
  };
}

#endif
