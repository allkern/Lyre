#pragma once
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdint>

#define _CPU_LOG
#define _CPU_EXIT_ON_BRK
#define _CPU_REALTIME
#define _CPU_FREQ 0.5 // In Hz

#define _log(l, d) std::cout << "[" << #l << ": " << __FUNCTION__ << "] " << d << "\n";
#define hex(n, w) std::hex << std::setfill('0') << std::setw(w) << (int)n

template <typename _Ty> std::string hexs(_Ty n) {
  std::ostringstream o;
  o << std::hex << std::setfill('0') << std::setw((sizeof(_Ty))*2) << (int)n;
  return o.str();
}

typedef uint_least8_t  u8 ;
typedef uint_least16_t u16;
typedef uint_least16_t u32;
typedef  int_least8_t  s8 ;

#undef _CPU_REALTIME
//#undef _CPU_LOG
