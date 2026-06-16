#ifndef LMGB_INTERRUPTS_H
#define LMGB_INTERRUPTS_H

#include <defs.h>

namespace lmgb {

class cpu;

inline constexpr word SERVICING_INTERRUPT_CYCLES = 5;

inline constexpr word INTERRUPT_ENABLE_ADDRESS = 0xffff;
inline constexpr word INTERRUPT_FLAG_ADDRESS = 0xff0f;

inline constexpr word INTERRUPT_VBLANK_ADDRESS = 0x0040;
inline constexpr word INTERRUPT_LCDC_ADDRESS = 0x0048;
inline constexpr word INTERRUPT_T_OVERFLOW_ADDRESS = 0x0050;
inline constexpr word INTERRUPT_IO_COMPLETE_ADDRESS = 0x0058;
inline constexpr word INTERRUPT_JOYPAD_ADDRESS = 0x0060;

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
  byte interrupt_enable_, interrupt_flag_;

public:
  byte read(word addr) const;
  void write(word addr, byte val);

  interrupts();

  void request_interrupt(INT_TYPE type);
  void service_interrupt(cpu& cpu, INT_TYPE type, word addr);

  bool step(cpu& cpu);
};

} // namespace lmgb

#endif
