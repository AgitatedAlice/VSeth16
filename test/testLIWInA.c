#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  
  addInstruction(&ovm,liw(false));
  addWord(&ovm,0xDEAD);
  addInstruction(&ovm,halt());
  executeUntilHalt(&ovm);

  assert(aRegisterValue(&ovm)==0xDEAD);
  return 0;
}
