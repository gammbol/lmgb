#include <mem.h>

namespace lmgb {

mem::mem(MBC_TYPES mbc_type, ROM_SIZES rom_size,
        RAM_SIZES ram_size, std::vector<byte> &rom_data) {
  switch (mbc_type) {
  default:
  case ROM_ONLY:
    memory_controller_ = new nombc(rom_size, ram_size, rom_data);
    break;
  case MBC1:
  case MBC1_RAM:
    memory_controller_ = new mbc1(rom_size, ram_size, rom_data);
    break;
  }
}

mem::~mem() {
  delete memory_controller_;
}

byte mem::Read(word addr) {
  switch (addr & 0xf000) {
  case 0x0000:
  case 0x1000:
  case 0x2000:
  case 0x3000:
  case 0x4000:
  case 0xa000:
  case 0xb000:
    return memory_controller_->read(addr);
  
  case 0xf000: {
    if (
      addr == INTERRUPT_ENABLE_ADDRESS ||
      addr == INTERRUPT_FLAG_ADDRESS
    ) interrupt_handler_.read(addr);
  }

  default:
    return -1;
  }
}

void mem::Write(word addr, byte val) {
  switch (addr & 0xf000) {
  case 0x0000:
  case 0x1000:
  case 0x2000:
  case 0x3000:
  case 0x4000:
  case 0x5000:
  case 0x6000:
  case 0x7000:
  case 0xa000:
  case 0xb000:
    memory_controller_->write(addr, val);
    break;

  case 0x8000:
  case 0x9000:
    // vram
    break;

  default:
    return;
  }
}

}
