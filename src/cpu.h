#ifndef LMGB_CPU_H
#define LBGB_CPU_H

#include <cstdint>

#define REG8T16(f, s) ((f << 8) | s)

typedef struct {
    uint8_t data[62526];
} Mem;

class Cpu {
    public:
    uint16_t pc, sp;
    uint8_t f, a, b, c, d, e, h, l;
    uint8_t ir, ie;

    // Cpu();
    // ~Cpu();

    uint8_t readMem(uint8_t addr, Mem ram);
    void writeMem(uint8_t addr, Mem ram, uint8_t val);
    void readOp(Mem ram);
};

#endif