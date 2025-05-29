#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include "lmgb.h"
#include "mem.h"

#define SYNC_WITH_CPU(clockDelta)
#define btow(a, b) ((a << 8) | b)

#define HALF_CARRY_FLAG(a, b) (((a & 0x0f) + (b & 0x0f)) > 0x0f)
#define CARRY_FLAG(a, b) (((a & 0xff) + (b & 0xff)) > 0xff)

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
} // namespace lmgb

#endif