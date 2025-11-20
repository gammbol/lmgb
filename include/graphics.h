#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

#include <lmgb.h>

#define LINES_PER_TILE 8
#define LINE_LEN 16
#define HF_LINE_LEN 8

namespace lmgb {
enum PALETTE { WHITE, LGRAY, DGRAY, BLACK };

typedef struct tileData {
  byte tileBytes[16];

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

typedef struct objData {
  byte posY;
  byte posX;
  byte tileIndex;
  byte Flags;
} objAttr;

// VRAM class
class graphics {
public:
  graphics() = default;

  // writing & reading tileData
  tileData readTileBlock(byte addr);
  void writeTileBlock(byte addr, tileData tile);

  // writing & reading tileMap
  byte readTileMap(word id);
  byte writeTileMap(word addr, byte id);

  // palette
  byte getPalette();
  byte setPalette(PALETTE id0, PALETTE id1, PALETTE id2, PALETTE id3);

private:
  byte palette;

  // Tile Data Blocks
  tileData tileBlock0[128];
  tileData tileBlock1[128];
  tileData tileBlock2[128];

  // Tile Maps
  byte tileMap0[1024];
  byte tileMap1[1024];

  // OAM
  objAttr oam[40];
};
} // namespace lmgb

#endif
