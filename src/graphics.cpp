#include <graphics.h>

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

// TileMap section
byte lmgb::graphics::readTileMap(word addr) {
  // TODO: check smth to choose the right tilemap
  return 0;
}

void lmgb::graphics::writeTileMap(word addr, byte id) {
  // TODO: check smth to choose the right tilemap
}

// Palette section
void lmgb::graphics::setPalette(lmgb::PALETTE id1, lmgb::PALETTE id2,
                                lmgb::PALETTE id3) {
  palette[1] = id1;
  palette[2] = id2;
  palette[3] = id3;
}

// LCDC section
bool lmgb::graphics::getLCDCParam(lmgb::LCDCONTROL parameter) {
  return LCDC & parameter;
}

void lmgb::graphics::setLCDCParam(lmgb::LCDCONTROL parameter, bool value,
                                  bool reset) {
  if (reset)
    LCDC = LCDC & ~parameter;
  else
    LCDC = LCDC | parameter;
}

void lmgb::graphics::DMATransfer(word addr) {
    word startAddr = addr & 0xff00;
    word endAddr = startAddr + 0x009f;

    if (((addr & 0xff00) >> 8) > 0xdf)
      return;

    for (int i = startAddr; i < endAddr; i += 4) {
      oam[i/4] = sprite();
      oam[i/4].setAttribute(POSY, memory->Read(i));
      oam[i/4].setAttribute(POSX, memory->Read(i + 1));
      oam[i/4].setAttribute(TILEINDEX, memory->Read(i + 2));
      oam[i/4].setAttribute(FLAGS, memory->Read(i + 3));
    }
  }