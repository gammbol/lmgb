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
  interrupt_flag = 0;
  interrupt_enable = 0;
}

byte interrupts::read(word addr) const {
  assert(
    addr == INTERRUPT_ENABLE_ADDRESS || 
    addr == INTERRUPT_FLAG_ADDRESS
  );

  switch(addr) {
  case INTERRUPT_ENABLE_ADDRESS: return interrupt_enable;
  case INTERRUPT_FLAG_ADDRESS: return interrupt_flag;
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
    interrupt_enable = val;
    break;

  case INTERRUPT_FLAG_ADDRESS:
    interrupt_flag = val;
    break;
  }
}

void interrupts::request_interrupt(INT_TYPE type) { interrupt_flag |= type; }

void interrupts::step(int steps, lmgb::cpu &cpu) {
  byte pending = (interrupt_flag & interrupt_enable) & 0x1f;

  if (!pending) return;

  if (cpu.state == HALTED) cpu.state = RUNNING;

  if (!cpu.ime) return;

  if (pending & VBLANK) {
    interrupt_flag &= ~VBLANK;
    cpu.pushWord(cpu.pc);
    cpu.pc = 0x40;
  } else if (pending & LCDC) {
    interrupt_flag &= ~LCDC;
    cpu.pushWord(cpu.pc);
    cpu.pc = 0x48;
  } else if (pending & T_OVERFLOW) {
    interrupt_flag &= ~T_OVERFLOW;
    cpu.pushWord(cpu.pc);
    cpu.pc = 0x50;
  } else if (pending & IO_COMPLETE) {
    interrupt_flag &= ~IO_COMPLETE;
    cpu.pushWord(cpu.pc);
    cpu.pc = 0x58;
  } else if (pending & JOYPAD) {
    interrupt_flag &= ~JOYPAD;
    cpu.pushWord(cpu.pc);
    cpu.pc = 0x60;
  }
}

}
