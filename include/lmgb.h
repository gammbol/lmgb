#ifndef LMGB_H
#define LMGB_H

#include <fstream>

// gameboy elements
#include "cpu.h"
#include "interrupts.h"
#include "timer.h"

namespace lmgb {
class gb {

  // i'm not sure why i even made these classes friend,
  // so i deleted it away
  lmgb::cpu cpu;
  lmgb::interrupts interrupts;
  lmgb::timer timer;

public:
  char game_title[16];
  MBC_TYPES mbc_type;
  ROM_SIZES rom_size;
  RAM_SIZES ram_size;

  gb(const char *path) {
    std::ifstream CartridgeHeaderFile;

    CartridgeHeaderFile.open(path);
    if (!CartridgeHeaderFile.is_open()) {
      throw std::invalid_argument("Unable to open file");
    }

    // reading game title
    CartridgeHeaderFile.seekg(0x0134, std::ifstream::beg);
    CartridgeHeaderFile.read(game_title, sizeof(game_title));

    // reading MBC type
    CartridgeHeaderFile.seekg(0x0147, std::ifstream::beg);
    CartridgeHeaderFile.read(reinterpret_cast<char *>(&mbc_type), 1);

    // reading ROM size
    CartridgeHeaderFile.seekg(0x0148, std::ifstream::beg);
    CartridgeHeaderFile.read(reinterpret_cast<char *>(&rom_size), 1);

    if (mbc_type != lmgb::MBC_TYPES::MBC1_RAM) {
      ram_size = lmgb::RAM_SIZES::RAM_NO_RAM;
    } else {
      CartridgeHeaderFile.seekg(0x0149, std::ifstream::beg);
      CartridgeHeaderFile.read(reinterpret_cast<char *>(&ram_size), 1);
    }
  }
};
} // namespace lmgb

#endif
