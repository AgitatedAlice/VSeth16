#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

//Test case showing an issue.
//If we push values till the max, and then we pop, we pop the max - 1
//I left the wrong assertion in there to illustrate this

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  addInstruction(&ovm,liw(A_REGISTER));
  addWord(&ovm,0xDEAD);
  for(uint16_t i= 0x00 ; i < 0xFF ;++i){
    addInstruction(&ovm,psh(A_REGISTER));
  }
  addInstruction(&ovm,liw(A_REGISTER));
  addWord(&ovm,0xBEEF);
  addInstruction(&ovm,psh(A_REGISTER));
  addInstruction(&ovm,pop(D_REGISTER)); 
  addInstruction(&ovm,halt());
  executeUntilHalt(&ovm);

  assert(dRegisterValue(&ovm) == 0xDEAD); //This assertion should not pass.
  assert(dRegisterValue(&ovm) == 0xBEEF);
  return 0;
}
