#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include "lmgb.h"
#include "mem.h"

#define REG8T16(f, s) ((f << 8) | s)

#define SYNC_WITH_CPU(clockDelta)

namespace lmgb {
  union WordRegister {
    struct {
      byte l;
      byte h;
    } bytes;
    word word;
  };

  class Cpu {
  public:
    WordRegister af, bc, de, hl;


    lmgb::Memory &mmu;

    Cpu(lmgb::Memory mem);
    // ~Cpu();

    void Step();

    uint8_t readMem(uint8_t addr, lmgb::Memory ram);
    void readOp(lmgb::Memory ram);
  };
}

#endif