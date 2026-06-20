#include <cpu.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>

namespace lmgb {
namespace {

inline bool bit(byte value, int pos) { return ((value >> pos) & 1) != 0; }
inline byte lo(word value) { return static_cast<byte>(value & 0xff); }
inline byte hi(word value) { return static_cast<byte>((value >> 8) & 0xff); }

} // namespace

cpu::cpu(lmgb::mem& memory, lmgb::interrupts& interrupt_handler)
    : memory_(memory), interrupt_handler_(interrupt_handler) {
  state = RUNNING;
  ime = false;

  // DMG state after boot ROM, enough for cartridge-only start at 0x0100.
  af.pair = 0x01b0;
  bc.pair = 0x0013;
  de.pair = 0x00d8;
  hl.pair = 0x014d;
  sp = 0xfffe;
  pc = 0x0100;

  memory_.Write(0xff05, 0x00);
  memory_.Write(0xff06, 0x00);
  memory_.Write(0xff07, 0x00);
  memory_.Write(0xff10, 0x80);
  memory_.Write(0xff11, 0xbf);
  memory_.Write(0xff12, 0xf3);
  memory_.Write(0xff14, 0xbf);
  memory_.Write(0xff16, 0x3f);
  memory_.Write(0xff17, 0x00);
  memory_.Write(0xff19, 0xbf);
  memory_.Write(0xff1a, 0x7f);
  memory_.Write(0xff1b, 0xff);
  memory_.Write(0xff1c, 0x9f);
  memory_.Write(0xff1e, 0xbf);
  memory_.Write(0xff20, 0xff);
  memory_.Write(0xff21, 0x00);
  memory_.Write(0xff22, 0x00);
  memory_.Write(0xff23, 0xbf);
  memory_.Write(0xff24, 0x77);
  memory_.Write(0xff25, 0xf3);
  memory_.Write(0xff26, 0xf1);
  memory_.Write(0xff40, 0x91);
  memory_.Write(0xff42, 0x00);
  memory_.Write(0xff43, 0x00);
  memory_.Write(0xff45, 0x00);
  memory_.Write(0xff47, 0xfc);
  memory_.Write(0xff48, 0xff);
  memory_.Write(0xff49, 0xff);
  memory_.Write(0xff4a, 0x00);
  memory_.Write(0xff4b, 0x00);
  memory_.Write(0xff0f, 0xe1);
  memory_.Write(0xffff, 0x00);
}

void cpu::pushByte(byte val) { memory_.Write(--sp, val); }

void cpu::pushWord(word val) {
  pushByte(hi(val));
  pushByte(lo(val));
}

byte cpu::popByte() { return memory_.Read(sp++); }

word cpu::popWord() {
  byte low = popByte();
  byte high = popByte();
  return static_cast<word>((high << 8) | low);
}

byte cpu::fetch_u8() { return memory_.Read(pc++); }

word cpu::fetch_u16() {
  byte low = fetch_u8();
  byte high = fetch_u8();
  return static_cast<word>((high << 8) | low);
}

byte cpu::readOp(word &pc_ref) { return memory_.Read(pc_ref++); }

unsigned cpu::step() {
  if (state == STOPPED) return 4;

  const word opcode_pc = pc;
  const byte opcode = readOp(pc);
  unsigned cycles = 4;

  auto get_f = [this](byte mask) { return (af.bytes.l & mask) != 0; };
  auto set_f = [this](byte mask, bool value) {
    if (value) af.bytes.l |= mask;
    else af.bytes.l &= static_cast<byte>(~mask);
    af.bytes.l &= 0xf0;
  };
  auto set_znhc = [&](bool z, bool n, bool h, bool c) {
    af.bytes.l = 0;
    if (z) af.bytes.l |= Z_FLAG;
    if (n) af.bytes.l |= N_FLAG;
    if (h) af.bytes.l |= H_FLAG;
    if (c) af.bytes.l |= C_FLAG;
  };

  auto read_r = [&](int index) -> byte {
    switch (index) {
    case 0: return bc.bytes.h;
    case 1: return bc.bytes.l;
    case 2: return de.bytes.h;
    case 3: return de.bytes.l;
    case 4: return hl.bytes.h;
    case 5: return hl.bytes.l;
    case 6: return memory_.Read(hl.pair);
    case 7: return af.bytes.h;
    default: return 0xff;
    }
  };

  auto write_r = [&](int index, byte value) {
    switch (index) {
    case 0: bc.bytes.h = value; break;
    case 1: bc.bytes.l = value; break;
    case 2: de.bytes.h = value; break;
    case 3: de.bytes.l = value; break;
    case 4: hl.bytes.h = value; break;
    case 5: hl.bytes.l = value; break;
    case 6: memory_.Write(hl.pair, value); break;
    case 7: af.bytes.h = value; break;
    }
  };

  auto read_r16 = [&]() -> word& { return bc.pair; };
  (void)read_r16;

  auto inc8 = [&](byte value) -> byte {
    byte result = static_cast<byte>(value + 1);
    set_f(Z_FLAG, result == 0);
    set_f(N_FLAG, false);
    set_f(H_FLAG, (value & 0x0f) == 0x0f);
    return result;
  };

  auto dec8 = [&](byte value) -> byte {
    byte result = static_cast<byte>(value - 1);
    set_f(Z_FLAG, result == 0);
    set_f(N_FLAG, true);
    set_f(H_FLAG, (value & 0x0f) == 0x00);
    return result;
  };

  auto add_a = [&](byte value) {
    unsigned result = af.bytes.h + value;
    set_znhc((result & 0xff) == 0, false,
             ((af.bytes.h & 0x0f) + (value & 0x0f)) > 0x0f, result > 0xff);
    af.bytes.h = static_cast<byte>(result);
  };

  auto adc_a = [&](byte value) {
    unsigned carry = get_f(C_FLAG) ? 1 : 0;
    unsigned result = af.bytes.h + value + carry;
    set_znhc((result & 0xff) == 0, false,
             ((af.bytes.h & 0x0f) + (value & 0x0f) + carry) > 0x0f,
             result > 0xff);
    af.bytes.h = static_cast<byte>(result);
  };

  auto sub_a = [&](byte value) {
    unsigned a = af.bytes.h;
    unsigned result = a - value;
    set_znhc((result & 0xff) == 0, true, (a & 0x0f) < (value & 0x0f), a < value);
    af.bytes.h = static_cast<byte>(result);
  };

  auto sbc_a = [&](byte value) {
    unsigned carry = get_f(C_FLAG) ? 1 : 0;
    unsigned a = af.bytes.h;
    unsigned result = a - value - carry;
    set_znhc((result & 0xff) == 0, true,
             (a & 0x0f) < ((value & 0x0f) + carry), a < value + carry);
    af.bytes.h = static_cast<byte>(result);
  };

  auto and_a = [&](byte value) {
    af.bytes.h &= value;
    set_znhc(af.bytes.h == 0, false, true, false);
  };

  auto xor_a = [&](byte value) {
    af.bytes.h ^= value;
    set_znhc(af.bytes.h == 0, false, false, false);
  };

  auto or_a = [&](byte value) {
    af.bytes.h |= value;
    set_znhc(af.bytes.h == 0, false, false, false);
  };

  auto cp_a = [&](byte value) {
    unsigned a = af.bytes.h;
    unsigned result = a - value;
    set_znhc((result & 0xff) == 0, true, (a & 0x0f) < (value & 0x0f), a < value);
  };

  auto add_hl = [&](word value) {
    unsigned result = hl.pair + value;
    set_f(N_FLAG, false);
    set_f(H_FLAG, ((hl.pair & 0x0fff) + (value & 0x0fff)) > 0x0fff);
    set_f(C_FLAG, result > 0xffff);
    hl.pair = static_cast<word>(result);
  };

  auto add_sp_e8 = [&](std::int8_t offset) -> word {
    word old_sp = sp;
    word result = static_cast<word>(sp + offset);
    byte uoff = static_cast<byte>(offset);
    set_znhc(false, false, ((old_sp & 0x0f) + (uoff & 0x0f)) > 0x0f,
             ((old_sp & 0xff) + uoff) > 0xff);
    return result;
  };

  auto jr = [&](bool condition) {
    auto offset = static_cast<std::int8_t>(fetch_u8());
    if (condition) {
      pc = static_cast<word>(pc + offset);
      cycles = 12;
    } else {
      cycles = 8;
    }
  };

  auto jp = [&](bool condition) {
    word addr = fetch_u16();
    if (condition) {
      pc = addr;
      cycles = 16;
    } else {
      cycles = 12;
    }
  };

  auto call = [&](bool condition) {
    word addr = fetch_u16();
    if (condition) {
      pushWord(pc);
      pc = addr;
      cycles = 24;
    } else {
      cycles = 12;
    }
  };

  auto ret = [&](bool condition) {
    if (condition) {
      pc = popWord();
      cycles = 20;
    } else {
      cycles = 8;
    }
  };

  auto rst = [&](word addr) {
    pushWord(pc);
    pc = addr;
    cycles = 16;
  };

  if (opcode >= 0x40 && opcode <= 0x7f) {
    if (opcode == 0x76) {
      state = HALTED;
      return 4;
    }
    int dst = (opcode >> 3) & 0x07;
    int src = opcode & 0x07;
    write_r(dst, read_r(src));
    af.bytes.l &= 0xf0;
    return (dst == 6 || src == 6) ? 8 : 4;
  }

  if (opcode >= 0x80 && opcode <= 0xbf) {
    int src = opcode & 0x07;
    byte value = read_r(src);
    switch ((opcode >> 3) & 0x07) {
    case 0: add_a(value); break;
    case 1: adc_a(value); break;
    case 2: sub_a(value); break;
    case 3: sbc_a(value); break;
    case 4: and_a(value); break;
    case 5: xor_a(value); break;
    case 6: or_a(value); break;
    case 7: cp_a(value); break;
    }
    af.bytes.l &= 0xf0;
    return (src == 6) ? 8 : 4;
  }

  switch (opcode) {
  case 0x00: cycles = 4; break; // NOP
  case 0x10: (void)fetch_u8(); state = STOPPED; cycles = 4; break;
  case 0x76: state = HALTED; cycles = 4; break;

  case 0x01: bc.pair = fetch_u16(); cycles = 12; break;
  case 0x11: de.pair = fetch_u16(); cycles = 12; break;
  case 0x21: hl.pair = fetch_u16(); cycles = 12; break;
  case 0x31: sp = fetch_u16(); cycles = 12; break;

  case 0x02: memory_.Write(bc.pair, af.bytes.h); cycles = 8; break;
  case 0x12: memory_.Write(de.pair, af.bytes.h); cycles = 8; break;
  case 0x22: memory_.Write(hl.pair++, af.bytes.h); cycles = 8; break;
  case 0x32: memory_.Write(hl.pair--, af.bytes.h); cycles = 8; break;
  case 0x0a: af.bytes.h = memory_.Read(bc.pair); cycles = 8; break;
  case 0x1a: af.bytes.h = memory_.Read(de.pair); cycles = 8; break;
  case 0x2a: af.bytes.h = memory_.Read(hl.pair++); cycles = 8; break;
  case 0x3a: af.bytes.h = memory_.Read(hl.pair--); cycles = 8; break;

  case 0x03: ++bc.pair; cycles = 8; break;
  case 0x13: ++de.pair; cycles = 8; break;
  case 0x23: ++hl.pair; cycles = 8; break;
  case 0x33: ++sp; cycles = 8; break;
  case 0x0b: --bc.pair; cycles = 8; break;
  case 0x1b: --de.pair; cycles = 8; break;
  case 0x2b: --hl.pair; cycles = 8; break;
  case 0x3b: --sp; cycles = 8; break;

  case 0x04: bc.bytes.h = inc8(bc.bytes.h); cycles = 4; break;
  case 0x0c: bc.bytes.l = inc8(bc.bytes.l); cycles = 4; break;
  case 0x14: de.bytes.h = inc8(de.bytes.h); cycles = 4; break;
  case 0x1c: de.bytes.l = inc8(de.bytes.l); cycles = 4; break;
  case 0x24: hl.bytes.h = inc8(hl.bytes.h); cycles = 4; break;
  case 0x2c: hl.bytes.l = inc8(hl.bytes.l); cycles = 4; break;
  case 0x34: memory_.Write(hl.pair, inc8(memory_.Read(hl.pair))); cycles = 12; break;
  case 0x3c: af.bytes.h = inc8(af.bytes.h); cycles = 4; break;

  case 0x05: bc.bytes.h = dec8(bc.bytes.h); cycles = 4; break;
  case 0x0d: bc.bytes.l = dec8(bc.bytes.l); cycles = 4; break;
  case 0x15: de.bytes.h = dec8(de.bytes.h); cycles = 4; break;
  case 0x1d: de.bytes.l = dec8(de.bytes.l); cycles = 4; break;
  case 0x25: hl.bytes.h = dec8(hl.bytes.h); cycles = 4; break;
  case 0x2d: hl.bytes.l = dec8(hl.bytes.l); cycles = 4; break;
  case 0x35: memory_.Write(hl.pair, dec8(memory_.Read(hl.pair))); cycles = 12; break;
  case 0x3d: af.bytes.h = dec8(af.bytes.h); cycles = 4; break;

  case 0x06: bc.bytes.h = fetch_u8(); cycles = 8; break;
  case 0x0e: bc.bytes.l = fetch_u8(); cycles = 8; break;
  case 0x16: de.bytes.h = fetch_u8(); cycles = 8; break;
  case 0x1e: de.bytes.l = fetch_u8(); cycles = 8; break;
  case 0x26: hl.bytes.h = fetch_u8(); cycles = 8; break;
  case 0x2e: hl.bytes.l = fetch_u8(); cycles = 8; break;
  case 0x36: memory_.Write(hl.pair, fetch_u8()); cycles = 12; break;
  case 0x3e: af.bytes.h = fetch_u8(); cycles = 8; break;

  case 0x07: {
    byte old = af.bytes.h;
    af.bytes.h = static_cast<byte>((old << 1) | (old >> 7));
    set_znhc(false, false, false, bit(old, 7));
    cycles = 4;
    break;
  }
  case 0x17: {
    byte old = af.bytes.h;
    byte carry = get_f(C_FLAG) ? 1 : 0;
    af.bytes.h = static_cast<byte>((old << 1) | carry);
    set_znhc(false, false, false, bit(old, 7));
    cycles = 4;
    break;
  }
  case 0x0f: {
    byte old = af.bytes.h;
    af.bytes.h = static_cast<byte>((old >> 1) | (old << 7));
    set_znhc(false, false, false, bit(old, 0));
    cycles = 4;
    break;
  }
  case 0x1f: {
    byte old = af.bytes.h;
    byte carry = get_f(C_FLAG) ? 0x80 : 0;
    af.bytes.h = static_cast<byte>((old >> 1) | carry);
    set_znhc(false, false, false, bit(old, 0));
    cycles = 4;
    break;
  }

  case 0x08: {
    word addr = fetch_u16();
    memory_.Write(addr, lo(sp));
    memory_.Write(static_cast<word>(addr + 1), hi(sp));
    cycles = 20;
    break;
  }

  case 0x09: add_hl(bc.pair); cycles = 8; break;
  case 0x19: add_hl(de.pair); cycles = 8; break;
  case 0x29: add_hl(hl.pair); cycles = 8; break;
  case 0x39: add_hl(sp); cycles = 8; break;

  case 0x18: jr(true); break;
  case 0x20: jr(!get_f(Z_FLAG)); break;
  case 0x28: jr(get_f(Z_FLAG)); break;
  case 0x30: jr(!get_f(C_FLAG)); break;
  case 0x38: jr(get_f(C_FLAG)); break;

  case 0x27: { // DAA
    byte a = af.bytes.h;
    byte adjust = 0;
    bool carry = get_f(C_FLAG);

    if (!get_f(N_FLAG)) {
      if (get_f(H_FLAG) || (a & 0x0f) > 9) adjust |= 0x06;
      if (carry || a > 0x99) { adjust |= 0x60; carry = true; }
      a = static_cast<byte>(a + adjust);
    } else {
      if (get_f(H_FLAG)) adjust |= 0x06;
      if (carry) adjust |= 0x60;
      a = static_cast<byte>(a - adjust);
    }

    af.bytes.h = a;
    set_f(Z_FLAG, af.bytes.h == 0);
    set_f(H_FLAG, false);
    set_f(C_FLAG, carry);
    cycles = 4;
    break;
  }
  case 0x2f: af.bytes.h ^= 0xff; set_f(N_FLAG, true); set_f(H_FLAG, true); cycles = 4; break;
  case 0x37: set_f(N_FLAG, false); set_f(H_FLAG, false); set_f(C_FLAG, true); cycles = 4; break;
  case 0x3f: set_f(N_FLAG, false); set_f(H_FLAG, false); set_f(C_FLAG, !get_f(C_FLAG)); cycles = 4; break;

  case 0xc0: ret(!get_f(Z_FLAG)); break;
  case 0xc8: ret(get_f(Z_FLAG)); break;
  case 0xd0: ret(!get_f(C_FLAG)); break;
  case 0xd8: ret(get_f(C_FLAG)); break;
  case 0xc9: pc = popWord(); cycles = 16; break;
  case 0xd9: pc = popWord(); ime = true; cycles = 16; break;

  case 0xc1: bc.pair = popWord(); cycles = 12; break;
  case 0xd1: de.pair = popWord(); cycles = 12; break;
  case 0xe1: hl.pair = popWord(); cycles = 12; break;
  case 0xf1: af.pair = popWord(); af.bytes.l &= 0xf0; cycles = 12; break;
  case 0xc5: pushWord(bc.pair); cycles = 16; break;
  case 0xd5: pushWord(de.pair); cycles = 16; break;
  case 0xe5: pushWord(hl.pair); cycles = 16; break;
  case 0xf5: pushWord(af.pair); cycles = 16; break;

  case 0xc2: jp(!get_f(Z_FLAG)); break;
  case 0xca: jp(get_f(Z_FLAG)); break;
  case 0xd2: jp(!get_f(C_FLAG)); break;
  case 0xda: jp(get_f(C_FLAG)); break;
  case 0xc3: pc = fetch_u16(); cycles = 16; break;
  case 0xe9: pc = hl.pair; cycles = 4; break;

  case 0xc4: call(!get_f(Z_FLAG)); break;
  case 0xcc: call(get_f(Z_FLAG)); break;
  case 0xd4: call(!get_f(C_FLAG)); break;
  case 0xdc: call(get_f(C_FLAG)); break;
  case 0xcd: call(true); break;

  case 0xc6: add_a(fetch_u8()); cycles = 8; break;
  case 0xce: adc_a(fetch_u8()); cycles = 8; break;
  case 0xd6: sub_a(fetch_u8()); cycles = 8; break;
  case 0xde: sbc_a(fetch_u8()); cycles = 8; break;
  case 0xe6: and_a(fetch_u8()); cycles = 8; break;
  case 0xee: xor_a(fetch_u8()); cycles = 8; break;
  case 0xf6: or_a(fetch_u8()); cycles = 8; break;
  case 0xfe: cp_a(fetch_u8()); cycles = 8; break;

  case 0xc7: rst(0x00); break;
  case 0xcf: rst(0x08); break;
  case 0xd7: rst(0x10); break;
  case 0xdf: rst(0x18); break;
  case 0xe7: rst(0x20); break;
  case 0xef: rst(0x28); break;
  case 0xf7: rst(0x30); break;
  case 0xff: rst(0x38); break;

  case 0xe0: memory_.Write(static_cast<word>(0xff00 + fetch_u8()), af.bytes.h); cycles = 12; break;
  case 0xf0: af.bytes.h = memory_.Read(static_cast<word>(0xff00 + fetch_u8())); cycles = 12; break;
  case 0xe2: memory_.Write(static_cast<word>(0xff00 + bc.bytes.l), af.bytes.h); cycles = 8; break;
  case 0xf2: af.bytes.h = memory_.Read(static_cast<word>(0xff00 + bc.bytes.l)); cycles = 8; break;
  case 0xea: { word addr = fetch_u16(); memory_.Write(addr, af.bytes.h); cycles = 16; break; }
  case 0xfa: { word addr = fetch_u16(); af.bytes.h = memory_.Read(addr); cycles = 16; break; }

  case 0xe8: sp = add_sp_e8(static_cast<std::int8_t>(fetch_u8())); cycles = 16; break;
  case 0xf8: hl.pair = add_sp_e8(static_cast<std::int8_t>(fetch_u8())); cycles = 12; break;
  case 0xf9: sp = hl.pair; cycles = 8; break;

  case 0xf3: ime = false; cycles = 4; break;
  case 0xfb: ime = true; cycles = 4; break; // delayed EI can be added later

  case 0xcb: {
    byte cb = fetch_u8();
    int x = cb >> 6;
    int y = (cb >> 3) & 0x07;
    int z = cb & 0x07;
    byte value = read_r(z);

    auto cb_write = [&](byte result) {
      write_r(z, result);
      cycles = (z == 6) ? 16 : 8;
    };

    if (x == 0) {
      byte result = value;
      bool carry = false;
      switch (y) {
      case 0: carry = bit(value, 7); result = static_cast<byte>((value << 1) | (value >> 7)); break; // RLC
      case 1: carry = bit(value, 0); result = static_cast<byte>((value >> 1) | (value << 7)); break; // RRC
      case 2: carry = bit(value, 7); result = static_cast<byte>((value << 1) | (get_f(C_FLAG) ? 1 : 0)); break; // RL
      case 3: carry = bit(value, 0); result = static_cast<byte>((value >> 1) | (get_f(C_FLAG) ? 0x80 : 0)); break; // RR
      case 4: carry = bit(value, 7); result = static_cast<byte>(value << 1); break; // SLA
      case 5: carry = bit(value, 0); result = static_cast<byte>((value >> 1) | (value & 0x80)); break; // SRA
      case 6: result = static_cast<byte>((value << 4) | (value >> 4)); carry = false; break; // SWAP
      case 7: carry = bit(value, 0); result = static_cast<byte>(value >> 1); break; // SRL
      }
      cb_write(result);
      set_znhc(result == 0, false, false, carry);
    } else if (x == 1) { // BIT
      set_f(Z_FLAG, !bit(value, y));
      set_f(N_FLAG, false);
      set_f(H_FLAG, true);
      cycles = (z == 6) ? 12 : 8;
    } else if (x == 2) { // RES
      byte result = static_cast<byte>(value & ~(1u << y));
      cb_write(result);
    } else { // SET
      byte result = static_cast<byte>(value | (1u << y));
      cb_write(result);
    }
    break;
  }

  // Illegal opcodes on LR35902. Stop loudly instead of executing garbage.
  case 0xd3: case 0xdb: case 0xdd: case 0xe3: case 0xe4:
  case 0xeb: case 0xec: case 0xed: case 0xf4: case 0xfc: case 0xfd:
    std::cerr << "[CPU] Illegal opcode 0x" << std::hex << std::setw(2)
              << std::setfill('0') << static_cast<int>(opcode)
              << " at PC=0x" << std::setw(4) << opcode_pc << std::dec << '\n';
    state = STOPPED;
    cycles = 4;
    break;

  default:
    std::cerr << "[CPU] Unimplemented opcode 0x" << std::hex << std::setw(2)
              << std::setfill('0') << static_cast<int>(opcode)
              << " at PC=0x" << std::setw(4) << opcode_pc << std::dec << '\n';
    state = STOPPED;
    cycles = 4;
    break;
  }

  af.bytes.l &= 0xf0;
  return cycles;
}

} // namespace lmgb
