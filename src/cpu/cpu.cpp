#include "cpu.h"

lmgb::Cpu::Cpu() {
  // initializing registers
  af.pair = 0x11;
  af.bytes.l = 0xb0;
  bc.pair = 0x0013;
  de.pair = 0x00d8;
  hl.pair = 0x014d;
  sp = 0xfffe;
  pc = 0x100;

  // initializing RAM
  mem.Write(0xff05, 0x00);
  mem.Write(0xff06, 0x00);
  mem.Write(0xff07, 0x00);
  mem.Write(0xff10, 0x80);
  mem.Write(0xff11, 0xbf);
  mem.Write(0xff12, 0xf3);
  mem.Write(0xff14, 0xbf);
  mem.Write(0xff16, 0x3f);
  mem.Write(0xff17, 0x00);
  mem.Write(0xff19, 0xbf);
  mem.Write(0xff1a, 0x7f);
  mem.Write(0xff1b, 0xff);
  mem.Write(0xff1c, 0x9f);
  mem.Write(0xff1e, 0xbf);
  mem.Write(0xff20, 0xff);
  mem.Write(0xff21, 0x00);
  mem.Write(0xff22, 0x00);
  mem.Write(0xff23, 0xbf);
  mem.Write(0xff24, 0x77);
  mem.Write(0xff25, 0xf3);
  mem.Write(0xff26, 0xf1);
  mem.Write(0xff40, 0x91);
  mem.Write(0xff42, 0x00);
  mem.Write(0xff43, 0x00);
  mem.Write(0xff45, 0x00);
  mem.Write(0xff47, 0xfc);
  mem.Write(0xff48, 0xff);
  mem.Write(0xff49, 0xff);
  mem.Write(0xff4a, 0x00);
  mem.Write(0xff4b, 0x00);
  mem.Write(0xffff, 0x00);
}

byte lmgb::Cpu::readOp(word &pc) {
  byte opcode = mem.Read(pc);
  pc++;
  return opcode;
}

void lmgb::Cpu::Step() {
  byte opcode = readOp(pc);
  char cycles = 0;

  switch (opcode) {
  // LD nn,n
  case 0x06:
    bc.bytes.h = mem.Read(pc++);
    cycles = 8;
    break;
  case 0x0e:
    bc.bytes.l = mem.Read(pc++);
    cycles = 8;
    break;
  case 0x16:
    de.bytes.h = mem.Read(pc++);
    cycles = 8;
    break;
  case 0x1e:
    de.bytes.l = mem.Read(pc++);
    cycles = 8;
    break;
  case 0x26:
    hl.bytes.h = mem.Read(pc++);
    cycles = 8;
    break;
  case 0x2e:
    hl.bytes.l = mem.Read(pc++);
    cycles = 8;
    break;

  // LD r1,r2
  case 0x7f:
    af.bytes.h = af.bytes.h;
    cycles = 4;
    break;
  case 0x78:
    af.bytes.h = bc.bytes.h;
    cycles = 4;
    break;
  case 0x79:
    af.bytes.h = bc.bytes.l;
    cycles = 4;
    break;
  case 0x7a:
    af.bytes.h = de.bytes.h;
    cycles = 4;
    break;
  case 0x7b:
    af.bytes.h = de.bytes.l;
    cycles = 4;
    break;
  case 0x7c:
    af.bytes.h = hl.bytes.h;
    cycles = 4;
    break;
  case 0x7d:
    af.bytes.h = hl.bytes.l;
    cycles = 4;
    break;
  case 0x7e:
    af.bytes.h = mem.Read(hl.pair);
    cycles = 8;
    break;
  case 0x40:
    bc.bytes.h = bc.bytes.h;
    cycles = 4;
    break;
  case 0x41:
    bc.bytes.h = bc.bytes.l;
    cycles = 4;
    break;
  case 0x42:
    bc.bytes.h = de.bytes.h;
    cycles = 4;
    break;
  case 0x43:
    bc.bytes.h = de.bytes.l;
    cycles = 4;
    break;
  case 0x44:
    bc.bytes.h = hl.bytes.h;
    cycles = 4;
    break;
  case 0x45:
    bc.bytes.h = hl.bytes.l;
    cycles = 5;
    break;
  case 0x46:
    bc.bytes.h = mem.Read(hl.pair);
    cycles = 8;
    break;
  case 0x48:
    bc.bytes.l = bc.bytes.h;
    cycles = 4;
    break;
  case 0x49:
    bc.bytes.l = bc.bytes.l;
    cycles = 4;
    break;
  case 0x4a:
    bc.bytes.l = de.bytes.h;
    cycles = 4;
    break;
  case 0x4b:
    bc.bytes.l = de.bytes.l;
    cycles = 4;
    break;
  case 0x4c:
    bc.bytes.l = hl.bytes.h;
    cycles = 4;
    break;
  case 0x4d:
    bc.bytes.l = hl.bytes.l;
    cycles = 4;
    break;
  case 0x4e:
    bc.bytes.l = mem.Read(hl.pair);
    cycles = 8;
    break;
  case 0x50:
    de.bytes.h = bc.bytes.h;
    cycles = 4;
    break;
  case 0x51:
    de.bytes.h = bc.bytes.l;
    cycles = 4;
    break;
  case 0x52:
    de.bytes.h = de.bytes.h;
    cycles = 4;
    break;
  case 0x53:
    de.bytes.h = de.bytes.l;
    cycles = 4;
    break;
  case 0x54:
    de.bytes.h = hl.bytes.h;
    cycles = 4;
    break;
  case 0x55:
    de.bytes.h = hl.bytes.l;
    cycles = 4;
    break;
  case 0x56:
    de.bytes.h = mem.Read(hl.pair);
    cycles = 8;
    break;
  case 0x58:
    de.bytes.l = bc.bytes.h;
    cycles = 4;
    break;
  case 0x59:
    de.bytes.l = bc.bytes.l;
    cycles = 4;
    break;
  case 0x5a:
    de.bytes.l = de.bytes.h;
    cycles = 4;
    break;
  case 0x5b:
    de.bytes.l = de.bytes.l;
    cycles = 4;
    break;
  case 0x5c:
    de.bytes.l = hl.bytes.h;
    cycles = 4;
    break;
  case 0x5d:
    de.bytes.l = hl.bytes.l;
    cycles = 4;
    break;
  case 0x5e:
    de.bytes.l = mem.Read(hl.pair);
    cycles = 8;
    break;
  case 0x60:
    hl.bytes.h = bc.bytes.h;
    cycles = 4;
    break;
  case 0x61:
    hl.bytes.h = bc.bytes.l;
    cycles = 4;
    break;
  case 0x62:
    hl.bytes.h = de.bytes.h;
    cycles = 4;
    break;
  case 0x63:
    hl.bytes.h = de.bytes.l;
    cycles = 4;
    break;
  case 0x64:
    hl.bytes.h = hl.bytes.h;
    cycles = 4;
    break;
  case 0x65:
    hl.bytes.h = hl.bytes.l;
    cycles = 4;
    break;
  case 0x66:
    hl.bytes.h = mem.Read(hl.pair);
    cycles = 8;
    break;
  case 0x68:
    hl.bytes.l = bc.bytes.h;
    cycles = 4;
    break;
  case 0x69:
    hl.bytes.l = bc.bytes.l;
    cycles = 4;
    break;
  case 0x6a:
    hl.bytes.l = de.bytes.h;
    cycles = 4;
    break;
  case 0x6b:
    hl.bytes.l = de.bytes.l;
    cycles = 4;
    break;
  case 0x6c:
    hl.bytes.l = hl.bytes.h;
    cycles = 4;
    break;
  case 0x6d:
    hl.bytes.l = hl.bytes.l;
    cycles = 4;
    break;
  case 0x6e:
    hl.bytes.l = mem.Read(hl.pair);
    cycles = 8;
    break;
  case 0x70:
    mem.Write(hl.pair, bc.bytes.h);
    cycles = 8;
    break;
  case 0x71:
    mem.Write(hl.pair, bc.bytes.l);
    cycles = 8;
    break;
  case 0x72:
    mem.Write(hl.pair, de.bytes.h);
    cycles = 8;
    break;
  case 0x73:
    mem.Write(hl.pair, de.bytes.l);
    cycles = 8;
    break;
  case 0x74:
    mem.Write(hl.pair, hl.bytes.h);
    cycles = 8;
    break;
  case 0x75:
    mem.Write(hl.pair, hl.bytes.l);
    cycles = 8;
    break;
  case 0x36:
    byte val = mem.Read(pc);
    mem.Write(hl.pair, val);
    cycles = 12;
    break;

  // LD A,n
  case 0x0a:
    af.bytes.h = mem.Read(bc.pair);
    cycles = 8;
    break;
  case 0x1a:
    af.bytes.h = mem.Read(de.pair);
    cycles = 8;
    break;
  case 0xfa:
    word val = btow(mem.Read(pc), mem.Read(pc + 1));
    af.bytes.h = mem.Read(val);
    cycles = 16;
    break;
  case 0x3e:
    af.bytes.h = mem.Read(pc);
    cycles = 8;
    break;

  // LD A,(C)
  // LD (C),A
  case 0xf2:
    af.bytes.h = mem.Read(0xff00 + bc.bytes.l);
    cycles = 8;
    break;
  case 0xe2:
    mem.Write(0xff00 + bc.bytes.l, af.bytes.h);
    cycles = 8;
    break;

  // LD A,(HLD)
  // LD A,(HL-)
  // LDD A,(HL)
  case 0x3a:
    af.bytes.h = mem.Read(hl.pair);
    hl.pair--;
    cycles = 8;
    break;

  // LD (HLD),A
  // LD (HL-),A
  // LDD (HL),A
  case 0x32:
    mem.Write(hl.pair, af.bytes.h);
    hl.pair--;
    cycles = 8;
    break;

  // LD A,(HLI)
  // LD A,(HL+)
  // LDI A,(HL)
  case 0x2a:
    af.bytes.h = mem.Read(hl.pair);
    hl.pair++;
    cycles = 8;
    break;

  // LD (HLI),A
  // LD (HL+),A
  // LDI (HL),A
  case 0x22:
    mem.Write(hl.pair, af.bytes.h);
    hl.pair++;
    cycles = 8;
    break;

  // LDH (n),A
  case 0xe0:
    byte memAddr = mem.Read(pc++);
    mem.Write(0xff00 + memAddr, af.bytes.h);
    cycles = 12;
    break;

  // LDH A,(n)
  case 0xf0:
    byte memAddr = mem.Read(pc++);
    af.bytes.h = mem.Read(0xff00 + memAddr);
    cycles = 12;
    break;

  // LD n,nn
  case 0x01:
    bc.pair = btow(mem.Read(pc++), mem.Read(pc++));
    cycles = 12;
    break;
  case 0x11:
    de.pair = btow(mem.Read(pc++), mem.Read(pc++));
    cycles = 12;
    break;
  case 0x21:
    hl.pair = btow(mem.Read(pc++), mem.Read(pc++));
    cycles = 12;
    break;
  case 0x31:
    sp = btow(mem.Read(pc++), mem.Read(pc++));
    cycles = 12;
    break;

  // LD SP,HL
  case 0xf9:
    sp = hl.pair;
    cycles = 8;
    break;

  // LDHL SP,n
  case 0xf8:
    int8_t n = mem.Read(pc++);
    hl.pair = sp + n;
    af.bytes.l &= 0x00;
    if (HALF_CARRY_FLAG(sp, n))
      af.bytes.l |= 0x20;
    if (CARRY_FLAG(sp, n))
      af.bytes.l |= 0x10;
    cycles = 12;
    break;

  // LD (nn),SP
  case 0x08:
    word addr = mem.Read(pc++);
    mem.Write(addr, sp);
    cycles = 20;
    break;

  // PUSH nn
  case 0xf5:
    mem.Write(--sp, af.bytes.h);
    mem.Write(--sp, af.bytes.l);
    cycles = 16;
    break;
  case 0xc5:
    mem.Write(--sp, bc.bytes.h);
    mem.Write(--sp, bc.bytes.l);
    cycles = 16;
    break;
  case 0xd5:
    mem.Write(--sp, de.bytes.h);
    mem.Write(--sp, de.bytes.l);
    cycles = 16;
    break;
  case 0xe5:
    mem.Write(--sp, hl.bytes.h);
    mem.Write(--sp, hl.bytes.l);
    cycles = 16;
    break;

  // POP nn
  case 0xf1:
    af.bytes.l = mem.Read(sp++);
    af.bytes.h = mem.Read(sp++);
    cycles = 12;
    break;
  case 0xc1:
    bc.bytes.l = mem.Read(sp++);
    bc.bytes.h = mem.Read(sp++);
    cycles = 12;
    break;
  case 0xd1:
    de.bytes.l = mem.Read(sp++);
    de.bytes.h = mem.Read(sp++);
    cycles = 12;
    break;
  case 0xe1:
    hl.bytes.l = mem.Read(sp++);
    hl.bytes.h = mem.Read(sp++);
    cycles = 12;
    break;
  }
}
