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
    if (HF_CHECK(sp, n))
      af.bytes.l |= 0x20;
    if (CF_CHECK(sp, n))
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

  // ADD A,n
  case 0x87:
    HF_CHECK(af.bytes.h, af.bytes.h) ? HF_SET(af.bytes.l)
                                     : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, af.bytes.h) ? CF_SET(af.bytes.l)
                                     : CF_RESET(af.bytes.l);
    af.bytes.h += af.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x80:
    HF_CHECK(af.bytes.h, bc.bytes.h) ? HF_SET(af.bytes.l)
                                     : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, bc.bytes.h) ? CF_SET(af.bytes.l)
                                     : CF_RESET(af.bytes.l);
    af.bytes.h += bc.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x81:
    HF_CHECK(af.bytes.h, bc.bytes.l) ? HF_SET(af.bytes.l)
                                     : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, bc.bytes.l) ? CF_SET(af.bytes.l)
                                     : CF_RESET(af.bytes.l);
    af.bytes.h += bc.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x82:
    HF_CHECK(af.bytes.h, de.bytes.h) ? HF_SET(af.bytes.l)
                                     : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, de.bytes.h) ? CF_SET(af.bytes.l)
                                     : CF_RESET(af.bytes.l);
    af.bytes.h += de.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x83:
    HF_CHECK(af.bytes.h, de.bytes.l) ? HF_SET(af.bytes.l)
                                     : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, de.bytes.l) ? CF_SET(af.bytes.l)
                                     : CF_RESET(af.bytes.l);
    af.bytes.h += de.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x84:
    HF_CHECK(af.bytes.h, hl.bytes.h) ? HF_SET(af.bytes.l)
                                     : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, hl.bytes.h) ? CF_SET(af.bytes.l)
                                     : CF_RESET(af.bytes.l);
    af.bytes.h += hl.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x85:
    HF_CHECK(af.bytes.h, hl.bytes.l) ? HF_SET(af.bytes.l)
                                     : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, hl.bytes.l) ? CF_SET(af.bytes.l)
                                     : CF_RESET(af.bytes.l);
    af.bytes.h += hl.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x86:
    byte val = mem.Read(hl.pair);
    HF_CHECK(af.bytes.h, val) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, val) ? CF_SET(af.bytes.l) : CF_RESET(af.bytes.l);
    af.bytes.h += val;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 8;
    break;
  case 0xc6:
    byte val = mem.Read(pc++);
    HF_CHECK(af.bytes.h, val) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, val) ? CF_SET(af.bytes.l) : CF_RESET(af.bytes.l);
    af.bytes.h += val;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 8;
    break;

  // ADC A,n
  case 0x8f:
    byte carry = CF_GET(af.bytes.l);
    HF_CHECK(af.bytes.h, af.bytes.h + carry) ? HF_SET(af.bytes.l)
                                             : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, af.bytes.h + carry) ? CF_SET(af.bytes.l)
                                             : CF_RESET(af.bytes.l);
    af.bytes.h += af.bytes.h + carry;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x88:
    byte carry = CF_GET(af.bytes.l);
    HF_CHECK(af.bytes.h, bc.bytes.h + carry) ? HF_SET(af.bytes.l)
                                             : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, bc.bytes.h + carry) ? CF_SET(af.bytes.l)
                                             : CF_RESET(af.bytes.l);
    af.bytes.h += bc.bytes.h + carry;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x89:
    byte carry = CF_GET(af.bytes.l);
    HF_CHECK(af.bytes.h, bc.bytes.l + carry) ? HF_SET(af.bytes.l)
                                             : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, bc.bytes.l + carry) ? CF_SET(af.bytes.l)
                                             : CF_RESET(af.bytes.l);
    af.bytes.h += bc.bytes.l + carry;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x8a:
    byte carry = CF_GET(af.bytes.l);
    HF_CHECK(af.bytes.h, de.bytes.h + carry) ? HF_SET(af.bytes.l)
                                             : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, de.bytes.h + carry) ? CF_SET(af.bytes.l)
                                             : CF_RESET(af.bytes.l);
    af.bytes.h += de.bytes.h + carry;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x8b:
    byte carry = CF_GET(af.bytes.l);
    HF_CHECK(af.bytes.h, de.bytes.l + carry) ? HF_SET(af.bytes.l)
                                             : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, de.bytes.l + carry) ? CF_SET(af.bytes.l)
                                             : CF_RESET(af.bytes.l);
    af.bytes.h += de.bytes.l + carry;
    ZF_RESET(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x8c:
    byte carry = CF_GET(af.bytes.l);
    HF_CHECK(af.bytes.h, hl.bytes.h + carry) ? HF_SET(af.bytes.l)
                                             : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, hl.bytes.h + carry) ? CF_SET(af.bytes.l)
                                             : CF_RESET(af.bytes.l);
    af.bytes.h += hl.bytes.h + carry;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x8d:
    byte carry = CF_GET(af.bytes.l);
    HF_CHECK(af.bytes.h, hl.bytes.l + carry) ? HF_SET(af.bytes.l)
                                             : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, hl.bytes.l + carry) ? CF_SET(af.bytes.l)
                                             : CF_RESET(af.bytes.l);
    af.bytes.h += hl.bytes.l + carry;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x8e:
    byte carry = CF_GET(af.bytes.l);
    byte val = mem.Read(hl.pair);
    HF_CHECK(af.bytes.h, val + carry) ? HF_SET(af.bytes.l)
                                      : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, val + carry) ? CF_SET(af.bytes.l)
                                      : CF_RESET(af.bytes.l);
    af.bytes.h += val + carry;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 8;
    break;
  case 0xce:
    byte carry = CF_GET(af.bytes.l);
    byte cal = mem.Read(pc++);
    HF_CHECK(af.bytes.h, val + carry) ? HF_SET(af.bytes.l)
                                      : HF_RESET(af.bytes.l);
    CF_CHECK(af.bytes.h, val + carry) ? CF_SET(af.bytes.l)
                                      : CF_RESET(af.bytes.l);
    af.bytes.h += val + carry;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 8;
    break;

  // SUB n
  case 0x97:
    HSF_CHECK(af.bytes.h, af.bytes.h) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, af.bytes.h) ? CF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    af.bytes.h -= af.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x90:
    HSF_CHECK(af.bytes.h, bc.bytes.h) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, bc.bytes.h) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    af.bytes.h -= bc.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x91:
    HSF_CHECK(af.bytes.h, bc.bytes.l) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, bc.bytes.l) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    af.bytes.h -= bc.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x92:
    HSF_CHECK(af.bytes.h, de.bytes.h) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, de.bytes.h) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    af.bytes.h -= de.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x93:
    HSF_CHECK(af.bytes.h, de.bytes.l) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, de.bytes.l) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    af.bytes.h -= de.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x94:
    HSF_CHECK(af.bytes.h, hl.bytes.h) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, hl.bytes.h) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    af.bytes.h -= hl.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x95:
    HSF_CHECK(af.bytes.h, hl.bytes.l) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, hl.bytes.l) ? CF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    af.bytes.h -= hl.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x96:
    byte val = mem.Read(hl.pair);
    HSF_CHECK(af.bytes.h, val) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, val) ? CF_RESET(af.bytes.l) : CF_SET(af.bytes.l);
    af.bytes.h -= val;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 8;
    break;
  case 0xd6:
    byte val = mem.Read(pc++);
    HSF_CHECK(af.bytes.h, val) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, val) ? CF_RESET(af.bytes.l) : CF_SET(af.bytes.l);
    af.bytes.h -= val;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 8;
    break;

  // SBC A,n
  case 0x9f:
    byte carry = CF_GET(af.bytes.l);
    HSF_CHECK(af.bytes.h, af.bytes.h + carry) ? HF_RESET(af.bytes.l)
                                              : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, af.bytes.h + carry) ? CF_RESET(af.bytes.l)
                                              : CF_SET(af.bytes.l);
    af.bytes.h -= (af.bytes.h + carry);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x98:
    byte carry = CF_GET(af.bytes.l);
    HSF_CHECK(af.bytes.h, bc.bytes.h + carry) ? HF_RESET(af.bytes.l)
                                              : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, bc.bytes.h + carry) ? CF_RESET(af.bytes.l)
                                              : CF_SET(af.bytes.l);
    af.bytes.h -= (bc.bytes.h + carry);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x99:
    byte carry = CF_GET(af.bytes.l);
    HSF_CHECK(af.bytes.h, bc.bytes.l + carry) ? HF_RESET(af.bytes.l)
                                              : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, bc.bytes.l + carry) ? CF_RESET(af.bytes.l)
                                              : CF_SET(af.bytes.l);
    af.bytes.h -= (bc.bytes.l + carry);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x9a:
    byte carry = CF_GET(af.bytes.l);
    HSF_CHECK(af.bytes.h, de.bytes.h + carry) ? HF_RESET(af.bytes.l)
                                              : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, de.bytes.h + carry) ? CF_RESET(af.bytes.l)
                                              : CF_SET(af.bytes.l);
    af.bytes.h -= (de.bytes.h + carry);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x9b:
    byte carry = CF_GET(af.bytes.l);
    HSF_CHECK(af.bytes.h, de.bytes.l + carry) ? HF_RESET(af.bytes.l)
                                              : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, de.bytes.l + carry) ? CF_RESET(af.bytes.l)
                                              : CF_SET(af.bytes.l);
    af.bytes.h -= (de.bytes.l + carry);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x9c:
    byte carry = CF_GET(af.bytes.l);
    HSF_CHECK(af.bytes.h, hl.bytes.h + carry) ? HF_RESET(af.bytes.l)
                                              : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, hl.bytes.h + carry) ? CF_RESET(af.bytes.l)
                                              : CF_SET(af.bytes.l);
    af.bytes.h -= (hl.bytes.l + carry);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x9d:
    byte carry = CF_GET(af.bytes.l);
    HSF_CHECK(af.bytes.h, hl.bytes.l + carry) ? HF_RESET(af.bytes.l)
                                              : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, hl.bytes.l + carry) ? CF_RESET(af.bytes.l)
                                              : CF_SET(af.bytes.l);
    af.bytes.h -= (hl.bytes.l + carry);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x9e:
    byte carry = CF_GET(af.bytes.l);
    byte val = mem.Read(hl.pair);
    HSF_CHECK(af.bytes.h, val + carry) ? HF_RESET(af.bytes.l)
                                       : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, val + carry) ? CF_RESET(af.bytes.l)
                                       : CF_SET(af.bytes.l);
    af.bytes.h -= (val + carry);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 8;
    break;
  // SBC A,#
  // Opcode - ??
  // Cycles - ??

  // AND n
  case 0xa7:
    af.bytes.h = af.bytes.h && af.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa0:
    af.bytes.h = af.bytes.h && bc.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa1:
    af.bytes.h = af.bytes.h && bc.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa2:
    af.bytes.h = af.bytes.h && de.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa3:
    af.bytes.h = af.bytes.h && de.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa4:
    af.bytes.h = af.bytes.h && hl.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa5:
    af.bytes.h = af.bytes.h && hl.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa6:
    byte val = mem.Read(hl.pair);
    af.bytes.h = af.bytes.h && val;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 8;
    break;
  case 0xe6:
    byte val = mem.Read(pc++);
    af.bytes.h = af.bytes.h && val;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_SET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 8;
    break;

  // OR n
  case 0xb7:
    af.bytes.h = af.bytes.h || af.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb0:
    af.bytes.h = af.bytes.h || bc.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb1:
    af.bytes.h = af.bytes.h || bc.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb2:
    af.bytes.h = af.bytes.h || de.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb3:
    af.bytes.h = af.bytes.h || de.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb4:
    af.bytes.h = af.bytes.h || hl.bytes.h;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb5:
    af.bytes.h = af.bytes.h || hl.bytes.l;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb6:
    byte val = mem.Read(hl.pair);
    af.bytes.h = af.bytes.h || val;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 8;
    break;
  case 0xf6:
    byte val = mem.Read(pc++);
    af.bytes.h = af.bytes.h || val;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 8;
    break;

  // XOR n
  case 0xaf:
    af.bytes.h = (!af.bytes.h) != (!af.bytes.h);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa8:
    af.bytes.h = (!af.bytes.h) != (!bc.bytes.h);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xa9:
    af.bytes.h = (!af.bytes.h) != (!bc.bytes.l);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xaa:
    af.bytes.h = (!af.bytes.h) != (!de.bytes.h);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xab:
    af.bytes.h = (!af.bytes.h) != (!de.bytes.l);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xac:
    af.bytes.h = (!af.bytes.h) != (!hl.bytes.h);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xad:
    af.bytes.h = (!af.bytes.h) != (!hl.bytes.l);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0xae:
    byte val = mem.Read(hl.pair);
    af.bytes.h = (!af.bytes.h) != (!val);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 8;
    break;
  case 0xee:
    byte val = mem.Read(pc++);
    af.bytes.h = (!af.bytes.h) != (!val);
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    CF_RESET(af.bytes.l);
    cycles = 8;
    break;

  // CP n
  case 0xbf:
    HSF_CHECK(af.bytes.h, af.bytes.h) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, af.bytes.h) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    byte res = af.bytes.h - af.bytes.h;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb8:
    HSF_CHECK(af.bytes.h, bc.bytes.h) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, bc.bytes.h) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    byte res = af.bytes.h - bc.bytes.h;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0xb9:
    HSF_CHECK(af.bytes.h, bc.bytes.l) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, bc.bytes.l) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    byte res = af.bytes.h - bc.bytes.l;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0xba:
    HSF_CHECK(af.bytes.h, de.bytes.h) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, de.bytes.h) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    byte res = af.bytes.h - de.bytes.h;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0xbb:
    HSF_CHECK(af.bytes.h, de.bytes.l) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, de.bytes.l) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    byte res = af.bytes.h - de.bytes.l;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0xbc:
    HSF_CHECK(af.bytes.h, hl.bytes.h) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, hl.bytes.h) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    byte res = af.bytes.h - hl.bytes.h;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0xbd:
    HSF_CHECK(af.bytes.h, hl.bytes.l) ? HF_RESET(af.bytes.l)
                                      : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, hl.bytes.l) ? CF_RESET(af.bytes.l)
                                      : CF_SET(af.bytes.l);
    byte res = af.bytes.h - hl.bytes.l;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0xbe:
    byte val = mem.Read(hl.pair);
    HSF_CHECK(af.bytes.h, val) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, val) ? CF_RESET(af.bytes.l) : CF_SET(af.bytes.l);
    byte res = af.bytes.h - res;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 8;
    break;
  case 0xfe:
    byte val = mem.Read(pc++);
    HSF_CHECK(af.bytes.h, val) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    CSF_CHECK(af.bytes.h, val) ? CF_RESET(af.bytes.l) : CF_SET(af.bytes.l);
    byte res = af.bytes.h - res;
    ZF_CHECK(res) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 8;
    break;

  // INC n
  case 0x3c:
    HF_CHECK(af.bytes.h, 1) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    af.bytes.h++;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x04:
    HF_CHECK(bc.bytes.h, 1) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    bc.bytes.h++;
    ZF_CHECK(bc.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x0c:
    HF_CHECK(bc.bytes.l, 1) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    bc.bytes.l++;
    ZF_CHECK(bc.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x14:
    HF_CHECK(de.bytes.h, 1) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    de.bytes.h++;
    ZF_CHECK(de.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x1c:
    HF_CHECK(de.bytes.l, 1) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    de.bytes.l++;
    ZF_CHECK(de.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x24:
    HF_CHECK(hl.bytes.h, 1) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    hl.bytes.h++;
    ZF_CHECK(hl.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x2c:
    HF_CHECK(hl.bytes.l, 1) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    hl.bytes.l++;
    ZF_CHECK(hl.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 4;
    break;
  case 0x34:
    byte val = mem.Read(hl.pair);
    HF_CHECK(val, 1) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    val++;
    ZF_CHECK(val) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    mem.Write(hl.pair, val);
    cycles = 12;
    break;

  // DEC n
  case 0x3d:
    HSF_CHECK(af.bytes.h, 1) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    af.bytes.h--;
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x05:
    HSF_CHECK(bc.bytes.h, 1) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    bc.bytes.h--;
    ZF_CHECK(bc.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x0d:
    HSF_CHECK(bc.bytes.l, 1) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    bc.bytes.l--;
    ZF_CHECK(bc.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x15:
    HSF_CHECK(de.bytes.h, 1) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    de.bytes.h--;
    ZF_CHECK(de.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x1d:
    HSF_CHECK(de.bytes.l, 1) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    de.bytes.l--;
    ZF_CHECK(de.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x25:
    HSF_CHECK(hl.bytes.h, 1) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    hl.bytes.h--;
    ZF_CHECK(hl.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x2d:
    HSF_CHECK(hl.bytes.l, 1) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    hl.bytes.l--;
    ZF_CHECK(hl.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    cycles = 4;
    break;
  case 0x35:
    byte val = mem.Read(hl.pair);
    HSF_CHECK(val, 1) ? HF_RESET(af.bytes.l) : HF_SET(af.bytes.l);
    val--;
    ZF_CHECK(val) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    NF_SET(af.bytes.l);
    mem.Write(hl.pair, val);
    cycles = 12;
    break;

  // ADD HL,n
  case 0x09:
    HF_CHECK16(hl.pair, bc.pair) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    CF_CHECK16(hl.pair, bc.pair) ? CF_SET(af.bytes.l) : CF_RESET(af.bytes.l);
    hl.pair += bc.pair;
    NF_RESET(af.bytes.l);
    cycles = 8;
    break;
  case 0x19:
    HF_CHECK16(hl.pair, de.pair) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    CF_CHECK16(hl.pair, de.pair) ? CF_SET(af.bytes.l) : CF_RESET(af.bytes.l);
    hl.pair += de.pair;
    NF_RESET(af.bytes.l);
    cycles = 8;
    break;
  case 0x29:
    HF_CHECK16(hl.pair, hl.pair) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    CF_CHECK16(hl.pair, hl.pair) ? CF_SET(af.bytes.l) : CF_RESET(af.bytes.l);
    hl.pair += hl.pair;
    NF_RESET(af.bytes.l);
    cycles = 8;
    break;
  case 0x39:
    HF_CHECK16(hl.pair, sp) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    CF_CHECK16(hl.pair, sp) ? CF_SET(af.bytes.l) : CF_RESET(af.bytes.l);
    hl.pair += sp;
    NF_RESET(af.bytes.l);
    cycles = 8;
    break;

  // ADD SP,n
  case 0xe8:
    byte val = mem.Read(pc++);
    HF_CHECK(sp, val) ? HF_SET(af.bytes.l) : HF_RESET(af.bytes.l);
    CF_CHECK(sp, val) ? CF_SET(af.bytes.l) : CF_RESET(af.bytes.l);
    sp += val;
    ZF_RESET(af.bytes.l);
    NF_RESET(af.bytes.l);
    cycles = 16;
    break;

  // INC nn
  case 0x03:
    bc.pair++;
    cycles = 8;
    break;
  case 0x13:
    de.pair++;
    cycles = 8;
    break;
  case 0x23:
    hl.pair++;
    cycles = 8;
    break;
  case 0x33:
    sp++;
    cycles = 8;
    break;

  // DEC nn
  case 0x0b:
    bc.pair--;
    cycles = 8;
    break;
  case 0x1b:
    de.pair--;
    cycles = 8;
    break;
  case 0x2b:
    hl.pair--;
    cycles = 8;
    break;
  case 0x3b:
    sp--;
    cycles = 8;
    break;

  // SWAP
  case 0xcb:
    byte cbPref = mem.Read(pc++);
    switch (cbPref) {
    case 0x37:
      SWAP(af.bytes.h);
      ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
      HF_RESET(af.bytes.l);
      CF_RESET(af.bytes.l);
      NF_RESET(af.bytes.l);
      cycles = 8;
      break;
    case 0x30:
      SWAP(bc.bytes.h);
      ZF_CHECK(bc.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
      HF_RESET(af.bytes.l);
      CF_RESET(af.bytes.l);
      NF_RESET(af.bytes.l);
      cycles = 8;
      break;
    case 0x31:
      SWAP(bc.bytes.l);
      ZF_CHECK(bc.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
      HF_RESET(af.bytes.l);
      CF_RESET(af.bytes.l);
      NF_RESET(af.bytes.l);
      cycles = 8;
      break;
    case 0x32:
      SWAP(de.bytes.h);
      ZF_CHECK(de.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
      HF_RESET(af.bytes.l);
      CF_RESET(af.bytes.l);
      NF_RESET(af.bytes.l);
      cycles = 8;
      break;
    case 0x33:
      SWAP(de.bytes.l);
      ZF_CHECK(de.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
      HF_RESET(af.bytes.l);
      CF_RESET(af.bytes.l);
      NF_RESET(af.bytes.l);
      cycles = 8;
      break;
    case 0x34:
      SWAP(hl.bytes.h);
      ZF_CHECK(hl.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
      HF_RESET(af.bytes.l);
      CF_RESET(af.bytes.l);
      NF_RESET(af.bytes.l);
      cycles = 8;
      break;
    case 0x35:
      SWAP(hl.bytes.l);
      ZF_CHECK(hl.bytes.l) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
      HF_RESET(af.bytes.l);
      CF_RESET(af.bytes.l);
      NF_RESET(af.bytes.l);
      cycles = 8;
      break;
    case 0x36:
      byte val = mem.Read(pc);
      SWAP(val);
      ZF_CHECK(val) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
      HF_RESET(af.bytes.l);
      CF_RESET(af.bytes.l);
      NF_RESET(af.bytes.l);
      mem.Write(pc++, val);
      cycles = 16;
      break;
    }
    break;

  // DAA
  case 0x27:
    byte correction = 0;
    if (!NF_GET(af.bytes.l)) {
      if (HF_GET(af.bytes.l) || (af.bytes.h & 0x0f) > 0x09)
        correction |= 0x06;
      if (CF_GET(af.bytes.l) || af.bytes.h > 0x99) {
        correction |= 0x60;
        CF_SET(af.bytes.l);
      }
      af.bytes.l -= correction;
    } else {
      if (HF_GET(af.bytes.l))
        correction |= 0x06;
      if (CF_GET(af.bytes.l)) {
        correction |= 0x06;
        CF_SET(af.bytes.l);
      }
      af.bytes.h -= correction;
    }
    ZF_CHECK(af.bytes.h) ? ZF_SET(af.bytes.l) : ZF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    cycles = 4;
    break;

  // CPL
  case 0x2f:
    af.bytes.h = ~af.bytes.h;
    NF_SET(af.bytes.l);
    HF_SET(af.bytes.l);
    cycles = 4;
    break;

  // CCF
  case 0x3f:
    CF_GET(af.bytes.l) ? CF_RESET(af.bytes.l) : CF_SET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    cycles = 4;
    break;

  // SCF
  case 0x37:
    CF_SET(af.bytes.l);
    NF_RESET(af.bytes.l);
    HF_RESET(af.bytes.l);
    cycles = 4;
    break;
  }
}
