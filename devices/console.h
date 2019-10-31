#include <array>
#include <cmath>

#include "device.h"
#include "global.h"

template <int w, int h> class console : public device {
  std::array <u8, (w-1)*(h-1)> d;
  void loop() {
    auto fPos = [&] (u8 x, u8 y) { return (u16)(x+(y*w)); };
    while (true) {
      if (consoleRegs[3] && cnslR) { consoleRegs[0] = d[fPos(consoleRegs[1], consoleRegs[2])]; }
      if (consoleRegs[3] && cnslW) { d[fPos(consoleRegs[1], consoleRegs[2])] = consoleRegs[0]; }
      for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
          std::cout << (char)d[x+(y*w)];
        } std::cout << '\n';
      } system("clear");
    }
  }

  const u8 cnslW = 0x01,
           cnslR = 0x02;

  u8 consoleRegs[3] = { 0 };

public:
  console() {
    this->busAlloc = std::make_pair((u16)0xa000, (u16)0xa004);
    this->pThread = new std::thread(&console::loop, this);
    this->Name = "LY9002 Console Controller";
    this->Mode = busMode::rw;
    this->Id   = 2;
    std::fill(d.begin(), d.end(), 0);
  }

  make_rb( return consoleRegs[addr]; )
  make_wb( consoleRegs[addr] = v; )
  make_translator( return addr - getLowerBound(); )
};
