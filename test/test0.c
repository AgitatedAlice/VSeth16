#include <stdio.h>
#include "../src/cgafc.h"

#define PASS 0
#define FAIL 1

int assertionCount = 0;

int assert(bool booleanExpression){
  ++assertionCount;
  if(booleanExpression){
   printf("Passed\n");
   return PASS;
  }
  else{
   printf("FFFailed\n");
   return assertionCount;
  }
}

int main(void){
  ASTM16 seth16 = ASTM_Init(0x10, 0);
  // Init first 16 pages of memory as RAM
  for(uint8_t __i = 0; __i < 0x10; __i++){
    seth16.MEM[__i] = InitAPage('m');
  }
  seth16.MEM[0x10] = InitAPage('r');
  
  assert(seth16.instructionCount==0);
  return 0;
}
