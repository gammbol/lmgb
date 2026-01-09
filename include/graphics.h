#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

#include <lmgb.h>
#include <mem.h>

#define LINES_PER_TILE 8
#define LINE_LEN 16
#define HF_LINE_LEN 8

namespace lmgb {

// PPU MODES
// MODE0 - Horizontal Blank
// MODE1 - Vertical Blank
// MODE2 - OAM Scan
// MODE3 - Drawind Pixels
enum PPUSTATE {
  MODE0,
  MODE1,
  MODE2,
  MODE3
};


enum LCDCONTROL {
  ENABLE = 0x80,
  WINDOW_TILEMAP = 0x40,
  WINDOW_ENABLE = 0x20,
  BGWIN_AREA = 0x10,
  BG_TILEMAP = 0x08,
  OBJ_SIZE = 0x04,
  OBJ_ENABLE = 0x02,
  BGWIN_PRIORITY = 0x01
};

enum ATTRS { POSY, POSX, TILEINDEX, FLAGS };

enum PALETTES {
  BGP,
  OBP0,
  OBP1
};

enum PALETTE_COLORS { 
  WHITE = 0x9bbc0f, 
  LGRAY = 0x8bac0f, 
  DGRAY = 0x306230, 
  BLACK = 0x0f380f,
  TRANSPARENT
};


// tile structure
// contains 16 bytes for the whole tile
typedef struct tileData {
  byte tileBytes[16];

  // getting color indices for a line
  // first line = 0
  word getLine(byte line) {
    if (line < 0 || line > LINES_PER_TILE - 1)
      return 0;

    word res = 0;
    for (int i = 0; i < HF_LINE_LEN; i++) {
      res |= (tileBytes[line * 2 + 1] & (1 << i)) << (i + 1);
      res |= (tileBytes[line * 2] & (1 << i)) << i;
    }
    return res;
  }
} tileData;

class sprite {
public:
  sprite() = default;
  sprite(byte posy, byte posx, byte tileindex, byte flags)
      : posY(posy), posX(posx), tileIndex(tileindex), flags(flags) {}

  byte getAttribute(ATTRS attr);
  void setAttribute(ATTRS attr, byte val);
private:
  byte posY;
  byte posX;
  byte tileIndex;
  byte flags;
};

// VRAM class
class graphics {
public:
  graphics(mem *memory) : memory(memory) {
    // BG palette init
    BGPalette[0] = WHITE;
    BGPalette[1] = LGRAY;
    BGPalette[2] = DGRAY;
    BGPalette[3] = BLACK;

    // OBJ palettes init
    for (int i = 0; i < 2; i++) {
      OBJPalettes[i][0] = TRANSPARENT;
      OBJPalettes[i][1] = LGRAY;
      OBJPalettes[i][2] = DGRAY;
      OBJPalettes[i][3] = BLACK;
    }
  }

  // palettes
  void setPalette(PALETTES plt, byte val);

  // OAM
  void DMATransfer(word addr);

  void step(int steps);

private:
  PPUSTATE state;

  mem *memory;

  // Background position
  byte SYC;
  byte SCX;

  // Window position
  byte WY;
  byte WX;

  PALETTE_COLORS BGPalette[4];
  PALETTE_COLORS OBJPalettes[2][4];
  
  byte LCDC; // LCD Control

  // LCD Status
  byte LY;
  byte LYC;
  byte STAT;

  // Tile Data Blocks
  tileData tileBlock0[128];
  tileData tileBlock1[128];
  tileData tileBlock2[128];

  // Tile Maps
  // made them 2d for the better accessing
  byte tileMap0[32][32];
  byte tileMap1[32][32];

  // OAM
  sprite oam[40];
};
} // namespace lmgb

#endif
