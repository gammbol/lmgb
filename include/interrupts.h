#ifndef LMGB_INTERRUPTS_H
#define LMGB_INTERRUPTS_H

#include "cpu.h"
#include "lmgb.h"

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

class Interrupts {
private:
  byte inte, intf;

public:
  void Step(lmgb::Cpu &cpu);
};

} // namespace lmgb

#endif
