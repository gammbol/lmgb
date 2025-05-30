#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include "lmgb.h"
#include "mem.h"

#define SYNC_WITH_CPU(clockDelta)
#define btow(a, b) ((a << 8) | b)

#define ZF_CHECK(a) (a == 0)
#define HF_CHECK(a, b) (((a & 0x0f) + (b & 0x0f)) > 0x0f)
#define CF_CHECK(a, b) (((a & 0xff) + (b & 0xff)) > 0xff)

#define HSF_CHECK(a, b) ((a & 0x0f) < (b & 0x0f))
#define CSF_CHECK(a, b) (a < b)

#define Z_FLAG 0x80
#define N_FLAG 0x40
#define H_FLAG 0x20
#define C_FLAG 0x10

#define CF_GET(f) ((f & C_FLAG) >> 4)

#define ZF_SET(f) (f |= Z_FLAG)
#define NF_SET(f) (f |= N_FLAG)
#define HF_SET(f) (f |= H_FLAG)
#define CF_SET(f) (f |= C_FLAG)

#define ZF_RESET(f) (f &= ~Z_FLAG)
#define NF_RESET(f) (f &= ~N_FLAG)
#define HF_RESET(f) (f &= ~H_FLAG)
#define CF_RESET(f) (f &= ~C_FLAG)

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
