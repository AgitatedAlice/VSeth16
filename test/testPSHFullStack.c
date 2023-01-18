#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  addInstruction(&ovm,liw(A_REGISTER));
  addWord(&ovm,0xDEAD);
  for(uint16_t i= 0x00 ; i < 0xFF ;++i){
    addInstruction(&ovm,psh(A_REGISTER));
  }
  addInstruction(&ovm,psh(A_REGISTER)); //one more
  addInstruction(&ovm,halt());
  executeUntilHalt(&ovm);

  assert(ovm.vm.sX.p==0xFF);
  return 0;
}
