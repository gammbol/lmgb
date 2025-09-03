#include "interrupts.h"

/*
Interrupt			Priority	Start Address
V-Blank				1			$0040
LCDC Status			2			$0048 - Modes 0, 1, 2,
LYC=LY coincide (selectable) Timer Overflow		3
$0050 Serial Transfer		4			$0058 - when transfer is
complete Hi-Lo of P10-P13	5			$0060
*/

void lmgb::Interrupts::Step(lmgb::Cpu &cpu) {
  if ((intf & inte) & 0x1f) {
    
  }
}