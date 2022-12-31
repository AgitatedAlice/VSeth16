#include <stdio.h>
#include "cgafc.h"

#define PASS 0
#define FAIL 1


int assert(bool booleanExpression){
  if(booleanExpression){ 
   printf("Passed\n");
    return PASS;
  }
  else{
    printf("Failed\n");
    return FAIL;
  }
}

int main(void){
  ASTM16 seth16 = ASTM_Init(0x10, 0);
  // Init first 16 pages of memory as RAM
  for(uint8_t __i = 0; __i < 0x10; __i++){
    seth16.MEM[__i] = InitAPage('m');
  }
  seth16.MEM[0x10] = InitAPage('r');
  ASTM16AddInstruction(&seth16,0x7F);//halt
  ASTM_tick(&seth16,8);
  
  assert(seth16.instructionCount==1);
  assert(seth16.HALT);
}
