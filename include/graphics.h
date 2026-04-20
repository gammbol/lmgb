#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

#include <defs.h>

namespace lmgb {

// --------- COLORS ---------
namespace pcolors {

const unsigned transparent = 0x000000;
const unsigned lgray = 0x000001;
const unsigned dgray = 0x000002;
const unsigned black = 0x000003;

} // namespace pcolors

const unsigned short MAX_DOTS = 456;

// PPU Modes
enum ppu_modes {
  HBLANK,
  VBLANK,
  SCANNING,
  DRAWING
};

class tiles {
  byte bytes[16]{};

public:
  // byte operations
  void setByte(byte bt, byte val);
  byte getByte(byte bt);

  word composeLine(byte line);
};

struct oam_obj {
  byte posy{};
  byte posx{};
  byte attributes;
};

class oam_block {
  oam_obj oam[40]{};

public:
  oam_obj get(byte id) { return oam[id%40]; }
};

class graphics {
  // mode
  ppu_modes mode{};

  // ticks
  unsigned current_ticks{};
  unsigned last_drawing_ticks{};

  // gb palette data
  byte bgp{};

  // obj palette 0,1 data
  byte obp[2]{};

  // tile data
  tiles tileBlock[3][128]{};

  // tileMap
  byte tileMap[2][1024]{};

  // oam dma transfer
  byte dma_src{}, dma_cur{};
  bool isDmaTransfer{false}; 

  // object attribute memory
  oam_block oam{};

  // LCD control
  byte lcdc{};

  // LCD Status Registers
  byte ly{}, lyc{}, stat{};

  // LCD scrolling
  byte scy{}, scx{};

  // window position
  byte wy{}, wx{};

  void switchMode();

  // framebuffer
  unsigned framebuffer[160][144]{ transparent };

public:
  void Step(int steps);

  byte read(word addr);
  void write(word addr, byte val);

  bool dmaTransferStatus() { return isDmaTransfer; };
};

} // namespace lmgb

#endif