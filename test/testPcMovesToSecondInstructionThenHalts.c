#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  
  addInstruction(&ovm,nop());//whatever
  addInstruction(&ovm,halt());//halt
  executeUntilHalt(&ovm);

  assert(ovm.vm.PC.a==2);
  return 0;
}
