#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include "lmgb.h"
#include "mem.h"

#define SYNC_WITH_CPU(clockDelta)
#define btow(a, b) ((a << 8) | b)

#define Z_FLAG_SET(f) (f |= 0x01 << 7)
#define N_FLAG_SET(f) (f |= 0x01 << 6)
#define H_FLAG_SET(f) (f |= 0x01 << 5)
#define C_FLAG_SET(f) (f |= 0x01 << 4)

#define Z_FLAG_CHECK(a) (a == 0)
#define H_FLAG_CHECK(a, b) (((a & 0x0f) + (b & 0x0f)) > 0x0f)
#define C_FLAG_CHECK(a, b) (((a & 0xff) + (b & 0xff)) > 0xff)

#define Z_FLAG_RESET(f) (f &= 0x70)
#define N_FLAG_RESET(f) (f &= 0xb0)
#define H_FLAG_RESET(f) (f &= 0xd0)
#define C_FLAG_RESET(f) (f &= 0xe0)

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
