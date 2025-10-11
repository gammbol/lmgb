#ifndef LMGB_INTERRUPTS_H
#define LMGB_INTERRUPTS_H

#include "cpu.h"
#include "defs.h"

namespace lmgb {

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
  interrupts();

  void requestInterrupt(INT_TYPE type);
  void Step(lmgb::cpu &cpu);
};

} // namespace lmgb

#endif
