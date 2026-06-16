#include <interrupts.h>
#include <timer.h>

lmgb::timer::timer() {
  cycles_ = 0;
  div_ = 0;
  tima_ = 0;
  tma_ = 0;
  tac_ = 0;
}

short lmgb::timer::getCS() const {
  switch (tac_ & 0b11) {
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

bool lmgb::timer::isTacEnabled() const { return (tac_ & 0x04) >> 2; }

lmgb::byte lmgb::timer::read(const word addr) const {
  switch (addr) {
  case 0xff04: return div_;
  case 0xff06: return tma_;
  case 0xff07: return tac_;
  }
}

void lmgb::timer::write(const word addr, const byte value) {
  switch (addr & 0x00ff) {
  case 0x04:
    div_ = 0;
    break;
  case 0x06:
    tma_ = value;
    break;
  case 0x07:
    tac_ = value & 0x03;
  default:;
  }
}

void lmgb::timer::step(const word c, interrupts& interrupt) {
  cycles_ += c;

  // DIV
  // if (cycles % 64 != 0)
  //   div++;
  div_ = cycles_ % 64;

  // TIMA
  if (isTacEnabled()) {
    if (isOverflow(tima_)) {
      tima_ = tma_;
      interrupt.request_interrupt(T_OVERFLOW);
      return;
    }
    tima_ = cycles_ % getCS();
  }
  // tima = isOverflow(tima) ? tma : cycles % getCS();
}