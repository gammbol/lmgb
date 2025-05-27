#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include "lmgb.h"
#include "mem.h"

#define SYNC_WITH_CPU(clockDelta)
#define btow(a, b) ((a << 8) | b)

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
    word sp, pc;
    lmgb::Memory mem;

    Cpu();
    // ~Cpu();

    byte readOp(word &pc);
    void Step();
  };
}

#endif