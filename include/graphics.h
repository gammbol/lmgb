#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

#include <lmgb.h>

#define LINE_LEN 16
#define HF_LINE_LEN 8

namespace lmgb {
  typedef struct tileData {
    byte lsb, msb;

    word getLine() {
      word res = 0;
      for (int i = 0; i < HF_LINE_LEN; i++) {
        res |= (msb & (1 << i)) << (i + 1);
        res |= (lsb & (1 << i)) << i;
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

  private:
    // Tile Data Blocks
    tileData tileBlock0[384];
    tileData tileBlock1[384];

    // Tile Maps
    byte tileMap0[1024];
    byte tileMap1[1024];

    // OAM
    objAttr oam[40];
  };
}

#endif
