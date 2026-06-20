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

void ppu::step(int cycles) {
  if (!lcd_enabled()) {
    ly_ = 0;
    dots_ = 0;
    mode_ = mode::oam_scan;
    frame_ready_ = false;
    return;
  }

  dots_ += cycles;

  switch(mode_) {
  
  case mode::oam_scan: {
    if (dots_ >= OAM_SCAN_LENGTH) {
      dots_ -= OAM_SCAN_LENGTH;
      mode_ = mode::drawing;
    }
    break;
  }

  case mode::drawing: {
    if (dots_ >= DRAWING_LENGTH) {
      dots_ -= DRAWING_LENGTH;
      render_scanline();
      mode_ = mode::hblank;
    }
    break;
  }

  case mode::hblank: {
    if (dots_ >= HBLANK_LENGTH) {
      dots_ -= HBLANK_LENGTH;
      ++ly_;

      if (ly_ == SCREEN_HEIGHT) {
        mode_ = mode::vblank;
        frame_ready_ = true;
        interrupt_handler_.request_interrupt(INT_TYPE::VBLANK);
      } else {
        mode_ = mode::oam_scan;
      }
    }
    break;
  }

  case mode::vblank: {
    if (dots_ >= SCANLINE_LENGTH) {
      dots_ -= SCANLINE_LENGTH;
      ++ly_;

      if (ly_ > 153) {
        ly_ = 0;
        frame_ready_ = false;
        mode_ = mode::oam_scan;
      }
    }
    break;
  }
  
  }

  stat_check_interrupts();
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
  case STAT_ADDRESS: return stat_ | 0x80;
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
    stat_ = val;
    stat_update(); 
    break;
  case SCY_ADDRESS: scy_ = val; break;
  case SCX_ADDRESS: scx_ = val; break;
  case LY_ADDRESS: ly_ = 0; break;
  case LYC_ADDRESS: 
    lyc_ = val; 
    stat_update(); 
    break;
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
  if (ly_ >= SCREEN_HEIGHT) return;

  render_background_line();
  render_window_line();
  render_object_line();
}
void ppu::render_background_line() {
  std::fill(bg_color_ids_.begin(), bg_color_ids_.end(), 0);
  const int line_offset = static_cast<int>(ly_) * SCREEN_WIDTH;

  if (!bg_enabled()) {
    for (int screen_x = 0; screen_x < SCREEN_WIDTH; ++screen_x) {
      framebuffer_[line_offset + screen_x] = bgp_.color(0);
    }
    return;
  }


  for (int screen_x = 0; screen_x < SCREEN_WIDTH; ++screen_x) {
    const byte bg_x = static_cast<byte>(scx_ + screen_x);
    const byte bg_y = static_cast<byte>(scy_ + ly_);

    const int tile_x = bg_x / 8;
    const int tile_y = bg_y / 8;

    const int pixel_x = bg_x % 8;
    const int pixel_y = bg_y % 8;

    byte tile_id = read_vram(
      bg_tile_map_base() + tile_y * 32 + tile_x
    );
    const byte color_id = bg_window_tile_pixel(tile_id, 
      static_cast<byte>(pixel_x), 
      static_cast<byte>(pixel_y)
    );

    bg_color_ids_[screen_x] = color_id;
    framebuffer_[line_offset + screen_x] = bgp_.color(color_id);
  }
}
void ppu::render_window_line() {
  if (!window_enabled() || ly_ < wy_) return;

  const int line_offset = static_cast<int>(ly_) * SCREEN_WIDTH;
  const int window_screen_x = static_cast<int>(wx_) - 7;

  for (int screen_x = 0; screen_x < SCREEN_WIDTH; ++screen_x) {
    if (screen_x < window_screen_x) continue;

    const int win_x = screen_x - window_screen_x;
    const int win_y = static_cast<int>(ly_) - static_cast<int>(wy_);

    const int tile_x = win_x / 8;
    const int tile_y = win_y / 8;

    const int pixel_x = win_x % 8;
    const int pixel_y = win_y % 8;

    const byte tile_id = read_vram(window_tile_map_base() + tile_y * 32 + tile_x);
    const auto color_id = bg_window_tile_pixel(
      tile_id, 
      static_cast<byte>(pixel_x), 
      static_cast<byte>(pixel_y)
    );

    bg_color_ids_[screen_x] = color_id;
    framebuffer_[line_offset + screen_x] = bgp_.color(color_id);
  }
}
void ppu::render_object_line() {
  if (!objects_enabled()) return;

  const int line_offset = static_cast<int>(ly_) * SCREEN_WIDTH;

  const int height = object_height();
  std::vector<object> objects{};
  for (int i = 0; i < OAM_COUNT && objects.size() < 10; ++i) {
    object obj = read_object(i);
    int sprite_y = obj.screen_y();

    bool intersects =
      sprite_y <= ly_ &&
      ly_ < sprite_y + height;

    if (intersects) {
      objects.push_back(obj);
    }
  }

  std::sort(
    objects.begin(), 
    objects.end(), 
    [](const object& left, const object& right) {
      return left.x > right.x;
    }
  );

  for (const auto& obj : objects) {
    for (int sprite_x = 0; sprite_x < 8; ++sprite_x) {
      auto screen_x = obj.screen_x() + sprite_x;

      if (screen_x < 0 || screen_x >= SCREEN_WIDTH) {
        continue;
      }

      int tile_x = sprite_x;
      int tile_y = static_cast<int>(ly_) - obj.screen_y();

      if (obj.x_flip()) tile_x = 7 - tile_x;
      if (obj.y_flip()) tile_y = height - 1 - tile_y;

      auto color_id = object_tile_pixel(
        obj.tile_index, 
        static_cast<byte>(tile_x), 
        static_cast<byte>(tile_y)
      );

      if (color_id == 0) continue;
      if (obj.behind_bg() && bg_color_ids_[screen_x] != 0) continue;
      
      framebuffer_[line_offset + screen_x] = obj.palette_id() == 0
        ? obp0_.color(color_id)
        : obp1_.color(color_id);
    }
  }
}

unsigned ppu::m_to_t_convert(const unsigned m_cycles) const { 
  return m_cycles * T_CYCLES_PER_M;
}

bool ppu::stat_lyc_selected() const {
  return (stat_ & 0x40) != 0;
}
bool ppu::stat_mode2_selected() const {
  return (stat_ & 0x20) != 0;
}
bool ppu::stat_mode1_selected() const {
  return (stat_ & 0x10) != 0;
}
bool ppu::stat_mode0_selected() const {
  return (stat_ & 0x08) != 0;
}

void ppu::stat_update() {
  if (ly_ == lyc_) stat_ |= 0x04;
  else stat_ &= 0xfb;

  stat_ = (stat_ & 0xfc) | static_cast<byte>(mode_);
}

void ppu::stat_check_interrupts() {
  stat_update();

  bool stat_signal = 
    ((ly_ == lyc_) &&  stat_lyc_selected()) ||
    ((mode_ ==  mode::hblank) && stat_mode0_selected()) ||
    ((mode_ == mode::vblank) && stat_mode1_selected()) ||
    ((mode_ == mode::oam_scan) && stat_mode2_selected());

  if (stat_signal && !stat_interrupt_line_)
    interrupt_handler_.request_interrupt(LCDC);

  stat_interrupt_line_ = stat_signal;

}


}

