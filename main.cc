#include "cpu.h"
#include "assembler.h"
#include "file.h"
#include "global.h"

int main() {
  cpu myCPU;
  memory& myMemory = myCPU.getMemory();
  assembler myAssembler(myMemory);
  myMemory.init("test.x");
  myCPU.launch();
  //auto fMain = myAssembler.disassemble(0, 0x10);
  //auto fRstX = myAssembler.disassemble(0x200, 0x203);
  //auto fPchX = myAssembler.disassemble(0x100, 0x10e);
  //std::cout << "<fMain>\n";
  //for (std::string s : fMain) {
  //  std::cout << s << '\n';
  //}
  //std::cout << "\n<fRstX>\n";
  //for (std::string s : fRstX) {
  //  std::cout << s << '\n';
  //}
  //std::cout << "\n<fPrintCharX>\n";
  //for (std::string s : fPchX) {
  //  std::cout << s << '\n';
  //}
  //std::cout << '\n';
  return 0;
}
