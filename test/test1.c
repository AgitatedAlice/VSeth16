#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"


int main(void){
  ASTM16 seth16 = ASTM_Init(0, 0);
  // Init first 16 pages of memory as RAM
  for(uint8_t __i = 0; __i < 0x10; __i++){
    seth16.MEM[__i] = InitAPage('m');
  }
  seth16.MEM[0x10] = InitAPage('r');
  ASTM16AddInstruction(&seth16,0xFFFF);//halt
  ASTM_tick(&seth16,8);
  
  assert(seth16.instructionCount==1);
  assert(seth16.HALT);
  return 0;
}
