#ifndef MEM_H
#define MEM_H

#include <cstdint>
#include "lmgb.h"

namespace lmgb {
  class Memory {
    byte mem[0x10000];
    public:
      void Write(word addr, word val);
      byte Read(word addr);
  };
}

#endif
