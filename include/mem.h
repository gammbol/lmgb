#ifndef LMGB_MEM_H
#define LMGB_MEM_H


// include all the mbcs here for now
#include <mbc.h>
#include <mbc1.h>
#include <mbc_nombc.h>

#include <array>
#include <defs.h>
#include <interrupts.h>
#include <graphics.h>
#include <timer.h>
// #define MEM_LEN 0x10000

namespace lmgb {

class mem {
  mbc *memory_controller_;
  interrupts interrupt_handler_;
  ppu& pixel_processing_unit_;

  timer timer_{};


  // TODO: finish oam dma
  void dma_transfer();

public:
  mem(MBC_TYPES mbc_type, ROM_SIZES rom_size, RAM_SIZES ram_size,
      std::vector<byte> &rom_data, ppu& ppu);
  ~mem();

  std::array<byte, 0x2000> wram_{};
  std::array<byte, 0x7f> hram_{};

  byte Read(word addr);
  void Write(word addr, byte val);
};
} // namespace lmgb

#endif
