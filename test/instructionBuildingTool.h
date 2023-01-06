#ifndef INSTRUCTION_BUILDING_TOOL_C
#define INSTRUCTION_BUILDING_TOOL_C

#include "../extra/vseth16asm.h"

typedef struct {
  int instructionCount;
  ASTM16 vm;
} ObservableVM;

void initializeObservableVM(ObservableVM * ovm){
  ovm-> instructionCount = 0;
  ovm->vm = ASTM_Init(0, 0);
  // Init first 16 pages of memory as RAM
  for(uint8_t __i = 0; __i < 0x10; __i++){
    ovm->vm.MEM[__i] = InitAPage('m');
  }
  ovm->vm.MEM[0x10] = InitAPage('r');
}

void executeUntilHalt(ObservableVM * ovm){
  while( ! (ovm->vm.HALT) )
    ASTM_tick(&ovm->vm,8);
}

void addInstruction(ObservableVM * ovm, int instruction){
  ovm->vm.MEM[0].d[ovm->instructionCount] = instruction;
  ++(ovm->instructionCount);
}

#endif
