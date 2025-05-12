#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include <cstdint>
#include "mem.h"

#define REG8T16(f, s) ((f << 8) | s)

#define SYNC_WITH_CPU(clockDelta)

namespace lmgb {
  union WordRegister {
    struct {
      uint8_t l;
      uint8_t h;
    } bytes;
    uint16_t word;
  };

  class Cpu {
  public:
    WordRegister af, bc, de, hl;

    // Cpu();
    // ~Cpu();

    void Step();

    uint8_t readMem(uint8_t addr, Mem ram);
    void readOp(Mem ram);
  };
}

#endif
