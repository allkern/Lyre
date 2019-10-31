#pragma once
#include <vector>

#include "global.h"
#include "device.h"
#include "memory.h"
#include "console.h"

class bus {
  // Connected devices
  std::vector <device*> devices;

  bool ensureNoConflict() {}

public:
  bus() {
    devices.push_back(&_memory_);
    // devices.push_back(new console<40, 30>);
  }

  void joinAllThreads() { for (device* d : devices) { d->joinThread(); } }

  device* getDevice(int idx) { return devices[idx]; }

  #define inRange(a) (d->getLowerBound() <= a) && (d->getUpperBound() >= a)
  u8& rb(u16 addr) {
    for (device* d : this->devices) {
      if (inRange(addr)) { return d->rb(d->translate(addr)); }
    }
    _log(w, "Bus read on unallocated space ($" << hex(addr, 4) << ")");
  }

  void wb(u16 addr, u8 v) {
    for (device* d : this->devices) {
      if (inRange(addr)) { d->wb(d->translate(addr), v); break; }
    }
    _log(w, "Bus write on unallocated space ($" << hex(addr, 4) << ")");
  }
}; bus _bus_;
