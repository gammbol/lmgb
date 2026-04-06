#ifndef LMGB_DEFS_H
#define LMGB_DEFS_H

#include <cstdint>

namespace lmgb {

#undef byte
#undef word

#define sbyte int8_t

typedef uint8_t byte;
typedef uint16_t word;

namespace palette {

const unsigned og_white = 0x00;
const unsigned og_lgray = 0x01;
const unsigned og_dgray = 0x02;
const unsigned og_black = 0x03;

}; // namespace palette

}; // namespace lmgb

#endif