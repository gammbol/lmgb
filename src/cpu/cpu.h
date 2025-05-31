#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include "lmgb.h"
#include "mem.h"

#define SYNC_WITH_CPU(clockDelta)
#define btow(a, b) ((a << 8) | b)

#define ZF_CHECK(a) (a == 0)
#define HF_CHECK(a, b) (((a & 0x0f) + (b & 0x0f)) > 0x0f)
#define CF_CHECK(a, b) (((a & 0xff) + (b & 0xff)) > 0xff)

#define CF_GET(f) ((f & 0x10) >> 4)

#define ZF_SET(f) (f |= 0x01 << 7)
#define NF_SET(f) (f |= 0x01 << 6)
#define HF_SET(f) (f |= 0x01 << 5)
#define CF_SET(f) (f |= 0x01 << 4)

#define ZF_RESET(f) (f &= 0x70)
#define NF_RESET(f) (f &= 0xb0)
#define HF_RESET(f) (f &= 0xd0)
#define CF_RESET(f) (f &= 0xe0)

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
