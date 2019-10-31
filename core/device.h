#pragma once
#include <utility>
#include <thread>
#include <string>

#include "global.h"

enum busMode { r = 0b01, w = 0b10, rw = r | w };

class device {
public:
  // Optional thread to host the device
  std::thread* pThread = nullptr;
  
  // Addressing space to allocate
  std::pair <u16, u16> busAlloc;

  std::string Name = "<unknown>";
  unsigned long Id = -1;

  // Interrupt Flags
  bool Irq = false, Nmi = false;

  // Device mode
  busMode Mode;

  u16 getLowerBound() { return std::get<0>(this->busAlloc); }
  u16 getUpperBound() { return std::get<1>(this->busAlloc); }
  void joinThread() { if (this->pThread != nullptr) { this->pThread->join(); }}

  // Read a byte
  virtual u8&  rb(u16) = 0;

  // Write a byte
  virtual void wb(u16, u8) = 0;

  // Translate address to local space
  virtual u16  translate(u16) = 0;
};

#define make_rb(body) \
  u8& rb(u16 addr) override { \
    if ((this->Mode == busMode::r) || (this->Mode == busMode::rw)) { \
      body \
    } else { _log(w, "Requested read on a write only device (" << this->Name << ", id " << this->Id << ")"); } \
  }

#define make_wb(body) \
  void wb(u16 addr, u8 v) override { \
    if ((this->Mode == busMode::w) || (this->Mode == busMode::rw)) { \
      body \
    } else { _log(w, "Requested write on a read only device (" << this->Name << ", id " << this->Id << ")"); } \
  }

#define make_translator(body) \
  u16 translate(u16 addr) override { body }
