#include "cpu.h"

lmgb::Cpu::Cpu(lmgb::Memory mem) {
    mmu = mem;
}

void lmgb::Cpu::Step() {
    byte opcode = mmu.Read(0);
}