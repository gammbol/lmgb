#ifndef LMGB_MEM_H
#define LMGB_MEM_H

#include <array>
#include <defs.h>
#include <graphics.h>
#include <interrupts.h>
#include <mbc.h>
#include <mbc1.h>
#include <mbc_nombc.h>
#include <timer.h>

namespace lmgb {

class mem {
  mbc *memory_controller_{};
  ppu& pixel_processing_unit_;
  interrupts& interrupt_handler_;
  timer& timer_;

  int pending_cycles_ = 0;

  void dma_transfer(byte high_addr);

public:
  mem(MBC_TYPES mbc_type, ROM_SIZES rom_size, RAM_SIZES ram_size,
      std::vector<byte> &rom_data, ppu& ppu, interrupts& interrupt_handler,
      timer& timer_ref);
  ~mem();

  mem(const mem&) = delete;
  mem& operator=(const mem&) = delete;

  std::array<byte, 0x2000> wram_{};
  std::array<byte, 0x7f> hram_{};

  int consume_pending_cycles();

  byte Read(word addr);
  void Write(word addr, byte val);
};
} // namespace lmgb

#endif
