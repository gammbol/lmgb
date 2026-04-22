#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

#include <defs.h>

namespace lmgb {

// Palettes
class palettes {
  byte bgp_;
  byte obp_[2];

public:
  byte get_bgp();
  void set_bgp(byte val);

  byte get_obp(byte id);
  void set_obp(byte id, byte val);
};

// Window
class window {
public:
  byte wy;
  byte wx;
  byte y_cond;
};

// Background
class background {
public:
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
  oam_obj get_obj(byte id);
  void set_obj(byte id, oam_obj object);
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
  tileData get_block(byte id);
  void set_block(byte id, tileData block);
};

// Tile Maps
class tileMaps {
  byte maps_[2][1024];

public:
  byte get_tile(byte map, byte id);
  void set_tile(byte map, byte id, byte val);
};

// LCD
class lcd {
  // LCD Control
  byte lcdc_;

  // LCD Status Registers
  byte lcdy_;
  byte lyc_;
  byte stat_;

public:
  byte get_lcdc();
  void set_lcdc(byte val);

  byte get_lcdy();

  byte get_lyc();
  void set_lyc(byte val);

  byte get_stat();
  void set_stat(byte val);
  
};

// Pixel Process Unit
class ppu {
  lcd lcd_control{};
  tileMaps tile_maps{};
  tileBlocks tile_blocks{};
  oam object_mem{};
  window wnd{};
  background bg{};

public:
  byte read(word addr);
  void write(word addr, byte val);

  void step(int ticks);
};

} // namespace lmgb

#endif