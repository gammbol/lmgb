#include "input.h"

bool lmgb::input::isButtonPressed() const { return buttons & 0x0f; }

void lmgb::input::setButtonPressed(LMGB_INPUT_BUTTONS input) {
  buttons |= input;
}
