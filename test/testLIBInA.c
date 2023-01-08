#include <stdio.h>
#include "../src/cgafc.h"
#include "cAssert.h"
#include "instructionBuildingTool.h"

int main(void){
  ObservableVM ovm;
  initializeObservableVM(&ovm);
  
  addInstruction(&ovm,lib(A_REGISTER, 0xED));
  addInstruction(&ovm,halt());
  executeUntilHalt(&ovm);

  assert(aRegisterValue(&ovm)==0xED);
  return 0;
}
