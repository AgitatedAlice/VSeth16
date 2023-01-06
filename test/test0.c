#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  ovm.vm = ASTM_Init(0, 0);
  // Init first 16 pages of memory as RAM
  for(uint8_t __i = 0; __i < 0x10; __i++){
    ovm.vm.MEM[__i] = InitAPage('m');
  }
  ovm.vm.MEM[0x10] = InitAPage('r');
  
  assert(ovm.instructionCount==0);
  return 0;
}
