#ifndef LMGB_MBC_H
#define LMGB_MBC_H

#include <algorithm>
#include <vector>

#include <defs.h>
#include <enums.h>

namespace lmgb {

inline constexpr std::size_t romBankSize = 0x4000;
inline constexpr std::size_t ramBankSize = 0x2000;

class mbc {
public:
  mbc(ROM_SIZES rom_size, RAM_SIZES ram_size, std::vector<byte> &rom_data)
      : rom(std::move(rom_data)) {
    switch (rom_size) {
    case ROM_KIB_32:  romSize = 2; break;
    case ROM_KIB_64:  romSize = 4; break;
    case ROM_KIB_128: romSize = 8; break;
    case ROM_KIB_256: romSize = 16; break;
    case ROM_KIB_512: romSize = 32; break;
    default:          romSize = std::max<std::size_t>(1, rom.size() / romBankSize); break;
    }

    switch (ram_size) {
    case RAM_KIB_8:   ramSize = 1; break;
    case RAM_KIB_32:  ramSize = 4; break;
    case RAM_KIB_64:  ramSize = 8; break;
    case RAM_KIB_128: ramSize = 16; break;
    case RAM_NO_RAM:
    case RAM_UNUSED:
    default:          ramSize = 0; break;
    }

    if (ramSize > 0) {
      ram.resize(ramSize * ramBankSize, 0xff);
    }
  }

  virtual ~mbc() = default;

  virtual byte read(word addr) = 0;
  virtual void write(word addr, byte val) = 0;

protected:
  bool ramEnable = false;

  byte selectedRom = 1;
  byte selectedRam = 0;
  std::size_t romOffset = romBankSize;
  std::size_t ramOffset = 0;

  std::size_t romSize = 0; // number of banks
  std::size_t ramSize = 0; // number of banks

  std::vector<byte> rom;
  std::vector<byte> ram;

  byte safe_rom_read(std::size_t index) const {
    if (rom.empty()) return 0xff;
    return index < rom.size() ? rom[index] : 0xff;
  }

  byte safe_ram_read(std::size_t index) const {
    return index < ram.size() ? ram[index] : 0xff;
  }

  void safe_ram_write(std::size_t index, byte val) {
    if (index < ram.size()) ram[index] = val;
  }
};

} // namespace lmgb

#endif
