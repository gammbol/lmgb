#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

#include <lmgb.h>
#include <mem.h>

#define LINES_PER_TILE 8
#define LINE_LEN 16
#define HF_LINE_LEN 8

namespace lmgb {
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

enum PALETTE_COLORS { 
  WHITE = 0x9bbc0f, 
  LGRAY = 0x8bac0f, 
  DGRAY = 0x306230, 
  BLACK = 0x0f380f 
};

typedef struct palette {
  const PALETTE_COLORS id0 = WHITE;
  PALETTE_COLORS id1;
  PALETTE_COLORS id2;
  PALETTE_COLORS id3;
} palette;

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
  graphics(mem *memory) : memory(memory) {}

  // writing & reading tileData
  tileData readTileBlock(byte addr);
  void writeTileBlock(byte addr, tileData tile);

  // writing & reading tileMap
  byte readTileMap(word addr);
  void writeTileMap(word addr, byte id);

  // palette
  void setPalette(PALETTE_COLORS id1, PALETTE_COLORS id2,
                  PALETTE_COLORS id3); // id0 is always transparent

  // LCDC
  bool getLCDCParam(LCDCONTROL parameter);
  void setLCDCParam(LCDCONTROL parameter, bool value, bool reset);

  // OAM
  void DMATransfer(word addr);

  void Step();

private:
  mem *memory;

  palette palette;
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
  byte tileMap0[1024];
  byte tileMap1[1024];

  // OAM
  sprite oam[40];
};
} // namespace lmgb

#endif
