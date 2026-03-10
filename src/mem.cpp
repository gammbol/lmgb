#include <mem.h>

lmgb::mem::mem(lmgb::MBC_TYPES mbc_type, lmgb::ROM_SIZES rom_size,
               lmgb::RAM_SIZES ram_size, std::vector<byte> &rom_data) {
  switch (mbc_type) {
  default:
  case ROM_ONLY:
    memory_controller = new nombc(rom_size, ram_size, rom_data);
    break;
  case MBC1:
  case MBC1_RAM:
    memory_controller = new mbc1(rom_size, ram_size, rom_data);
    break;
  }
}

lmgb::mem::~mem() {
  delete memory_controller;
}

byte lmgb::mem::Read(word addr) {
  switch (addr & 0xf000) {
  case 0x0000:
  case 0x1000:
  case 0x2000:
  case 0x3000:
  case 0x4000:
  case 0xa000:
  case 0xb000:
    return memory_controller->read(addr);

  default:
    return -1;
  }
}

void lmgb::mem::Write(word addr, byte val) {
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
    memory_controller->write(addr, val);
    break;

  case 0x8000:
  case 0x9000:
    // vram
    break;

  default:
    return;
  }
}
