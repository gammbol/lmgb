#ifndef LMGB_TIMER_H
#define LMGB_TIMER_H

#include "defs.h"
#include "interrupts.h"

#define isOverflow(a) ((int)(a + 1) > 0xffff)

namespace lmgb {
// about timer control
// idk how to implement it right
// at first i wanted to use enum, but smth has gone wrong
// so for now i will be returning just a byte

// 00 - 256M cycles
// 01 - 4M cycles
// 10 - 16M cycles
// 11 - 64M cycles

class timer {
  word cycles_;
  byte div_, tima_, tma_, tac_;

public:
  timer();

  short getCS() const;
  bool isTacEnabled() const;

  byte read(word addr) const;
  void write(word addr, byte value);

  void step(word c, interrupts& interrupt);
};
} // namespace lmgb

#endif