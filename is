// Instruction set
// ld   x, imm16   opcode 00xxxx
// ld   y, imm16   opcode 01xxxx
// ld   x, [imm16] opcode 02xxxx
// ld   y, [imm16] opcode 03xxxx
// ld   [imm16], x opcode 04xxxx
// ld   [imm16], y opcode 05xxxx
// ld   x, y       opcode 06
// ld   y, x       opcode 07
// ld   sp, imm16  opcode 08xxxx
// ld   sp, x      opcode 09
// ld   sp, y      opcode 0a
// ld   sp, [x]    opcode 0b
// ld   sp, [y]    opcode 0c
// ld   sp,[imm16] opcode 0dxxxx

// addx x, imm16   opcode 10xxxx
// addx y, imm16   opcode 11xxxx
// addx x, y       opcode 12
// addx y, x       opcode 13
// addy x, imm16   opcode 14xxxx
// addy y, imm16   opcode 15xxxx
// addy x, y       opcode 16
// addy y, x       opcode 17
// subx x, imm16   opcode 20xxxx
// subx y, imm16   opcode 21xxxx
// subx x, y       opcode 22
// subx y, x       opcode 23
// suby x, imm16   opcode 24xxxx
// suby y, imm16   opcode 25xxxx
// suby x, y       opcode 26
// suby y, x       opcode 27
// mulx x, imm16   opcode 30xxxx
// mulx y, imm16   opcode 31xxxx
// mulx x, y       opcode 32
// mulx y, x       opcode 33
// muly x, imm16   opcode 34xxxx
// muly y, imm16   opcode 35xxxx
// muly x, y       opcode 36
// muly y, x       opcode 37
// divx x, imm16   opcode 40xxxx
// divx y, imm16   opcode 41xx
// divx x, y       opcode 42
// divx y, x       opcode 43
// divy x, imm16   opcode 44xxxx
// divy y, imm16   opcode 45xxxx
// divy x, y       opcode 46
// divy y, x       opcode 47
// ix              opcode 50
// dx              opcode 51
// iy              opcode 52
// dy              opcode 53
// im [imm16]      opcode 54xxxx
// dm [imm16]      opcode 55xxxx
// im [x]          opcode 56
// dm [x]          opcode 57
// im [y]          opcode 58
// dm [y]          opcode 59
// andx x, imm16   opcode 60xxxx
// andx y, imm16   opcode 61xxxx
// andx x, y       opcode 62
// andx y, x       opcode 63
// andy x, imm16   opcode 64xxxx
// andy y, imm16   opcode 65xxxx
// andy x, y       opcode 66
// andy y, x       opcode 67
// orx  x, imm16   opcode 70xxxx
// orx  y, imm16   opcode 71xxxx
// orx  x, y       opcode 72
// orx  y, x       opcode 73
// ory  x, imm16   opcode 74xxxx
// ory  y, imm16   opcode 75xxxx
// ory  x, y       opcode 76
// ory  y, x       opcode 77
// xorx x, imm16   opcode 80xxxx
// xorx y, imm16   opcode 81xxxx
// xorx x, y       opcode 82
// xorx y, x       opcode 83
// xory x, imm16   opcode 84xxxx
// xory y, imm16   opcode 85xxxx
// xory x, y       opcode 86
// xory y, x       opcode 87

// rstx            opcode 88
// rsty            opcode 89
// rst  [imm16]    opcode 8axxxx
// rst  [x]        opcode 8b
// rst  [y]        opcode 8c

// call [imm16]    opcode 90
// call [x]        opcode 91
// call [y]        opcode 92
// ret             opcode 93

// jp   [imm16]    opcode a0xxxx
// jp   [x]        opcode a1
// jp   [y]        opcode a2
// jo   [imm16]    opcode a3xxxx
// jo   [x]        opcode a4
// jo   [y]        opcode a5
// jz   [imm16]    opcode a6xxxx
// jz   [x]        opcode a7
// jz   [y]        opcode a8
// ji   [imm16]    opcode a9xxxx
// ji   [x]        opcode aa
// ji   [y]        opcode ab
// jc   [imm16]    opcode acxxxx
// jc   [x]        opcode ad
// jc   [y]        opcode ae
// jno  [imm16]    opcode afxxxx
// jno  [x]        opcode b0
// jno  [y]        opcode b1
// jnz  [imm16]    opcode b2xxxx
// jnz  [x]        opcode b3
// jnz  [y]        opcode b4
// jni  [imm16]    opcode b5xxxx
// jni  [x]        opcode b6
// jni  [y]        opcode b7
// jnc  [imm16]    opcode b8xxxx
// jnc  [x]        opcode b9
// jnc  [y]        opcode ba

// brn  [imm8]     opcode bbxx
// brn  [x]        opcode bc
// brn  [y]        opcode bd
// bo   [imm8]     opcode bexx
// bo   [x]        opcode bf
// bo   [y]        opcode c0
// bz   [imm8]     opcode c1xx
// bz   [x]        opcode c2
// bz   [y]        opcode c3
// bi   [imm8]     opcode c4xx
// bi   [x]        opcode c5
// bi   [y]        opcode c6
// bc   [imm8]     opcode c7xx
// bc   [x]        opcode c8
// bc   [y]        opcode c9
// bno  [imm8]     opcode caxx
// bno  [x]        opcode cb
// bno  [y]        opcode cc
// bnz  [imm8]     opcode cdxx
// bnz  [x]        opcode ce
// bnz  [y]        opcode cf
// bni  [imm8]     opcode d0xx
// bni  [x]        opcode d1
// bni  [y]        opcode d2
// bnc  [imm8]     opcode d3xx
// bnc  [x]        opcode d4
// bnc  [y]        opcode d5

// push imm16      opcode e0xxxx
// push [imm16]    opcode e1xxxx
// push [x]        opcode e2
// push [y]        opcode e3
// pushx           opcode e4
// pushy           opcode e5
// push            opcode e6
// popx            opcode e7
// popy            opcode e8
// pop             opcode e9

// nop             opcode fe
// brk             opcode ff
