#ifndef INSTRUCTION_BUILDING_TOOL_C
#define INSTRUCTION_BUILDING_TOOL_C

#include "../extra/vseth16asm.h"

typedef struct {
  int instructionCount;
  ASTM16 vm;
} ObservableVM;

void executeUntilHalt(ObservableVM * ovm){
  while( ! (ovm->vm.HALT) )
    ASTM_tick(&ovm->vm,8);
}

void addInstruction(ObservableVM * ovm, int instruction){
  ovm->vm.MEM[0].d[ovm->instructionCount] = instruction;
  ++(ovm->instructionCount);
}

#endif
