#ifndef LMGB_TIMER_H
#define LMGB_TIMER_H

#include "defs.h"
#include "interrupts.h"

namespace lmgb {

class timer {
  unsigned div_counter_{};
  unsigned tima_counter_{};
  byte div_{};
  byte tima_{};
  byte tma_{};
  byte tac_{};

public:
  timer() = default;

  unsigned timer_period() const;
  bool isTacEnabled() const;

  byte read(word addr) const;
  void write(word addr, byte value);

  void step(unsigned cycles, interrupts& interrupt);
};
} // namespace lmgb

#endif
