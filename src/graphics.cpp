#include <graphics.h>

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
    LCDC = LCDC & parameter;
  else
    LCDC = LCDC | parameter;
}