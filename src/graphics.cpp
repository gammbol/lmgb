#include <graphics.h>
#include <cassert>
#include <vector>
#include <algorithm>

namespace lmgb {

// dmg_palette
byte dmg_palette::read() const {
  return reg_;
}

void dmg_palette::write(byte val) {
  reg_ = val;
}

byte dmg_palette::shade_id(byte color_id) const {
  color_id &= 0x03;
  return static_cast<byte>((reg_ >> (color_id * 2)) & 0x03);
}

std::uint32_t dmg_palette::color(byte color_id) const {
  return DMG_COLORS[shade_id(color_id)];
}


// object
int object::screen_x() const {
  return static_cast<int>(x) - 8;
}

int object::screen_y() const {
  return static_cast<int>(y) - 16;
}

bool object::behind_bg() const {
  return (flags & 0x80) != 0;
}

bool object::y_flip() const {
  return (flags & 0x40) != 0;
}

bool object::x_flip() const {
  return (flags & 0x20) != 0;
}

byte object::palette_id() const {
  return (flags & 0x10) ? 1 : 0;
}


// ppu
byte ppu::read(word addr) const {
  if (addr >= 0x8000 && addr <= 0x9FFF) {
    return read_vram(addr - 0x8000);
  }

  if (addr >= 0xFE00 && addr <= 0xFE9F) {
    return read_oam(addr - 0xFE00);
  }

  if (addr >= 0xFF40 && addr <= 0xFF4B) {
    return read_register(addr);
  }

  return 0xFF;
}

void ppu::write(word addr, byte val) {
  if (addr >= 0x8000 && addr <= 0x9FFF) {
    write_vram(addr - 0x8000, val);
    return;
  }

  if (addr >= 0xFE00 && addr <= 0xFE9F) {
    write_oam(addr - 0xFE00, val);
    return;
  }

  if (addr >= 0xFF40 && addr <= 0xFF4B) {
    write_register(addr, val);
    return;
  }
}

bool ppu::frame_ready() const { return frame_ready_; }
void ppu::clear_frame_ready() { frame_ready_ = false; }

const std::array<std::uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT>& ppu::framebuffer() const {
  return framebuffer_;
}

byte ppu::read_vram(word offset) const { 
  assert(offset < vram_.size());
  return vram_[offset]; 
}
void ppu::write_vram(word offset, byte val) { 
  assert(offset < vram_.size());
  vram_[offset] = val; 
}

byte ppu::read_oam(word offset) const { 
  assert(offset < oam_.size());
  return oam_[offset]; 
}
void ppu::write_oam(word offset, byte val) { 
  assert(offset < oam_.size());
  oam_[offset] = val; 
}

byte ppu::read_register(word addr) const {
  switch(addr) {
  case LCDC_ADDRESS: return lcdc_;
  case STAT_ADDRESS: return stat_;
  case SCY_ADDRESS: return scy_;
  case SCX_ADDRESS: return scx_;
  case LY_ADDRESS: return ly_;
  case LYC_ADDRESS: return lyc_;
  case WY_ADDRESS: return wy_;
  case WX_ADDRESS: return wx_;

  case BGP_ADDRESS: return bgp_.read();
  case OBP0_ADDRESS: return obp0_.read();
  case OBP1_ADDRESS: return obp1_.read();

  default:
    return 0xff;
  }
}
void ppu::write_register(word addr, byte val) {
  switch(addr) {
  case LCDC_ADDRESS: lcdc_ = val; break;
  case STAT_ADDRESS: 
    stat_ = static_cast<byte>((stat_ & 0x07) | (val & 0x78)); 
    break;
  case SCY_ADDRESS: scy_ = val; break;
  case SCX_ADDRESS: scx_ = val; break;
  case LY_ADDRESS: ly_ = 0; break;
  case LYC_ADDRESS: lyc_ = val; break;
  case WY_ADDRESS: wy_ = val; break;
  case WX_ADDRESS: wx_ = val; break;

  case BGP_ADDRESS: bgp_.write(val); break;
  case OBP0_ADDRESS: obp0_.write(val); break;
  case OBP1_ADDRESS: obp1_.write(val); break;
  }
}

byte ppu::bg_window_tile_pixel(byte tile_id, byte x, byte y) const {
  assert(x < 8);
  assert(y < 8);

  word tile_addr{};

  if (bg_window_unsigned_addressing()) {
    tile_addr = static_cast<word>(tile_id) * 16;
  } else {
    const auto signed_id = static_cast<std::int8_t>(tile_id);
    tile_addr = static_cast<word>(0x1000 + signed_id * 16);
  }

  const word row_addr = tile_addr + y * 2;

  const byte lo = vram_[row_addr];
  const byte hi = vram_[row_addr + 1];

  const byte bit = static_cast<byte>(7 - x);

  return static_cast<byte>(
    (((hi >> bit) & 1) << 1) |
    ((lo >> bit) & 1)
  );
}

byte ppu::object_tile_pixel(byte tile_id, byte x, byte y) const {
  assert(x < 8);
  assert(y < object_height());

  word tile_addr = static_cast<word>(tile_id) * 16;

  if (object_height() == 16) {
    tile_addr = static_cast<word>(tile_id & 0xFE) * 16;
  }

  const word row_addr = tile_addr + y * 2;

  const byte lo = vram_[row_addr];
  const byte hi = vram_[row_addr + 1];

  const byte bit = static_cast<byte>(7 - x);

  return static_cast<byte>(
    (((hi >> bit) & 1) << 1) |
    ((lo >> bit) & 1)
  );
}

object ppu::read_object(byte index) const {
  assert(index < 40);

  const word offset = static_cast<word>(index) * 4;
  
  return {
    .y = oam_[offset],
    .x = oam_[offset + 1],
    .tile_index = oam_[offset + 2],
    .flags = oam_[offset + 3]
  };
}

bool ppu::lcd_enabled() const {
  return (lcdc_ & 0x80) != 0;
}
bool ppu::bg_enabled() const {
  return (lcdc_ & 0x01) != 0;
}
bool ppu::window_enabled() const {
  return (lcdc_ & 0x20) != 0;
}
bool ppu::objects_enabled() const {
  return (lcdc_ & 0x02) != 0;
}

word ppu::bg_tile_map_base() const {
  return (lcdc_ & 0x08) != 0 ? 0x1C00 : 0x1800;
}
word ppu::window_tile_map_base() const {
  return (lcdc_ & 0x40) != 0 ? 0x1C00 : 0x1800;
}
bool ppu::bg_window_unsigned_addressing() const {
  return (lcdc_ & 0x10) != 0;
}
byte ppu::object_height() const {
  return (lcdc_ & 0x04) != 0 ? 16 : 8;
}

void ppu::render_scanline() {
  if (mode_ == mode::vblank) return;

  render_background_line();
  render_window_line();
  render_object_line();
}
void ppu::render_background_line() {
  if (!bg_enabled()) return;

  byte height = (ly_ * SCREEN_WIDTH);
  for (int i = 0; i < SCREEN_WIDTH; ++i) {
    byte tile_id = read_vram(bg_tile_map_base() + height + i);
    framebuffer_[height + i] = bg_window_tile_pixel(tile_id, scx_, scy_);
  }
}
void ppu::render_window_line() {
  if (!window_enabled()) return;

  byte height = (ly_ * SCREEN_WIDTH);
  for (int i = 0; i < SCREEN_WIDTH; ++i) {
    byte tile_id = read_vram(window_tile_map_base() + height + i);
    framebuffer_[height + i] = bg_window_tile_pixel(tile_id, wx_, wy_);
  }
}
void ppu::render_object_line() {
  if (!objects_enabled()) return;

  byte objects_count{};

  byte height = (ly_ * SCREEN_HEIGHT);

  std::vector<object> objects{};
  for (int i = 0; i < OAM_COUNT && objects_count < 10; ++i) {
    object obj = read_object(i);
    if (
      obj.screen_y() - object_height() < ly_ &&
      obj.screen_y() > ly_ &&
      !obj.behind_bg()
    ) {
      objects.push_back(obj);
      ++objects_count;
    }
  }

  std::sort(objects.begin(), objects.end(), [](auto a, auto b) {
    return a.x < b.x;
  });

  for (auto obj : objects) {
    for (int i = 0; i + obj.screen_x() < SCREEN_WIDTH; ++i) {
      auto color_id = object_tile_pixel(obj.tile_index, obj.x, obj.y);
      switch(obj.palette_id()) {
      case 0:
        framebuffer_[i] = obp0_.color(color_id);
        break;
      case 1:
        framebuffer_[i] = obp1_.color(color_id);
        break;
      }
    }
  }
}


}

