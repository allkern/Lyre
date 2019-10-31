#pragma once
#include <algorithm>
#include <utility>
#include <array>

#include "device.h"
#include "global.h"
#include "file.h"

class memory : public device {
  std::array <u8, 0x7fff> m;

public:
  memory() {
    this->busAlloc = std::make_pair((u16)0x0000, (u16)0x7fff);
    this->Name = "Lycon RAM Module";
    this->Mode = busMode::rw;
    this->Id   = 0;
    std::fill(m.begin(), m.end(), (u8)0xff);
  }

  void init(std::string _Fname) {
    file rom = file(_Fname);
    std::copy(std::begin(rom), std::end(rom), std::begin(this->m));
  }

  make_rb( return m[addr]; )
  make_wb( m[addr] = v; )
  u16 translate(u16 addr) override { return addr - getLowerBound(); }
}; memory _memory_;
