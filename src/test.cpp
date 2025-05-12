#include "cpu.h"
#include <iostream>

int main() {
    lmgb::Cpu *cpu = new lmgb::Cpu();
    cpu->bc.bytes.l = 30;

    lmgb::Memory *mem = new lmgb::Memory();
    mem->Write(0, 0x41);
    cpu->readOp(mem);
    std::cout << "(" << (int)mem.data[0] << ") "<< "b = " << (int)cpu->b << std::endl;

    mem.mem[1] = 0x06;
    mem.mem[2] = 120;
    cpu->readOp(mem);
    std::cout << "(" << (int)mem.data[1] << ") "<< "b = " << (int)cpu->b << std::endl;

    mem.data[3] = 0x46;
    mem.data[4] = 150;
    cpu->h = 0x0;
    cpu->l = 0x4;
    cpu->readOp(mem);
    std::cout << "(" << (int)mem.data[3] << ") "<< "b = " << (int)cpu->b << std::endl;

    std::cout << "Testing REG8T16" << std::endl;
    cpu->h = 0x1;
    cpu->l = 0x0;
    std::cout << "h = " << (int)cpu->h << " l = " << (int)cpu->l << std::endl;
    std::cout << "REG = " << (int)REG8T16(cpu->h, cpu->l) << std::endl;

}