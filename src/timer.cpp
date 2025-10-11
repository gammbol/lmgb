#include "timer.h"
#include "interrupts.h"

lmgb::timer::timer() {
  cycles = 0;
  div = 0;
  tima = 0;
  tma = 0;
  tac = 0;
}

short lmgb::timer::getCS() const {
  switch (tac & 0b11) {
  case 0b00:
    return 256;
  case 0b01:
    return 4;
  case 0b10:
    return 16;
  case 0b11:
    return 64;

  default:
    return 0;
  }
}

bool lmgb::timer::isTacEnabled() const { return (tac & 0x04) >> 2; }

void lmgb::timer::write(const word addr, const byte value) {
  switch (addr & 0x00ff) {
  case 0x04:
    div = 0;
    break;
  case 0x06:
    tma = value;
    break;
  case 0x07:
    tac = value & 0x03;
  default:;
  }
}

void lmgb::timer::Step(const word c, interrupts interrupt) {
  cycles += c;

  // DIV
  // if (cycles % 64 != 0)
  //   div++;
  div = cycles % 64;

  // TIMA
  if (isTacEnabled()) {
    if (isOverflow(tima)) {
      tima = tma;
      interrupt.requestInterrupt(T_OVERFLOW);
      return;
    }
    tima = cycles % getCS();
  }
  // tima = isOverflow(tima) ? tma : cycles % getCS();
}