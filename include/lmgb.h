#ifndef LMGB_H
#define LMGB_H

// definitions
#include "defs.h"

// gameboy elements
#include "cpu.h"
#include "interrupts.h"
#include "timer.h"

namespace lmgb {
class gb {
  lmgb::cpu cpu;
  friend class lmgb::cpu;

  lmgb::interrupts interrupts;
  friend class lmgb::interrupts;

  lmgb::timer timer;
  friend class lmgb::timer;
};
} // namespace lmgb

#endif
