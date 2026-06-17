#include <mem.h>

namespace lmgb {

mem::mem(MBC_TYPES mbc_type, ROM_SIZES rom_size,
        RAM_SIZES ram_size, std::vector<byte> &rom_data, 
        ppu& ppu, interrupts& interrupt_handler) : 
        pixel_processing_unit_(ppu),
        interrupt_handler_(interrupt_handler)
{
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

void mem::dma_transfer(word addr) {
  word src = addr & 0xff00;

  if (addr > 0xdf00) return;

  for (word i = 0; i < 0xa0; ++i) {
    byte data = Read(src + i);
    pixel_processing_unit_.write(0xfe00 + i, data);
  }
}

int mem::consume_pending_cycles() { 
  int cycles = pending_cycles_;
  pending_cycles_ = 0;
  return cycles;
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

  case 0x8000:
  case 0x9000:
    return pixel_processing_unit_.read(addr);

  case 0xc000:
  case 0xd000:
    return wram_[addr - 0xc000];
  
  case 0xf000: {
    if (addr >= 0xfea0 && addr <= 0xfeff) return 0xff;

    if (
      addr == INTERRUPT_ENABLE_ADDRESS ||
      addr == INTERRUPT_FLAG_ADDRESS
    ) {
      return interrupt_handler_.read(addr);
    } else if (addr >= 0xff04 && addr <= 0xff07) {
      timer_.read(addr);
    } else if (addr >= 0xff40 && addr <= 0xff4b) {
      return pixel_processing_unit_.read(addr);
    } else if (addr >= 0xff80 && addr <= 0xfffe) {
      return hram_[addr - 0xff80];
    }
  }

  default:
    return 0xff;
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
    pixel_processing_unit_.write(addr, val);
    break;

  case 0xc000:
  case 0xd000:
    wram_[addr - 0xc000] = val;

  case 0xf000: {
    if (addr >= 0xfea0 && addr <= 0xfeff) return;

    if (
      addr == INTERRUPT_ENABLE_ADDRESS ||
      addr == INTERRUPT_FLAG_ADDRESS
    ) {
      interrupt_handler_.write(addr, val);
    } else if (addr >= 0xff04 && addr <= 0xff07) {
      timer_.write(addr, val);
    } else if (addr >= 0xff40 && addr <= 0xff4b) {
      if (addr == 0xff46) {
        dma_transfer(addr);
        pending_cycles_ += 160;
        return;
      }

      pixel_processing_unit_.write(addr, val);
    } else if (addr >= 0xff80 && addr <= 0xfffe) {
      hram_[addr - 0xff80] = val;
    }
    break;
  }

  default:
    return;
  }
}

}
