#include <cstdint>

struct CPU {
    uint8_t A, B, C, D, E, H, L;   // registers
    uint16_t SP, PC;               // stack pointer & program counter
    uint8_t Z, N, H, C;            // flags registers

    void Reset() {
        A = B = C = D = E = H = L = 0;
        Z = N = H = C = 0;
    }
};