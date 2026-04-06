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

class tiles {
  byte bytes[16]{};

public:
  // byte operations
  void setByte(byte line, bool isFirst);
  byte getByte(byte line, bool isFirst);

  word composeLine(byte line);
};

class graphics {
  // gb palette data
  byte bgp[4]{};

  // obj palette 0,1 data
  byte objbgp[2][4]{};

  // tile data
  tiles tileBlock[3][128]{};

  // tileMap
  byte tileMap[2][1024]{};

  // object attribute memory
  byte oam[40][4]{};

  // LCD control
  byte lcdc{};

  // LCD Status Registers
  byte ly{}, lyc{}, stat{};

  // LCD scrolling
  byte scy{}, scx{};

  // window position
  byte wy{}, wx{};

public:
  void Step();

  byte read(word addr);
  void write(word addr, byte val);
};

} // namespace lmgb

#endif