#include <graphics.h>

void lmgb::graphics::setPalette(PALETTES plt, byte val) {
  int offset = 0;
  for (int i = 0x03; i <= 0xc0; i *= 4) {
    byte id = (val & i) >> offset;
    if (plt == BGP) {
      switch (id) {
      case 0:
        BGPalette[offset] = WHITE;
        break;
      case 1:
        BGPalette[offset] = LGRAY;
        break;
      case 2:
        BGPalette[offset] = DGRAY;
        break;
      case 3:
        BGPalette[offset] = BLACK;
        break;
      }
    } else if (plt == OBP0) {
      if (i == 0x03) {
        OBJPalettes[0][0] = TRANSPARENT;
        continue;
      }

      switch (id) {
      case 0:
        OBJPalettes[0][offset] = TRANSPARENT;
        break;
      case 1:
        OBJPalettes[0][offset] = LGRAY;
        break;
      case 2:
        OBJPalettes[0][offset] = DGRAY;
        break;
      case 3:
        OBJPalettes[0][offset] = BLACK;
        break;
      }
    } else if (plt == OBP1) {
      if (i == 0x03) {
        OBJPalettes[1][0] = TRANSPARENT;
        continue;
      }

      switch (id) {
      case 0:
        OBJPalettes[1][offset] = TRANSPARENT;
        break;
      case 1:
        OBJPalettes[1][offset] = LGRAY;
        break;
      case 2:
        OBJPalettes[1][offset] = DGRAY;
        break;
      case 3:
        OBJPalettes[1][offset] = BLACK;
        break;
      }
    }

    offset++;
  }
}

// get sprite attribute
byte lmgb::sprite::getAttribute(lmgb::ATTRS attr) {
  switch (attr) {
  case POSY:
    return posY;
  case POSX:
    return posX;
  case TILEINDEX:
    return tileIndex;
  case FLAGS:
    return flags;
  }
}

void lmgb::sprite::setAttribute(lmgb::ATTRS attr, byte val) {
  switch (attr) {
  case POSY:
    posY = val;
    break;
  case POSX:
    posX = val;
    break;
  case TILEINDEX:
    tileIndex = val;
    break;
  case FLAGS:
    flags = val;
    break;
  }
}