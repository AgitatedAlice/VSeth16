#ifndef INSTRUCTION_BUILDING_TOOL_C
#define INSTRUCTION_BUILDING_TOOL_C

#include "../extra/vseth16asm.h"

typedef struct {
  int instructionCount;
  ASTM16 vm;
} ObservableVM;

void initializeObservableVM(ObservableVM * ovm);
void executeUntilHalt(ObservableVM * ovm);
void addInstruction(ObservableVM * ovm, AWORD instruction);
//opCodeConstructions
AWORD halt();


//       Functions


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
  while( ! (ovm->vm.HALT) ){
    printf("%d\n",ovm->vm.PC.a);
    ASTM_tick(&ovm->vm,8);
  }
}

void addInstruction(ObservableVM * ovm, AWORD instruction){
  //printf("%#010x %#010x\n",instruction.b[0],instruction.b[1]);
  ovm->vm.MEM[0].d[ovm->instructionCount] = instruction.w;
  ++(ovm->instructionCount);
}


//0x00
AWORD nop(){
  AWORD nope;
  setOpCode(&nope,NOP);
  setOperand(&nope,NOP);//does not matter
  return nope;
}



//0xFF
AWORD halt(){
  AWORD halt;
  setOpCode(&halt,HLT);
  setOperand(&halt,NOP);//does not matter
  return halt;
}

#endif
