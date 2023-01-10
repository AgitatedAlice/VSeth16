#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  
  addInstruction(&ovm,liw(A_REGISTER));
  addWord(&ovm,0xDEAD);
  addInstruction(&ovm,psh(A_REGISTER));
  addInstruction(&ovm,pop(A_REGISTER));
  addInstruction(&ovm,halt());
  executeUntilHalt(&ovm);

  assert(ovm.vm.sX.p==0);
  return 0;
}
