#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

#include <defs.h>

namespace lmgb {

// PPU Mode Enum
enum PPU_MODE {
  HBLANK,
  VBLANK,
  SCANNING,
  DRAWING
};

// Palettes
struct palettes {
  byte bgp;
  byte obp[2];
};

// Window
struct window {
  byte wy;
  byte wx;
  byte y_cond;
};

// Background
struct background {
  byte scy;
  byte scx;
};

// OAM Object
class oam_obj {
  byte posy_;
  byte posx_;
  byte tileIndex_;
  byte attrs_;

public:
  byte get_posy();
  
  byte get_posx();

  byte get_tileIndex();

  byte get_attrs();
};

// Object Attribute Memory
class oam {
  oam_obj objects_[40];

public:
  byte dma_src;
  byte dma_cur;
  bool isTransferring;

  byte read(word addr);
  void write(word addr, byte val);
};

// Tile Data Format
class tileData {
  byte data_[16];

public:
  byte get_byte(byte line, bool fst);
  void set_byte(byte line, bool fst, byte val);
};

// Tile Blocks
class tileBlocks {
  tileData blocks_[3][128];

public:
  byte read(word addr);
  void write(word addr, byte val);

  byte read_by_id(byte id, byte block);
};

// Tile Maps
class tileMaps {
  byte maps_[2][1024];

public:
  byte read(word addr);
  void write(word addr, byte val);

  byte get_tile(byte map, byte x, byte y);
  void set_tile(byte map, byte id, byte val);
};

// LCD
class lcd {
  // LCD Y coordinate
  byte ly_;

  // Ticks Spent on DRAWING mode
  unsigned short last_drawing_ticks_;
  unsigned short current_ticks_;

  // making Pixel Processing Unit
  // be able to use LY register
  friend ppu;

public:
  // LCD Control
  byte lcdc;

  // LCD Status Registers
  byte lyc;
  byte stat;

  byte get_ly();

  PPU_MODE get_mode();
  void set_mode(PPU_MODE mode);
  
};

// Pixel Process Unit
class ppu {
  lcd lcd_control{};
  tileMaps tile_maps{};
  tileBlocks tile_blocks{};
  oam object_mem{};
  window wnd{};
  background bg{};
  palettes plt{};

  void switchMode();

  unsigned scanLine[160]{};

public:
  byte read(word addr);
  void write(word addr, byte val);

  void step(int ticks);
};

} // namespace lmgb

#endif