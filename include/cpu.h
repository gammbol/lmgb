#ifndef LMGB_CPU_H
#define LMGB_CPU_H

#include <defs.h>
#include <enums.h>
#include <interrupts.h>
#include <mem.h>

inline constexpr unsigned CYCLES_WHILE_HALTED = 4;

#define Z_FLAG 0x80
#define N_FLAG 0x40
#define H_FLAG 0x20
#define C_FLAG 0x10

#define ZF_GET(f) (((f) & Z_FLAG) >> 7)
#define NF_GET(f) (((f) & N_FLAG) >> 6)
#define HF_GET(f) (((f) & H_FLAG) >> 5)
#define CF_GET(f) (((f) & C_FLAG) >> 4)

namespace lmgb {

union WordRegister {
  struct {
    byte l;
    byte h;
  } bytes;
  word pair;
};

enum CpuState { RUNNING, HALTED, STOPPED };

class cpu {
public:
  WordRegister af{}, bc{}, de{}, hl{};
  word sp{};
  word pc{};

  lmgb::mem& memory_;
  interrupts& interrupt_handler_;

  CpuState state{};
  bool ime{};

  cpu(lmgb::mem& memory, interrupts& interrupt_handler);

  void pushByte(byte val);
  void pushWord(word val);

  byte popByte();
  word popWord();

  byte fetch_u8();
  word fetch_u16();

  byte readOp(word &pc);
  unsigned step();
};
} // namespace lmgb

#endif
