#include <graphics.h>

void lmgb::ppu::switchMode() {
  switch(lcd_control.get_mode()) {
  case SCANNING: 
    lcd_control.set_mode(DRAWING);
    break;
  case DRAWING:
    lcd_control.set_mode(HBLANK);
    lcd_control.last_drawing_ticks_ = lcd_control.current_ticks_;
    break;
  case HBLANK:
    if (lcd_control.ly_ > 143) {
      lcd_control.set_mode(VBLANK);
    } else {
      lcd_control.set_mode(SCANNING);
      lcd_control.ly_++;
    }
    break;
  case VBLANK:
    lcd_control.set_mode(SCANNING);
    break;
  }

  lcd_control.current_ticks_ = 0;
}

void lmgb::ppu::step(int ticks) {
  int stod = ticks * 4;
  while (stod) {
    switch (lcd_control.get_mode() > 0) {
    case SCANNING:
      if (lcd_control.lcdc & 0x02 == 0) break;
      if (!lcd_control.current_ticks_) {
        for (int i = 0; i < 160; ++i) {
          scanLine[i] = tile_blocks.read_by_id(
            tile_maps.get_tile(lcd_control.lcdc & 0x08, bg.scx, bg.scy),
            lcd_control.lcdc & 0x10
          );
        }
      }
      stod -= 2;
      oam_obj obj = oam.get(0);
      byte tileMode = (lcdc & 0x04) ? 16 : 8;
      if (ly+16 >= obj.posy && ly+16 <= obj.posy+tileMode) {

      }
      ++current_ticks;
      if (current_ticks >= 80) switchMode();
      break;
    case DRAWING:
      stod -= 1;

      break;
    }
  }
}

// alert! retard code
lmgb::byte lmgb::ppu::read(word addr) {
  auto mode = lcd_control.get_mode();
  // OAM read
  if (addr >= 0xfe00 && addr <= 0xfe9f) {
    if (mode == SCANNING || mode == DRAWING) return 0xff;
    return object_mem.read(addr);
  }

  // VRAM read
  if (addr >= 0x8000 && addr <= 0x9fff) {
    if (mode == DRAWING) return 0xff;

    // tile blocks
    if (addr < 0x9800) {
      return tile_blocks.read(addr);
    }
    // tile maps
    else {
      return tile_maps.read(addr);
    }
  }

  // other registers
  switch(addr) {
  case 0xff40: return lcd_control.lcdc;
  case 0xff41: return lcd_control.stat;
  case 0xff42: return bg.scy;
  case 0xff43: return bg.scx;
  case 0xff44: return lcd_control.ly_;
  case 0xff45: return lcd_control.lyc;
  case 0xff46: return object_mem.dma_src;
  case 0xff47: return plt.bgp;
  case 0xff48: return plt.obp[0];
  case 0xff49: return plt.obp[1];
  case 0xff4a: return wnd.wy;
  case 0xff4b: return wnd.wx;
  }

  return 0xff;
}

void lmgb::ppu::write(word addr, byte val) {
  auto mode = lcd_control.get_mode();
  // OAM write
  if (addr >= 0xfe00 && addr <= 0xfe9f) {
    if (mode == SCANNING || mode == DRAWING) return;
    object_mem.write(addr, val);
  }

  // VRAM write
  if (addr >= 0x8000 && addr <= 0x9fff) {
    if (mode == DRAWING) return;

    // tile blocks
    if (addr < 0x9800) {
      tile_blocks.write(addr, val);
    }
    // tile maps
    else {
      tile_maps.write(addr, val);
    }
  }

  // other registers
  switch(addr) {
  case 0xff40: 
    lcd_control.lcdc = val;
    break;
  case 0xff41:
    lcd_control.stat = val;
    break;
  case 0xff42:
    bg.scy = val;
    break;
  case 0xff43:
    bg.scx = val;
    break;
  case 0xff44:
    return;
  case 0xff45:
    lcd_control.lyc = val;
    break;
  case 0xff46:
    object_mem.dma_src = val & 0xff00;
    object_mem.dma_cur = 0;
    object_mem.isTransferring = true;
    break;
  case 0xff47:
    plt.bgp = val;
    break;
  case 0xff48:
    plt.obp[0] = val;
    break;
  case 0xff49:
    plt.obp[1] = val;
    break;
  case 0xff4a:
    wnd.wy = val;
    break;
  case 0xff4b:
    wnd.wx = val;
    break;
  }
}