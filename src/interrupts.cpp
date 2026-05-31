#include <interrupts.h>
#include <cassert>

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
  intf = 0;
  inte = 0;
}

byte interrupts::read(word addr) const {
  assert(
    addr == INTERRUPT_ENABLE_ADDRESS || 
    addr == INTERRUPT_FLAG_ADDRESS
  );

  switch(addr) {
  case INTERRUPT_ENABLE_ADDRESS: return inte;
  case INTERRUPT_FLAG_ADDRESS: return intf;
  }
}

void interrupts::write(word addr, byte val) {
  assert(
    addr == INTERRUPT_ENABLE_ADDRESS || 
    addr == INTERRUPT_FLAG_ADDRESS
  );

  switch(addr) {
  case INTERRUPT_ENABLE_ADDRESS: 
    inte = val;
    break;

  case INTERRUPT_FLAG_ADDRESS:
    intf = val;
    break;
  }
}

void interrupts::request_interrupt(INT_TYPE type) { intf |= type; }

void interrupts::step(int steps, lmgb::cpu &cpu) {
  byte pending = (intf & inte) & 0x1f;
  if (pending) {
    if (cpu.ime) {
      cpu.ime = false;
      if (intf & VBLANK) {
        intf &= ~VBLANK;
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x40;
      }
      else if (pending & LCDC) {
        intf &= ~LCDC;
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x48;
      }
      else if (pending & T_OVERFLOW) {
        intf &= ~T_OVERFLOW;
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x50;
      }
      else if (pending & IO_COMPLETE) {
        intf &= ~IO_COMPLETE;
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x58;
      }
      else if (pending & JOYPAD) {
        intf &= ~JOYPAD;
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x60;
      }
    }
  }
}

}
