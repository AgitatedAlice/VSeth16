#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  addInstruction(&ovm,0xFF00);//halt
  executeUntilHalt(&ovm);
  
  assert(ovm.instructionCount==1);
  assert(ovm.vm.HALT);
  return 0;
}
