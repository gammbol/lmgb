#ifndef MEM_H
#define MEM_H

namespace lmgb {
  class Memory {
    uint8_t mem[0x10000];
    public:
      void Write(uint16_t addr, uint8_t val);
      uint8_t Read(uint16_t addr);
  };
}

#endif
