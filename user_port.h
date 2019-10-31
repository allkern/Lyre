#include <bitset>

#include "device.h"
#include "global.h"

class user_port : public device {
  std::bitset <16> d; // Data pins
  std::bitset <16> a; // Address pins
  std::bitset <8>  x; // Auxiliary pins  
}
