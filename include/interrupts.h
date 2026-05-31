#ifndef LMGB_INTERRUPTS_H
#define LMGB_INTERRUPTS_H

#include "cpu.h"
#include "defs.h"

namespace lmgb {

inline constexpr word INTERRUPT_ENABLE_ADDRESS = 0xffff;
inline constexpr word INTERRUPT_FLAG_ADDRESS = 0xffef;

// class Cpu;
// class Memory;

enum INT_TYPE {
  VBLANK = 0x01,
  LCDC = 0x02,
  T_OVERFLOW = 0x04,
  IO_COMPLETE = 0x08,
  JOYPAD = 0x10
};

class interrupts {
private:
  byte inte, intf;

public:
  byte read(word addr) const;
  void write(word addr, byte val);

  interrupts();

  void request_interrupt(INT_TYPE type);
  void step(int steps, lmgb::cpu &cpu);
};

} // namespace lmgb

#endif
