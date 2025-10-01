//
// Created by gammb on 9/26/2025.
//

#ifndef LMGB_INPUT_H
#define LMGB_INPUT_H

#include "defs.h"

namespace lmgb {

enum LMGB_INPUT_BUTTONS {
  RIGHT_KEY,
  LEFT_KEY,
  UP_KEY,
  DOWN_KEY,
  A_BTN,
  B_BTN,
  SELECT_BTN,
  START_BTN
};

class input {
private:
  bool selectButtons;
  bool selectDPad;

  // the thing i want to try to implement is creating an 8-bit variable for the
  // buttons, so that i won't be creating any arrays
  // so the map is
  //
  // lower 4 bits are for the direction keys
  // 0 = RIGHT KEY
  // 1 = LEFT KEY
  // 2 = UP KEY
  // 3 = DOWN KEY
  //
  // upper 4 bits are for the buttons
  // 4 = A BUTTON
  // 5 = B BUTTON
  // 6 = SELECT BUTTON
  // 7 = START BUTTON
  byte buttons;

public:
  input() : selectButtons(false), selectDPad(false), buttons(0) {}
  bool isButtonPressed() const;
};
} // namespace lmgb

#endif // LMGB_INPUT_H
