#ifndef LMGB_H
#define LMGB_H

#include <fstream>
#include <optional>
#include <vector>

#include <enums.h>

// gameboy elements
#include <cpu.h>
#include <graphics.h>
#include <interrupts.h>
#include <renderer.h>

namespace lmgb {
char const  *vertex_path = "shaders/vertex.vs";
char const  *fragment_path = "shaders/fragment.fs";

class gb {
  char game_title[16];
  MBC_TYPES mbc_type;
  ROM_SIZES rom_size;
  RAM_SIZES ram_size;

  std::vector<lmgb::byte> rom_data;

  cpu cpu_;
  interrupts interrupt_handler_{};
  mem memory_;
  ppu pixel_processing_unit_{interrupt_handler_};
  renderer *renderer_;

public:
  gb(const char *path);
  ~gb();

  void Step();
};

} // namespace lmgb

#endif
