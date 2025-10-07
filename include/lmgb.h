#ifndef LMGB_H
#define LMGB_H

// gameboy elements
#include "cpu.h"
#include "interrupts.h"
#include "timer.h"

namespace lmgb {
class gb {

  // i'm not sure why i even made these classes friend,
  // so i deleted it away
  lmgb::cpu cpu;
  lmgb::interrupts interrupts;
  lmgb::timer timer;
};
} // namespace lmgb

#endif
