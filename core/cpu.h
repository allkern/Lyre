#pragma once
#include <utility>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "global.h"
#include "bus.h"

class cpu {
  bus& Bus = _bus_;
  std::thread* _cpuLoop = nullptr;

  const u8 of   = 0b00000001, // Overflow flag
           zf   = 0b00000010, // Zero flag
	         cf   = 0b00000100, // Carry flag
           hf   = 0b00001000, // Half carry flag
           nmif = 0b01000000, // NMI flag
           irqf = 0b10000000; // IRQ flag

  u16 x  = 0x0000, // X register
      y  = 0x0000, // Y register
      sp = 0x0000, // Stack Pointer
      pc = 0x0000; // Program Counter
  u8  z  = 0x00;   // Flags register

  u16  opcode      = 0x0000, // Fetched opcode
       imm16       = 0x0000; // Fetched immediate operand
  u8   instruction = 0x00,   // Fetched instruction
       imm8        = 0x00;   // Higher 8 bits of imm16
  bool jump        = false;  // Did we jump on this cycle?

  // Get a reference to a RAM address
  u8& address(u16 addr) { return Bus.rb(addr); }

  // Get a RAM address by value
  u16 read(u16 addr) { return Bus.rb(addr); }

  u16 rsetr(u16& a, u16& b) { a  = b; return a; }
  u16 raddr(u16& a, u16& b) { a += b; return a; }
  u16 rsubr(u16& a, u16& b) { a -= b; return a; }
  u16 rmulr(u16& a, u16& b) { a *= b; return a; }
  u16 rdivr(u16& a, u16& b) { a /= b; return a; }
  u16 randr(u16& a, u16& b) { a &= b; return a; }
  u16 rorr (u16& a, u16& b) { a |= b; return a; }
  u16 rxorr(u16& a, u16& b)	{ a ^= b; return a; }
  u16 rsetr(u8 & a, u8 & b) { a  = b; return a; }
  u16 raddr(u8 & a, u8 & b) { a += b; return a; }
  u16 rsubr(u8 & a, u8 & b) { a -= b; return a; }
  u16 rmulr(u8 & a, u8 & b) { a *= b; return a; }
  u16 rdivr(u8 & a, u8 & b) { a /= b; return a; }
  u16 randr(u8 & a, u8 & b) { a &= b; return a; }
  u16 rorr (u8 & a, u8 & b) { a |= b; return a; }
  u16 rxorr(u8 & a, u8 & b)	{ a ^= b; return a; }
  u16 rset (u16& a, u16  b) { a  = b; return a; }
  u16 radd (u16& a, u16  b) { a += b; return a; }
  u16 rsub (u16& a, u16  b) { a -= b; return a; }
  u16 rmul (u16& a, u16  b) { a *= b; return a; }
  u16 rdiv (u16& a, u16  b) { a /= b; return a; }
  u16 rand (u16& a, u16  b) { a &= b; return a; }
  u16 ror  (u16& a, u16  b) { a |= b; return a; }
  u16 rxor (u16& a, u16  b)	{ a ^= b; return a; }
  u16 rset (u8 & a, u8   b) { a  = b; return a; }
  u16 radd (u8 & a, u8   b) { a += b; return a; }
  u16 rsub (u8 & a, u8   b) { a -= b; return a; }
  u16 rmul (u8 & a, u8   b) { a *= b; return a; }
  u16 rdiv (u8 & a, u8   b) { a /= b; return a; }
  u16 rand (u8 & a, u8   b) { a &= b; return a; }
  u16 ror  (u8 & a, u8   b) { a |= b; return a; }
  u16 rxor (u8 & a, u8   b) { a ^= b; return a; }
  u16 radd2(u16& a, u16& b, u16& c) { a = b + c; return a; }
  u16 rsub2(u16& a, u16& b, u16& c) { a = b - c; return a; }
  u16 rmul2(u16& a, u16& b, u16& c) { a = b * c; return a; }
  u16 rdiv2(u16& a, u16& b, u16& c) { a = b / c; return a; }
  u16 rand2(u16& a, u16& b, u16& c) { a = b & c; return a; }
  u16 ror2 (u16& a, u16& b, u16& c) { a = b | c; return a; }
  u16 rxor2(u16& a, u16& b, u16& c) { a = b ^ c; return a; }
  u16 radds(u16& a, s8   b) { a += b; return a; }
  u16 rsubs(u16& a, s8   b) { a -= b; return a; }
  u16 rmuls(u16& a, s8   b) { a *= b; return a; }
  u16 rdivs(u16& a, s8   b) { a /= b; return a; }

  // Main CPU loop
  void loop() {
  #ifdef _CPU_LOG
    system("clear");
    _log(i, "Launched");
  #endif
    while (true) {
      this->fetch();
      this->execute();
      #ifndef _CPU_REALTIME
         std::this_thread::sleep_for(std::chrono::milliseconds((int)((1/(_CPU_FREQ))*1000)));
      #endif
    }
  }

public:
  memory& getMemory() { return static_cast<memory&>(*Bus.getDevice(0)); }

  // Default constructor: initializes the CPU thread
  cpu() = default;

  void launch() {
    this->_cpuLoop = new std::thread(&cpu::loop, this);
    this->_cpuLoop->join();
    Bus.joinAllThreads();
  }

  std::thread* getThread() { return this->_cpuLoop; }

  void fetch() {
    #define rw(addr) (read(addr) | (read(addr+1) << 8))
    opcode      = rw(pc);
    instruction = opcode & 0xff;
    imm16       = rw(pc+1);
    imm8        = imm16&0xff;
    #ifdef _CPU_LOG
      _log(d,
          "\nopcode      = $" << hex(opcode, 4) << "\n" <<
          "instruction = $" << hex(instruction, 2) << "\n" <<
          "imm16       = $" << hex(imm16, 4) << "\n" <<
          "imm8        = $" << hex(imm8, 2) << "\n"
      );
    #endif
    #undef rw
  }

  #define testFlags(c, o, e) if (this->z && c) { o; } else { e; }
  #define testFlagsNot(c, o, e) if (!(this->z && c)) { o; } else { e; }

  #define applyFlags(o) { \
    u32 t = o; \
    if ((t&0xff0000) >> 0x10) { this->z |= this->of; } else { this->z &= not(this->of); } \
    if ((t&0x000100) >> 8)    { this->z |= this->hf; } else { this->z &= not(this->hf); } \
    if ((t&0x010000) >> 0x10) { this->z |= this->cf; } else { this->z &= not(this->cf); } \
    if ((t&0xffff  ) == 0)    { this->z |= this->zf; } else { this->z &= not(this->zf); } \
  }

  #define rw(addr) (u16)((u8)address(addr) | ((u8)address(addr+1) << 8))
  #define ww(addr, r) rset(address(addr), (u8)(r&0xff)); rset(address(addr+1), (u8)((r&0xff00)>>8));

  void execute() {
    #ifdef _CPU_LOG
      _log(d, "Before exec:" <<
          "\npc = $" << hex(pc, 4) << "\n" <<
          "sp = $" << hex(sp, 4) << "\n" <<
          "x  = $" << hex(x , 4) << "\n" <<
          "y  = $" << hex(y , 4) << "\n" <<
          "z  = $" << hex(z , 2) << "\n" <<
          "jp = " << (int)jump << "\n"
      )
    #endif
    switch(instruction) {
      // Sets
      case 0x00: rsetr(x, imm16); pc += 2; break;
      case 0x01: rsetr(y, imm16); pc += 2; break;
      case 0x02: rset(x, rw(imm16)); pc += 2; break;
      case 0x03: rset(y, rw(imm16)); pc += 2; break;
      case 0x04: ww(imm16, x); pc += 2; break;
      case 0x05: ww(imm16, y); pc += 2; break;
      case 0x06: rsetr(x, y); break;
      case 0x07: rsetr(y, x); break;
      case 0x08: ww(x, y); break;
      case 0x09: ww(y, x); break;
      case 0x0a: rsetr(sp, imm16); pc += 2; break;
      case 0x0b: rsetr(sp, x); break;
      case 0x0c: rsetr(sp, y); break;
      case 0x0d: rset(sp, rw(x)); break;
      case 0x0e: rset(sp, rw(y)); break;
      case 0x0f: rset(sp, rw(imm16)); pc += 2; break;

      // ALU: Basic arithmetic
      case 0x10: applyFlags(radd2(x, x, imm16)); pc += 2; break;
      case 0x11: applyFlags(radd2(x, y, imm16)); pc += 2; break;
      case 0x12: applyFlags(radd2(x, x, y)); break;
      case 0x13: applyFlags(radd2(x, y, x)); break;
      case 0x14: applyFlags(radd2(y, x, imm16)); pc += 2; break;
      case 0x15: applyFlags(radd2(y, y, imm16)); pc += 2; break;
      case 0x16: applyFlags(radd2(y, x, y)); break;
      case 0x17: applyFlags(radd2(y, y, x)); break;
      case 0x20: applyFlags(rsub2(x, x, imm16)); pc += 2; break;
      case 0x21: applyFlags(rsub2(x, y, imm16)); pc += 2; break;
      case 0x22: applyFlags(rsub2(x, x, y)); break;
      case 0x23: applyFlags(rsub2(x, y, x)); break;
      case 0x24: applyFlags(rsub2(y, x, imm16)); pc += 2; break;
      case 0x25: applyFlags(rsub2(y, y, imm16)); pc += 2; break;
      case 0x26: applyFlags(rsub2(y, x, y)); break;
      case 0x27: applyFlags(rsub2(y, y, x)); break;
      case 0x30: applyFlags(rmul2(x, x, imm16)); pc += 2; break;
      case 0x31: applyFlags(rmul2(x, y, imm16)); pc += 2; break;
      case 0x32: applyFlags(rmul2(x, x, y)); break;
      case 0x33: applyFlags(rmul2(x, y, x)); break;
      case 0x34: applyFlags(rmul2(y, x, imm16)); pc += 2; break;
      case 0x35: applyFlags(rmul2(y, y, imm16)); pc += 2; break;
      case 0x36: applyFlags(rmul2(y, x, y)); break;
      case 0x37: applyFlags(rmul2(y, y, x)); break;
      case 0x40: applyFlags(rdiv2(x, x, imm16)); pc += 2; break;
      case 0x41: applyFlags(rdiv2(x, y, imm16)); pc += 2; break;
      case 0x42: applyFlags(rdiv2(x, x, y)); break;
      case 0x43: applyFlags(rdiv2(x, y, x)); break;
      case 0x44: applyFlags(rdiv2(y, x, imm16)); pc += 2; break;
      case 0x45: applyFlags(rdiv2(y, y, imm16)); pc += 2; break;
      case 0x46: applyFlags(rdiv2(y, x, y)); break;
      case 0x47: applyFlags(rdiv2(y, y, x)); break;

      // ALU: Increments & decrements
      case 0x50: applyFlags(radd(x, 1)); break;
      case 0x51: applyFlags(rsub(x, 1)); break;
      case 0x52: applyFlags(radd(y, 1)); break;
      case 0x53: applyFlags(rsub(y, 1)); break;
      case 0x54: applyFlags(radd(address(imm16), 1)); pc += 2; break;
      case 0x55: applyFlags(rsub(address(imm16), 1)); pc += 2; break;
      case 0x56: applyFlags(radd(address(x), 1)); break;
      case 0x57: applyFlags(rsub(address(x), 1)); break;
      case 0x58: applyFlags(radd(address(y), 1)); break;
      case 0x59: applyFlags(rsub(address(y), 1)); break;

      // ALU: Logic
      case 0x60: applyFlags(rand2(x, x, imm16)); pc += 2; break;
      case 0x61: applyFlags(rand2(x, y, imm16)); pc += 2; break;
      case 0x62: applyFlags(rand2(x, x, y)); break;
      case 0x63: applyFlags(rand2(x, y, x)); break;
      case 0x64: applyFlags(rand2(y, x, imm16)); pc += 2; break;
      case 0x65: applyFlags(rand2(y, x, imm16)); pc += 2; break;
      case 0x66: applyFlags(rand2(y, x, y)); break;
      case 0x67: applyFlags(rand2(y, y, x)); break;
      case 0x70: applyFlags(ror2 (x, x, imm16)); pc += 2; break;
      case 0x71: applyFlags(ror2 (x, y, imm16)); pc += 2; break;
      case 0x72: applyFlags(ror2 (x, x, y)); break;
      case 0x73: applyFlags(ror2 (x, y, x)); break;
      case 0x74: applyFlags(ror2 (y, x, imm16)); pc += 2; break;
      case 0x75: applyFlags(ror2 (y, x, imm16)); pc += 2; break;
      case 0x76: applyFlags(ror2 (y, x, y)); break;
      case 0x77: applyFlags(ror2 (y, y, x)); break;
      case 0x80: applyFlags(rxor2(x, x, imm16)); pc += 2; break;
      case 0x81: applyFlags(rxor2(x, y, imm16)); pc += 2; break;
      case 0x82: applyFlags(rxor2(x, x, y)); break;
      case 0x83: applyFlags(rxor2(x, y, x)); break;
      case 0x84: applyFlags(rxor2(y, x, imm16)); pc += 2; break;
      case 0x85: applyFlags(rxor2(y, x, imm16)); pc += 2; break;
      case 0x86: applyFlags(rxor2(y, x, y)); break;
      case 0x87: applyFlags(rxor2(y, y, x)); break;

      // ALU: Resets
      case 0x88: applyFlags(rxorr(x, x)); break;
      case 0x89: applyFlags(rxorr(y, y)); break;
      case 0x8a: applyFlags(rxorr(address(imm16), address(imm16))); pc += 2; break;
      case 0x8b: applyFlags(rxorr(address(x), address(x))); break;
      case 0x8c: applyFlags(rxorr(address(y), address(y))); break;

      // Calls & Return
      case 0x90: pc += 3; ww(sp, pc) sp += 2; rsetr(pc, imm16); this->jump = true; break;
      case 0x91: pc += 1; ww(sp, pc) sp += 2; rsetr(pc, x); this->jump = true; break;
      case 0x92: pc += 1; ww(sp, pc) sp += 2; rsetr(pc, y); this->jump = true; break;
      case 0x93: sp -= 2; rset(pc, rw(sp)); this->jump = true; break;

      // Unconditioned jumps
      case 0xa0: rset(pc, imm16); this->jump = true; break;
      case 0xa1: rset(pc, x); this->jump = true; break;
      case 0xa2: rset(pc, y); this->jump = true; break;

      // Conditioned jumps
      case 0xa3: testFlags(this->of, this->jump = true; testFlags(this->of, rset(pc, imm16),);, ++pc;); break;
      case 0xa4: testFlags(this->of, this->jump = true; testFlags(this->of, rset(pc, x),);    , ++pc;); break;
      case 0xa5: testFlags(this->of, this->jump = true; testFlags(this->of, rset(pc, y),);    , ++pc;); break;
      case 0xa6: testFlags(this->zf, this->jump = true; testFlags(this->zf, rset(pc, imm16),);, ++pc;); break;
      case 0xa7: testFlags(this->zf, this->jump = true; testFlags(this->zf, rset(pc, x),);    , ++pc;); break;
      case 0xa8: testFlags(this->zf, this->jump = true; testFlags(this->zf, rset(pc, y),);    , ++pc;); break;
      case 0xa9: testFlags(this->cf, this->jump = true; testFlags(this->cf, rset(pc, imm16),);, ++pc;); break;
      case 0xaa: testFlags(this->cf, this->jump = true; testFlags(this->cf, rset(pc, x),);    , ++pc;); break;
      case 0xab: testFlags(this->cf, this->jump = true; testFlags(this->cf, rset(pc, y),);    , ++pc;); break;
      case 0xac: testFlagsNot(this->of, this->jump = true; testFlagsNot(this->of, rset(pc, imm16),);, ++pc;); break;
      case 0xad: testFlagsNot(this->of, this->jump = true; testFlagsNot(this->of, rset(pc, x),);    , ++pc;); break;
      case 0xae: testFlagsNot(this->of, this->jump = true; testFlagsNot(this->of, rset(pc, y),);    , ++pc;); break;
      case 0xaf: testFlagsNot(this->zf, this->jump = true; testFlagsNot(this->zf, rset(pc, imm16),);, ++pc;); break;
      case 0xb0: testFlagsNot(this->zf, this->jump = true; testFlagsNot(this->zf, rset(pc, x),);    , ++pc;); break;
      case 0xb1: testFlagsNot(this->zf, this->jump = true; testFlagsNot(this->zf, rset(pc, y),);    , ++pc;); break;
      case 0xb2: testFlagsNot(this->cf, this->jump = true; testFlagsNot(this->cf, rset(pc, imm16),);, ++pc;); break;
      case 0xb3: testFlagsNot(this->cf, this->jump = true; testFlagsNot(this->cf, rset(pc, x),);    , ++pc;); break;
      case 0xb4: testFlagsNot(this->cf, this->jump = true; testFlagsNot(this->cf, rset(pc, y),);    , ++pc;); break;

      // Unconditioned branches
      case 0xb5: applyFlags(radds(pc, (s8)imm8)); this->jump = true; break;
      case 0xb6: applyFlags(radds(pc, (s8)x)); this->jump = true; break;
      case 0xb7: applyFlags(radds(pc, (s8)y)); this->jump = true; break;

      // Conditioned branches
      case 0xb8: testFlags(this->of, this->jump = true; testFlags(this->of, applyFlags(radds(pc, (s8)imm8)),);, ++pc;); break;
      case 0xb9: testFlags(this->of, this->jump = true; testFlags(this->of, applyFlags(radds(pc, (s8)x)),);   , ++pc;); break;
      case 0xba: testFlags(this->of, this->jump = true; testFlags(this->of, applyFlags(radds(pc, (s8)y)),);   , ++pc;); break;
      case 0xbb: testFlags(this->zf, this->jump = true; testFlags(this->zf, applyFlags(radds(pc, (s8)imm8)),);, ++pc;); break;
      case 0xbc: testFlags(this->zf, this->jump = true; testFlags(this->zf, applyFlags(radds(pc, (s8)x)),);   , ++pc;); break;
      case 0xbd: testFlags(this->zf, this->jump = true; testFlags(this->zf, applyFlags(radds(pc, (s8)y)),);   , ++pc;); break;
      case 0xbe: testFlags(this->cf, this->jump = true; testFlags(this->cf, applyFlags(radds(pc, (s8)imm8)),);, ++pc;); break;
      case 0xbf: testFlags(this->cf, this->jump = true; testFlags(this->cf, applyFlags(radds(pc, (s8)x)),);   , ++pc;); break;
      case 0xc0: testFlags(this->cf, this->jump = true; testFlags(this->cf, applyFlags(radds(pc, (s8)y)),);   , ++pc;); break;
      case 0xc1: testFlagsNot(this->of, this->jump = true; testFlagsNot(this->of, applyFlags(radds(pc, (s8)imm8)),);, ++pc;); break;
      case 0xc2: testFlagsNot(this->of, this->jump = true; testFlagsNot(this->of, applyFlags(radds(pc, (s8)x)),);   , ++pc;); break;
      case 0xc3: testFlagsNot(this->of, this->jump = true; testFlagsNot(this->of, applyFlags(radds(pc, (s8)y)),);   , ++pc;); break;
      case 0xc4: testFlagsNot(this->zf, this->jump = true; testFlagsNot(this->zf, applyFlags(radds(pc, (s8)imm8)),);, ++pc;); break;
      case 0xc5: testFlagsNot(this->zf, this->jump = true; testFlagsNot(this->zf, applyFlags(radds(pc, (s8)x)),);   , ++pc;); break;
      case 0xc6: testFlagsNot(this->zf, this->jump = true; testFlagsNot(this->zf, applyFlags(radds(pc, (s8)y)),);   , ++pc;); break;
      case 0xc7: testFlagsNot(this->cf, this->jump = true; testFlagsNot(this->cf, applyFlags(radds(pc, (s8)imm8)),);, ++pc;); break;
      case 0xc8: testFlagsNot(this->cf, this->jump = true; testFlagsNot(this->cf, applyFlags(radds(pc, (s8)x)),);   , ++pc;); break;
      case 0xc9: testFlagsNot(this->cf, this->jump = true; testFlagsNot(this->cf, applyFlags(radds(pc, (s8)y)),);   , ++pc;); break;

      // Stack operations
      case 0xe0: ww(sp, imm16); sp += 2; pc += 2; break;
      case 0xe1: ww(sp, rw(imm16)); sp += 2; pc += 2; break;
      case 0xe2: ww(sp, rw(x)); sp += 2; break;
      case 0xe3: ww(sp, rw(y)); sp += 2; break;
      case 0xe4: ww(sp, x); sp += 2; break;
      case 0xe5: ww(sp, y); sp += 2; break;
      case 0xe6: ww(sp, x); sp += 2; ww(sp, y); sp += 2; break;
      case 0xe7: sp -= 2; rset(x, rw(sp)); break;
      case 0xe8: sp -= 2; rset(y, rw(sp)); break;
      case 0xe9: sp -= 2; rset(y, rw(sp)); sp -= 2; rset(x, rw(sp)); break;

      // Misc.
      case 0xfe: break;
      case 0xff:
      #ifndef _CPU_EXIT_ON_BRK
        while (!((this->z && this->irqf) && (this->z && this->nmif))) {}
      #endif
      break;
      default:
        #ifdef _CPU_LOG
          _log(w, "Unimplemented instruction $" << hex(instruction, 2));
        #endif
      break;
    } if (!jump) { ++this->pc; } else { this->jump = false; };
    #ifdef _CPU_LOG
      _log(d, "After exec:" <<
	        "\npc = $" << hex(pc, 4) << "\n" <<
	        "sp = $" << hex(sp, 4) << "\n" <<
	        "x  = $" << hex(x , 4) << "\n" <<
          "y  = $" << hex(y , 4) << "\n" <<
          "z  = $" << hex(z , 2) << "\n" <<
          "jp = " << (int)jump << "\n"
      )
    #endif
  }
};
