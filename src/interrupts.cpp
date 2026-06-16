#include <interrupts.h>
#include <cassert>

#include <cpu.h>

/*
Interrupt			Priority	Start Address
V-Blank				1			$0040
LCDC Status			2			$0048 - Modes 0, 1, 2,
LYC=LY coincide (selectable) Timer Overflow		3
$0050 Serial Transfer		4			$0058 - when transfer is
complete Hi-Lo of P10-P13	5			$0060
*/

namespace lmgb {

interrupts::interrupts() {
  interrupt_flag_ = 0;
  interrupt_enable_ = 0;
}

byte interrupts::read(word addr) const {
  assert(
    addr == INTERRUPT_ENABLE_ADDRESS || 
    addr == INTERRUPT_FLAG_ADDRESS
  );

  switch(addr) {
  case INTERRUPT_ENABLE_ADDRESS: return interrupt_enable_;
  case INTERRUPT_FLAG_ADDRESS: return interrupt_flag_;
  default: return 0xff;
  }
}

void interrupts::write(word addr, byte val) {
  assert(
    addr == INTERRUPT_ENABLE_ADDRESS || 
    addr == INTERRUPT_FLAG_ADDRESS
  );

  switch(addr) {
  case INTERRUPT_ENABLE_ADDRESS: 
    interrupt_enable_ = val;
    break;

  case INTERRUPT_FLAG_ADDRESS:
    interrupt_flag_ = val;
    break;
  }
}

void interrupts::request_interrupt(INT_TYPE type) { interrupt_flag_ |= type; }

void interrupts::service_interrupt(cpu& cpu, INT_TYPE type, word addr) {
  interrupt_flag_ &= ~type;
  cpu.pushWord(cpu.pc);
  cpu.pc = addr;
}

bool interrupts::step(cpu &cpu) {
  byte pending = (interrupt_flag_ & interrupt_enable_) & 0x1f;

  if (!pending) return false;

  if (cpu.state == HALTED) cpu.state = RUNNING;

  if (!cpu.ime) return false;

  if (pending & VBLANK)
    service_interrupt(cpu, VBLANK, INTERRUPT_VBLANK_ADDRESS);
  else if (pending & LCDC)
    service_interrupt(cpu, LCDC, INTERRUPT_LCDC_ADDRESS);
  else if (pending & T_OVERFLOW)
    service_interrupt(cpu, T_OVERFLOW, INTERRUPT_T_OVERFLOW_ADDRESS);
  else if (pending & IO_COMPLETE)
    service_interrupt(cpu, IO_COMPLETE, INTERRUPT_IO_COMPLETE_ADDRESS);
  else if (pending & JOYPAD)
    service_interrupt(cpu, JOYPAD, INTERRUPT_JOYPAD_ADDRESS);

  return true;
}

}
