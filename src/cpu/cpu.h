#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include "lmgb.h"
#include "mem.h"

#define SYNC_WITH_CPU(clockDelta)

namespace lmgb {
  // pair of registers
  union WordRegister {
    struct {
      byte l;
      byte h;
    } bytes;
    word pair;
  };
  
  class Cpu {
  public:
    WordRegister af, bc, de, hl;
    word sp;
    lmgb::Memory mem;

    Cpu();
    // ~Cpu();

    void Step();
    void readOp();
  };
}

#endif