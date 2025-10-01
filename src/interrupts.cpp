#include "interrupts.h"

/*
Interrupt			Priority	Start Address
V-Blank				1			$0040
LCDC Status			2			$0048 - Modes 0, 1, 2,
LYC=LY coincide (selectable) Timer Overflow		3
$0050 Serial Transfer		4			$0058 - when transfer is
complete Hi-Lo of P10-P13	5			$0060
*/

void lmgb::interrupts::requestInterrupt(INT_TYPE type) { intf |= type; }

void lmgb::interrupts::Step(lmgb::cpu &cpu) {
  if ((intf & inte) & 0x1f) {
    if (cpu.ime) {
      cpu.ime = false;
      if (intf & VBLANK) {
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x40;
      }
      if (intf & LCDC) {
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x48;
      }
      if (intf & T_OVERFLOW) {
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x50;
      }
      if (intf & IO_COMPLETE) {
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x58;
      }
      if (intf & JOYPAD) {
        cpu.pushWord(cpu.pc);
        cpu.pc = 0x60;
      }
    }
  }
}
