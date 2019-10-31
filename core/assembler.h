#pragma once
#include <string>
#include <vector>
#include <array>

#include "global.h"

#define smltab "\t"

class assembler {
  std::string _mop(u16 addr) { return "[" + hexs(addr) + "]"; }
  std::string _sop(s8  sign) { return "$" + hexs((u8)sign) + " (" + std::to_string(sign) + ")"; }
  std::string _iop(u16 imm)  { return "$" + hexs(imm); }

  memory* m;

  #define posi(m, t) posi_##m##_##t
  #define posi_ionly_ "$" + hexs(idx) + ": " + hexs(instruction)
  #define posi_io8_ "$" + hexs(idx) + ": " + hexs(instruction) + hexs(imm8)
  #define posi_io16_ "$" + hexs(idx) + ": " + hexs(instruction) + hexs(imm16m)
  #define posi_ionly_t idx += 1
  #define posi_io8_t idx += 2
  #define posi_io16_t idx += 3

public:
  assembler(memory& rMemory) { this->m = &rMemory; }

  std::vector <std::string> disassemble(u16 begin, u16 end) {
    std::vector <std::string> rt;
    u16 idx = begin;
    while (idx <= end) {
      #define rw(addr) ((r(addr)) | (r(addr+1) << 8))
      auto r = [&](u16 addr) { return (u8)this->m->rb(addr); };
      u16 opcode      = rw(idx),
          imm16       = rw(idx+1),
          imm16m      = imm16;
      u8  instruction = opcode&0xff,
          imm8        = imm16&0xff;
      imm16m = (imm8 << 8) | ((imm16m & 0xff00) >> 8);

      #define instruction0(m, op, id        ) case op: rt.push_back((posi(m,) + smltab + id)); posi(m,t); break
      #define instruction1(m, op, id, o1    ) case op: rt.push_back((posi(m,) + smltab + id + smltab + o1)); posi(m,t); break
      #define instruction2(m, op, id, o1, o2) case op: rt.push_back((posi(m,) + smltab + id + smltab + o1 + "," + smltab + o2)); posi(m,t); break

      switch(instruction) {
        // Sets
        instruction2(io16 , 0x00, "ld"   , "x", _iop(imm16));
        instruction2(io16 , 0x01, "ld"   , "y", _iop(imm16));
        instruction2(io16 , 0x02, "ld"   , "x", _mop(imm16));
        instruction2(io16 , 0x03, "ld"   , "y", _mop(imm16));
        instruction2(io16 , 0x04, "ld"   , _mop(imm16), "x");
        instruction2(io16 , 0x05, "ld"   , _mop(imm16), "y");
        instruction2(ionly, 0x06, "ld"   , "x", "y");
        instruction2(ionly, 0x07, "ld"   , "y", "x");
        instruction2(ionly, 0x08, "ld"   , "[x]", "[y]");
        instruction2(ionly, 0x09, "ld"   , "[y]", "[x]");
        instruction2(io16 , 0x0a, "ld"   , "sp", _iop(imm16));
        instruction2(ionly, 0x0b, "ld"   , "sp", "x");
        instruction2(ionly, 0x0c, "ld"   , "sp", "y");
        instruction2(ionly, 0x0d, "ld"   , "sp", "[x]");
        instruction2(ionly, 0x0e, "ld"   , "sp", "[y]");
        instruction2(io16 , 0x0f, "ld"   , "sp", _mop(imm16));

        // ALU: Basic arithmetic
        instruction2(io16 , 0x10, "addx" , "x", _iop(imm16));
        instruction2(io16 , 0x11, "addx" , "y", _iop(imm16));
        instruction2(ionly, 0x12, "addx" , "x", "y");
        instruction2(ionly, 0x13, "addx" , "y", "x");
        instruction2(io16 , 0x14, "addy" , "x", _iop(imm16));
        instruction2(io16 , 0x15, "addy" , "y", _iop(imm16));
        instruction2(ionly, 0x16, "addy" , "x", "y");
        instruction2(ionly, 0x17, "addy" , "y", "x");
        instruction2(io16 , 0x20, "subx" , "x", _iop(imm16));
        instruction2(io16 , 0x21, "subx" , "y", _iop(imm16));
        instruction2(ionly, 0x22, "subx" , "x", "y");
        instruction2(ionly, 0x23, "subx" , "y", "x");
        instruction2(io16 , 0x24, "suby" , "x", _iop(imm16));
        instruction2(io16 , 0x25, "suby" , "y", _iop(imm16));
        instruction2(ionly, 0x26, "suby" , "x", "y");
        instruction2(ionly, 0x27, "suby" , "y", "x");
        instruction2(io16 , 0x30, "mulx" , "x", _iop(imm16));
        instruction2(io16 , 0x31, "mulx" , "y", _iop(imm16));
        instruction2(ionly, 0x32, "mulx" , "x", "y");
        instruction2(ionly, 0x33, "mulx" , "y", "x");
        instruction2(io16 , 0x34, "muly" , "x", _iop(imm16));
        instruction2(io16 , 0x35, "muly" , "y", _iop(imm16));
        instruction2(ionly, 0x36, "muly" , "x", "y");
        instruction2(ionly, 0x37, "muly" , "y", "x");
        instruction2(io16 , 0x40, "divx" , "x", _iop(imm16));
        instruction2(io16 , 0x41, "divx" , "y", _iop(imm16));
        instruction2(ionly, 0x42, "divx" , "x", "y");
        instruction2(ionly, 0x43, "divx" , "y", "x");
        instruction2(io16 , 0x44, "divy" , "x", _iop(imm16));
        instruction2(io16 , 0x45, "divy" , "y", _iop(imm16));
        instruction2(ionly, 0x46, "divy" , "x", "y");
        instruction2(ionly, 0x47, "divy" , "y", "x");

        instruction0(ionly, 0x50, "ix");
        instruction0(ionly, 0x51, "dx");
        instruction0(ionly, 0x52, "iy");
        instruction0(ionly, 0x53, "dy");
        instruction1(io16 , 0x54, "im"   , _mop(imm16));
        instruction1(io16 , 0x55, "dm"   , _mop(imm16));
        instruction1(io16 , 0x56, "im"   , "[x]");
        instruction1(io16 , 0x57, "dm"   , "[x]");
        instruction1(io16 , 0x58, "im"   , "[y]");
        instruction1(io16 , 0x59, "dm"   , "[y]");

        // ALU: Logic
        instruction2(io16 , 0x60, "andx" , "x", _iop(imm16));
        instruction2(io16 , 0x61, "andx" , "y", _iop(imm16));
        instruction2(ionly, 0x62, "andx" , "x", "y");
        instruction2(ionly, 0x63, "andx" , "y", "x");
        instruction2(io16 , 0x64, "andy" , "x", _iop(imm16));
        instruction2(io16 , 0x65, "andy" , "y", _iop(imm16));
        instruction2(ionly, 0x66, "andy" , "x", "y");
        instruction2(ionly, 0x67, "andy" , "y", "x");
        instruction2(io16 , 0x70, "orx"  , "x", _iop(imm16));
        instruction2(io16 , 0x71, "orx"  , "y", _iop(imm16));
        instruction2(ionly, 0x72, "orx"  , "x", "y");
        instruction2(ionly, 0x73, "orx"  , "y", "x");
        instruction2(io16 , 0x74, "ory"  , "x", _iop(imm16));
        instruction2(io16 , 0x75, "ory"  , "y", _iop(imm16));
        instruction2(ionly, 0x76, "ory"  , "x", "y");
        instruction2(ionly, 0x77, "ory"  , "y", "x");
        instruction2(io16 , 0x80, "andx" , "x", _iop(imm16));
        instruction2(io16 , 0x81, "andx" , "y", _iop(imm16));
        instruction2(ionly, 0x82, "andx" , "x", "y");
        instruction2(ionly, 0x83, "andx" , "y", "x");
        instruction2(io16 , 0x84, "andy" , "x", _iop(imm16));
        instruction2(io16 , 0x85, "andy" , "y", _iop(imm16));
        instruction2(ionly, 0x86, "andy" , "x", "y");
        instruction2(ionly, 0x87, "andy" , "y", "x");

        // ALU: Resets
        instruction0(ionly, 0x88, "rstx");
        instruction0(ionly, 0x89, "rsty");
        instruction1(io16 , 0x8a, "rst"  , _mop(imm16));
        instruction1(ionly, 0x8b, "rst"  , "[x]");
        instruction1(ionly, 0x8c, "rst"  , "[y]");

        // Calls & Return
        instruction1(io16 , 0x90, "call" , _mop(imm16));
        instruction1(ionly, 0x91, "call" , "[x]");
        instruction1(ionly, 0x92, "call" , "[y]");
        instruction0(ionly, 0x93, "ret");

        // Unconditioned jumps
        instruction1(io16 , 0xa0, "jp"   , _mop(imm16));
        instruction1(ionly, 0xa1, "jp"   , "[x]");
        instruction1(ionly, 0xa2, "jp"   , "[y]");

        // Conditioned jumps
        instruction1(io16 , 0xa3, "jo"   , _mop(imm16));
        instruction1(ionly, 0xa4, "jo"   , "[x]");
        instruction1(ionly, 0xa5, "jo"   , "[y]");
        instruction1(io16 , 0xa6, "jz"   , _mop(imm16));
        instruction1(ionly, 0xa7, "jz"   , "[x]");
        instruction1(ionly, 0xa8, "jz"   , "[y]");
        instruction1(io16 , 0xa9, "jc"   , _mop(imm16));
        instruction1(ionly, 0xaa, "jc"   , "[x]");
        instruction1(ionly, 0xab, "jc"   , "[y]");
        instruction1(io16 , 0xac, "jno"  , _mop(imm16));
        instruction1(ionly, 0xad, "jno"  , "[x]");
        instruction1(ionly, 0xae, "jno"  , "[y]");
        instruction1(io16 , 0xaf, "jnz"  , _mop(imm16));
        instruction1(ionly, 0xb0, "jnz"  , "[x]");
        instruction1(ionly, 0xb1, "jnz"  , "[y]");
        instruction1(io16 , 0xb2, "jnc"  , _mop(imm16));
        instruction1(ionly, 0xb3, "jnc"  , "[x]");
        instruction1(ionly, 0xb4, "jnc"  , "[y]");

        // Unconditioned branches
        instruction1(io16 , 0xb5, "brn"  , _sop(imm8));
        instruction1(ionly, 0xb6, "brn"  , "[x]");
        instruction1(ionly, 0xb7, "brn"  , "[y]");

        // Conditioned branches
        instruction1(io8  , 0xb8, "bo"   , _sop(imm8));
        instruction1(ionly, 0xb9, "bo"   , "[x]");
        instruction1(ionly, 0xba, "bo"   , "[y]");
        instruction1(io8  , 0xbb, "bz"   , _sop(imm8));
        instruction1(ionly, 0xbc, "bz"   , "[x]");
        instruction1(ionly, 0xbd, "bz"   , "[y]");
        instruction1(io8  , 0xbe, "bc"   , _sop(imm8));
        instruction1(ionly, 0xbf, "bc"   , "[x]");
        instruction1(ionly, 0xc0, "bc"   , "[y]");
        instruction1(io8  , 0xc1, "bo"   , _sop(imm8));
        instruction1(ionly, 0xc2, "bno"  , "[x]");
        instruction1(ionly, 0xc3, "bno"  , "[y]");
        instruction1(io8  , 0xc4, "bnz"  , _sop(imm8));
        instruction1(ionly, 0xc5, "bnz"  , "[x]");
        instruction1(ionly, 0xc6, "bnz"  , "[y]");
        instruction1(io8  , 0xc7, "bnc"  , _sop(imm8));
        instruction1(ionly, 0xc8, "bnc"  , "[x]");
        instruction1(ionly, 0xc9, "bnc"  , "[y]");

        // Stack operations
        instruction1(io16 , 0xe0, "push" , _iop(imm16));
        instruction1(io16 , 0xe1, "push" , _mop(imm16));
        instruction1(ionly, 0xe2, "push" , "[x]");
        instruction1(ionly, 0xe3, "push" , "[y]");
        instruction0(ionly, 0xe4, "pushx");
        instruction0(ionly, 0xe5, "pushy");
        instruction0(ionly, 0xe6, "push" );
        instruction0(ionly, 0xe7, "popx" );
        instruction0(ionly, 0xe8, "popy" );
        instruction0(ionly, 0xe9, "pop"  );

        // Misc.
        instruction0(ionly, 0xfe, "nop"  );
        instruction0(ionly, 0xff, "brk"  );
        default: rt.push_back((posi(ionly,) + "\t<unknown opcode>")); posi(ionly,t); break;
      }
    }
    return rt;
  }
};
