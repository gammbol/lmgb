#include "cpu.h"

uint8_t Cpu::readMem(uint8_t addr, Mem ram) {
    return ram.data[addr];
}

void Cpu::writeMem(uint8_t addr, Mem ram, uint8_t val) {
    ram.data[addr] = val;
}

void Cpu::readOp(Mem ram) {
    ir = ram.data[pc];
    pc += 1;

    switch(ir) {
        case 0x41: b = c; break;
        case 0x06:
            b = readMem(pc, ram);
            pc++;
            break;
        case 0x46:
            b = readMem(REG8T16(h, l), ram);
            pc++;
            break;
        case 0x70:
            writeMem(REG8T16(h, l), ram, b);
    }
    
}