#include <mem.h>

namespace lmgb {

mem::mem(MBC_TYPES mbc_type, ROM_SIZES rom_size,
         RAM_SIZES ram_size, std::vector<byte> &rom_data, ppu& ppu,
         interrupts& interrupt_handler, timer& timer_ref)
    : pixel_processing_unit_(ppu), interrupt_handler_(interrupt_handler),
      timer_(timer_ref) {
  switch (mbc_type) {
  case MBC1:
  case MBC1_RAM:
    memory_controller_ = new mbc1(rom_size, ram_size, rom_data);
    break;
  case ROM_ONLY:
  default:
    memory_controller_ = new nombc(rom_size, ram_size, rom_data);
    break;
  }
}

mem::~mem() { delete memory_controller_; }

void mem::dma_transfer(byte high_addr) {
  word src = static_cast<word>(high_addr) << 8;

  for (word i = 0; i < 0xa0; ++i) {
    byte data = Read(static_cast<word>(src + i));
    pixel_processing_unit_.write(static_cast<word>(0xfe00 + i), data);
  }
}

int mem::consume_pending_cycles() {
  int cycles = pending_cycles_;
  pending_cycles_ = 0;
  return cycles;
}

byte mem::Read(word addr) {
  if (addr <= 0x7fff || (addr >= 0xa000 && addr <= 0xbfff)) {
    return memory_controller_->read(addr);
  }

  if (addr >= 0x8000 && addr <= 0x9fff) {
    return pixel_processing_unit_.read(addr);
  }

  if (addr >= 0xc000 && addr <= 0xdfff) {
    return wram_[addr - 0xc000];
  }

  if (addr >= 0xe000 && addr <= 0xfdff) {
    return wram_[addr - 0xe000];
  }

  if (addr >= 0xfe00 && addr <= 0xfe9f) {
    return pixel_processing_unit_.read(addr);
  }

  if (addr >= 0xfea0 && addr <= 0xfeff) {
    return 0xff;
  }

  if (addr == INTERRUPT_FLAG_ADDRESS || addr == INTERRUPT_ENABLE_ADDRESS) {
    return interrupt_handler_.read(addr);
  }

  if (addr >= 0xff04 && addr <= 0xff07) {
    return timer_.read(addr);
  }

  if (addr >= 0xff40 && addr <= 0xff4b) {
    return pixel_processing_unit_.read(addr);
  }

  if (addr >= 0xff80 && addr <= 0xfffe) {
    return hram_[addr - 0xff80];
  }

  return 0xff;
}

void mem::Write(word addr, byte val) {
  if (addr <= 0x7fff || (addr >= 0xa000 && addr <= 0xbfff)) {
    memory_controller_->write(addr, val);
    return;
  }

  if (addr >= 0x8000 && addr <= 0x9fff) {
    pixel_processing_unit_.write(addr, val);
    return;
  }

  if (addr >= 0xc000 && addr <= 0xdfff) {
    wram_[addr - 0xc000] = val;
    return;
  }

  if (addr >= 0xe000 && addr <= 0xfdff) {
    wram_[addr - 0xe000] = val;
    return;
  }

  if (addr >= 0xfe00 && addr <= 0xfe9f) {
    pixel_processing_unit_.write(addr, val);
    return;
  }

  if (addr >= 0xfea0 && addr <= 0xfeff) {
    return;
  }

  if (addr == INTERRUPT_FLAG_ADDRESS || addr == INTERRUPT_ENABLE_ADDRESS) {
    interrupt_handler_.write(addr, val);
    return;
  }

  if (addr >= 0xff04 && addr <= 0xff07) {
    timer_.write(addr, val);
    return;
  }

  if (addr >= 0xff40 && addr <= 0xff4b) {
    if (addr == 0xff46) {
      dma_transfer(val);
      pending_cycles_ += 160;
      return;
    }

    pixel_processing_unit_.write(addr, val);
    return;
  }

  if (addr >= 0xff80 && addr <= 0xfffe) {
    hram_[addr - 0xff80] = val;
    return;
  }
}

} // namespace lmgb
