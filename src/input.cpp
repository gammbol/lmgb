#include "input.h"

bool lmgb::input::isButtonPressed() const { return buttons & 0x0f; }