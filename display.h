#include <iostream>
#include <array>
#include <cmath>

#include "global.h"
#include "device.h"

// bbbbffff 4-bit color

class display : public device {
  std::array <u8, 0x12c0> vram; // 60 columns, 40 rows
  std::array <u8, 0x07ff> cram; // Space for 255, 8 line characters

  void tick() {
    for (u8 v : vram) {

    }
  }
public:
  memory() {
    this->busAlloc = std::make_pair((u16)0x8000, (u16)0x9fff);
    this->pThread = new std::thread()
    this->Name = "LY2C03 Display Controller";
    this->Mode = busMode::rw;
    this->Id   = 1;
    std::fill(vram.begin(), vram.end(), (u8)(rand() % 0xff););
    std::fill(cram.begin(), cram.end(), (u8)(rand() % 0xff););
  }
};
