#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

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
}

#endif
