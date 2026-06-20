#include <timer.h>

namespace lmgb {

unsigned timer::timer_period() const {
  switch (tac_ & 0x03) {
  case 0x00: return 1024; // 4096 Hz
  case 0x01: return 16;   // 262144 Hz
  case 0x02: return 64;   // 65536 Hz
  case 0x03: return 256;  // 16384 Hz
  default: return 1024;
  }
}

bool timer::isTacEnabled() const { return (tac_ & 0x04) != 0; }

byte timer::read(word addr) const {
  switch (addr) {
  case 0xff04: return div_;
  case 0xff05: return tima_;
  case 0xff06: return tma_;
  case 0xff07: return static_cast<byte>(tac_ | 0xf8);
  default: return 0xff;
  }
}

void timer::write(word addr, byte value) {
  switch (addr) {
  case 0xff04:
    div_ = 0;
    div_counter_ = 0;
    break;
  case 0xff05:
    tima_ = value;
    break;
  case 0xff06:
    tma_ = value;
    break;
  case 0xff07:
    tac_ = static_cast<byte>(value & 0x07);
    break;
  default:
    break;
  }
}

void timer::step(unsigned cycles, interrupts& interrupt) {
  div_counter_ += cycles;
  while (div_counter_ >= 256) {
    div_counter_ -= 256;
    ++div_;
  }

  if (!isTacEnabled()) {
    return;
  }

  tima_counter_ += cycles;
  const unsigned period = timer_period();
  while (tima_counter_ >= period) {
    tima_counter_ -= period;
    if (tima_ == 0xff) {
      tima_ = tma_;
      interrupt.request_interrupt(T_OVERFLOW);
    } else {
      ++tima_;
    }
  }
}

} // namespace lmgb
