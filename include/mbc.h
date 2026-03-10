#ifndef LMGB_MBC_H
#define LMGB_MBC_H

#include <vector>

#include <defs.h>
#include <enums.h>

namespace lmgb {

const int romBankSize = 0x4000;
const int ramBankSize = 0x2000;

// TODO: handle no ram scenario
class mbc {
public:
  mbc(ROM_SIZES rom_size, RAM_SIZES ram_size, std::vector<byte> &rom_data)
      : rom(std::move(rom_data)) {
    // TODO: check default cases for ram/rom switch-case

    // calculating rom size
    switch (rom_size) {
    case ROM_KIB_32:
      romSize = 2;
      break;
    case ROM_KIB_64:
      romSize = 4;
      break;
    case ROM_KIB_128:
      romSize = 8;
      break;
    case ROM_KIB_256:
      romSize = 16;
      break;
    case ROM_KIB_512:
      romSize = 32;
      break;

    default:
      romSize = 2;
      break;
    }

    // calculating ram size
    switch (ram_size) {
    case RAM_NO_RAM:
    case RAM_UNUSED:
      ramSize = 0;
      break;
    case RAM_KIB_8:
      ramSize = 1;
      break;
    case RAM_KIB_32:
      ramSize = 4;
      break;
    case RAM_KIB_64:
      ramSize = 8;
      break;
    case RAM_KIB_128:
      ramSize = 16;
      break;

    default:
      ramSize = 0;
      break;
    }
  }
  virtual ~mbc() {}

  virtual byte read(word addr) = 0;
  virtual void write(word addr, byte val) = 0;

protected:
  bool ramEnable = false;

  byte selectedRom = 0;
  byte selectedRam = 0;
  word romOffset = 0;
  word ramOffset = 0;

  word romSize = 0; // number of banks
  word ramSize = 0; // number of banks

  std::vector<byte> rom;
  byte *ram = nullptr;

  virtual byte *loadRom(const char *path) = 0;
  virtual byte *loadRam(const char *path) = 0;

  virtual void saveRam(const char *path) = 0;
};

} // namespace lmgb

#endif
