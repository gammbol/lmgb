#ifndef LMGB_DEFS_H
#define LMGB_DEFS_H

#include <cstdint>

namespace lmgb {

#undef byte
#undef word

#define sbyte int8_t

typedef uint8_t byte;
typedef uint16_t word;

};


#endif