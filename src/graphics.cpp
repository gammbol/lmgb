#include <graphics.h>

void lmgb::graphics::switchMode() {
  switch(mode) {
  case SCANNING: 
    mode = DRAWING;
    break;
  case DRAWING:
    mode = HBLANK;
    last_drawing_ticks = current_ticks;
    break;
  case HBLANK:
    if (ly > 143) {
      mode = VBLANK;
    } else {
      mode = SCANNING;
      ly++;
    }
    break;
  case VBLANK:
    mode = SCANNING;
    break;
  }

  current_ticks = 0;
}

void lmgb::graphics::Step(int steps) {
  int stod = steps * 4;
  while (stod) {
    switch (mode > 0) {
    case SCANNING:
      if (lcdc & 0x02 == 0) break;
      if (!current_ticks) {
        for (int i = 0; i < 160; ++i) {
          scanLine[i] = tileBlock[]
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
lmgb::byte lmgb::graphics::read(word addr) {
  // OAM read
  if (addr >= 0xfe00 && addr <= 0xfe9f) {
    if (mode == SCANNING || mode == DRAWING) return 0xff;
    byte id = addr & 0x00ff;
    return oam[id/4][id%4];
  }

  // VRAM read
  if (addr >= 0x8000 && addr <= 0x9fff) {
    if (mode == DRAWING) return 0xff;

    // tile blocks
    if (addr < 0x9800) {
      byte id = addr - 0x8000;
      return tileBlock[id/2048][id/16].getByte(id%16);
    }
    // tile maps
    else {
      byte id = addr - 0x9800;
      return tileMap[id/1024][id%1024];
    }
  }

  // other registers
  switch(addr) {
  case 0xff40: return lcdc;
  case 0xff41: return stat;
  case 0xff42: return scy;
  case 0xff43: return scx;
  case 0xff44: return ly;
  case 0xff45: return lyc;
  case 0xff46: return dma_src;
  case 0xff47: return bgp;
  case 0xff48: return obp[0];
  case 0xff49: return obp[1];
  case 0xff4a: return wy;
  case 0xff4b: return wx;
  }

  return 0xff;
}

void lmgb::graphics::write(word addr, byte val) {
  // OAM write
  if (addr >= 0xfe00 && addr <= 0xfe9f) {
    if (mode == SCANNING || mode == DRAWING) return;
    byte id = addr & 0x00ff;
    oam[id/4][id%4] = val;
  }

  // VRAM write
  if (addr >= 0x8000 && addr <= 0x9fff) {
    if (mode == DRAWING) return;

    // tile blocks
    if (addr < 0x9800) {
      byte id = addr - 0x8000;
      tileBlock[id/2048][id/16].setByte(id%16, val);
    }
    // tile maps
    else {
      byte id = addr - 0x9800;
      tileMap[id/1024][id%1024] = val;
    }
  }

  // other registers
  switch(addr) {
  case 0xff40: 
    lcdc = val;
    break;
  case 0xff41:
    stat = val;
    break;
  case 0xff42:
    scy = val;
    break;
  case 0xff43:
    scx = val;
    break;
  case 0xff44:
    ly = val;
    break;
  case 0xff45:
    lyc = val;
    break;
  case 0xff46:
    dma_src = val & 0xff00;
    dma_cur = 0;
    isDmaTransfer = true;
    break;
  case 0xff47:
    bgp = val;
    break;
  case 0xff48:
    obp[0] = val;
    break;
  case 0xff49:
    obp[1] = val;
    break;
  case 0xff4a:
    wy = val;
    break;
  case 0xff4b:
    wx = val;
    break;
  }
}