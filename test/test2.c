#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  
  addInstruction(&ovm,halt());//whatever
  addInstruction(&ovm,halt());//halt
  executeUntilHalt(&ovm);

  assert(ovm.vm.PC.p==3);
  assert(ovm.vm.HALT);
  return 0;
}
