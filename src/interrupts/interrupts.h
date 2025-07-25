#ifndef INTERRUPTS_H
#define INTERRUPTS_H

namespace lmgb {

enum INT_TYPE {
  VBLANK,
  LCDC,
  T_OVERFLOW,
  IO_COMPLETE,
  JOYPAD
}

class Interrupts {
private:
  byte inte, intf;

public:



#endif
