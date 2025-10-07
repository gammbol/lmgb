#include "timer.h"
#include "interrupts.h"

short lmgb::timer::getCS() {
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

bool lmgb::timer::isTacEnabled() { return (tac & 0x04) >> 2; }

void lmgb::timer::write(word addr) {
  switch (addr) {
  case 0xff04:
    div = 0;
    break;
  }
}

void lmgb::timer::Step(word c, interrupts interrupt) {
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