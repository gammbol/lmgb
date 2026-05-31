#ifndef LMGB_GRAPHICS_H
#define LMGB_GRAPHICS_H

#include <array>
#include <cstdint>
#include <defs.h>

namespace lmgb {

inline constexpr int SCREEN_WIDTH = 160;
inline constexpr int SCREEN_HEIGHT = 144;

inline constexpr word LCDC_ADDRESS = 0xff40;
inline constexpr word STAT_ADDRESS = 0xff41;
inline constexpr word SCY_ADDRESS = 0xff42;
inline constexpr word SCX_ADDRESS = 0xff43;
inline constexpr word LY_ADDRESS = 0xff44;
inline constexpr word LYC_ADDRESS = 0xff45;
inline constexpr word WY_ADDRESS = 0xff4a;
inline constexpr word WX_ADDRESS = 0xff4b;

inline constexpr word BGP_ADDRESS = 0xff47;
inline constexpr word OBP0_ADDRESS = 0xff48;
inline constexpr word OBP1_ADDRESS = 0xff49;

inline constexpr word VRAM_SIZE = 0x2000;
inline constexpr word OAM_SIZE = 0xa0;
inline constexpr byte OAM_COUNT = 40;

inline constexpr std::array<std::uint32_t, 4> DMG_COLORS {
    0x9bbc0f,
    0x8bac0f,
    0x306230,
    0x0f380f
};

class dmg_palette {
public:
    byte read() const;
    void write(byte val);

    byte shade_id(byte color_id) const;
    std::uint32_t color(byte color_id) const;

private:
    byte reg_{};
};

struct object {
  byte y{};
  byte x{};
  byte tile_index{};
  byte flags{};

  int screen_x() const;
  int screen_y() const;

  bool x_flip() const;
  bool y_flip() const;
  bool behind_bg() const;
  byte palette_id() const;
};

class ppu {
public:
  byte read(word addr) const;
  void write(word addr, byte val);

  void step(int cycles);

  bool frame_ready() const;
  void clear_frame_ready();

  const std::array<std::uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT>& framebuffer() const;

private:
  byte read_vram(word offset) const;
  void write_vram(word offset, byte val);

  byte read_oam(word offset) const;
  void write_oam(word offset, byte val);

  byte read_register(word addr) const;
  void write_register(word addr, byte val);

  byte bg_window_tile_pixel(byte tile_id, byte x, byte y) const;
  byte object_tile_pixel(byte tile_id, byte x, byte y) const;

  object read_object(byte index) const;

  bool lcd_enabled() const;
  bool bg_enabled() const;
  bool window_enabled() const;
  bool objects_enabled() const;

  word bg_tile_map_base() const;
  word window_tile_map_base() const;
  bool bg_window_unsigned_addressing() const;
  byte object_height() const;

  void render_scanline();
  void render_background_line();
  void render_window_line();
  void render_object_line();

private:
  std::array<byte, VRAM_SIZE> vram_{};
  std::array<byte, OAM_SIZE> oam_{};

  byte lcdc_{};
  byte stat_{};

  byte scy_{};
  byte scx_{};

  byte ly_{};
  byte lyc_{};

  dmg_palette bgp_{};
  dmg_palette obp0_{};
  dmg_palette obp1_{};

  byte wy_{};
  byte wx_{};

private:
  enum class mode {
    hblank = 0,
    vblank = 1,
    oam_scan = 2,
    drawing = 3
  };

  mode mode_{mode::oam_scan};
  int dots_{};
  bool frame_ready_{};

  std::array<std::uint32_t, SCREEN_WIDTH * SCREEN_HEIGHT> framebuffer_{};
  std::array<byte, SCREEN_WIDTH> bg_color_ids_{};
};

}

#endif