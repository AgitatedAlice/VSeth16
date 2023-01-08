#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  
  addInstruction(&ovm,liw(D_REGISTER));
  addWord(&ovm,0xDEAD);
  addInstruction(&ovm,halt());
  executeUntilHalt(&ovm);

  assert(ovm.vm.PC.a==3);
  return 0;
}
