#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include <iostream>

#include "lmgb.h"
#include "mem.h"

#define SYNC_WITH_CPU(clockDelta)

#define getbatpos(a, pos) ((a & (a << pos)) >> pos)
#define setbatpos(a, pos) (a = a | (1 << pos))
#define resetbatpos(a, pos) (a = a & ~(0 << pos))

#define getmsb(a) ((a & 0xff00) >> 7)
#define getlsb(a) (a & 0x00ff)

#define btow(a, b) ((a << 8) | b)

#define SWAP(a) (a = ((a & 0x0f) << 4) | ((a & 0xf0) >> 4))

#define ZF_CHECK(a) (a == 0)
#define HF_CHECK(a, b) (((a & 0x0f) + (b & 0x0f)) > 0x0f)
#define CF_CHECK(a, b) (((a & 0xff) + (b & 0xff)) > 0xff)

#define HSF_CHECK(a, b) ((a & 0x0f) < (b & 0x0f))
#define CSF_CHECK(a, b) (a < b)

#define HF_CHECK16(a, b) (((a & 0x00ff) + (b & 0x00ff)) > 0x00ff)
#define CF_CHECK16(a, b) (((a & 0xffff) + (b & 0xffff)) > 0xffff)

#define Z_FLAG 0x80
#define N_FLAG 0x40
#define H_FLAG 0x20
#define C_FLAG 0x10

#define ZF_GET(f) ((f & Z_FLAG) >> 7)
#define NF_GET(f) ((f & N_FLAG) >> 6)
#define HF_GET(f) ((f & H_FLAG) >> 5)
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

enum CpuState { RUNNING, HALTED, STOPPED };

class Cpu {
private:
  void getBit(const byte reg, int pos);

public:
  WordRegister af, bc, de, hl;
  word sp, pc;
  lmgb::Memory mem;

  CpuState state;
  bool ime;

  Cpu();
  // ~Cpu();

  void pushByte(byte val);
  void pushWord(word val);

  byte readOp(word &pc);
  void Step();
};
} // namespace lmgb

#endif
