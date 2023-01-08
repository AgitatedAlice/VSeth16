#ifndef INSTRUCTION_BUILDING_TOOL_C
#define INSTRUCTION_BUILDING_TOOL_C

#include "../extra/vseth16asm.h"

#define A_REGISTER 0
#define D_REGISTER 1


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
  ovm->vm.MEM[0].d[ovm->instructionCount] = instruction.w;
  ++(ovm->instructionCount);
}

void addWord(ObservableVM * ovm, uint16_t word){
  ovm->vm.MEM[0].d[ovm->instructionCount] = word;
  ++(ovm->instructionCount);
}

uint16_t aRegisterValue(ObservableVM * ovm){
  return ovm->vm.r[A_REGISTER];
}

uint16_t dRegisterValue(ObservableVM * ovm){
  return ovm->vm.r[D_REGISTER];
}



//0x00
AWORD nop(){
  AWORD nope;
  setOpCode(&nope,NOP);
  setOperand(&nope,NOP);//does not matter
  return nope;
}

//0x01
/* AWORD lib(bool isD, uint8_t value){ */
/*   AWORD lib[2]; */
/*   setOpCode(&lib[0],LIB); */
/*   setOperand(&lib[0],regist); */
/*   setOpCode(&lib[1],1); */
/*   setOperand(&lib[1],1); */
  
/*   return &lib; */
/* } */

//0x0é
AWORD liw(bool isD){
  AWORD liw;
  setOpCode(&liw,LIW | (isD?0x80:0x00));
  setOperand(&liw,NOP);//does not matter
  
  return liw;
}


//0x03
AWORD lra(){
  AWORD lra;
  setOpCode(&lra,LRA);
  setOperand(&lra,NOP);//does not matter
  return lra;
}

//0x05
AWORD pra(){
  AWORD pra;
  setOpCode(&pra,PRA);
  setOperand(&pra,NOP);//does not matter
  return pra;
}

//0x09
AWORD ptm(){
  AWORD ptm;
  setOpCode(&ptm,PTM);
  setOperand(&ptm,NOP);//does not matter
  return ptm;
}

//0x0A
AWORD str(){
  AWORD str;
  setOpCode(&str,STR);
  setOperand(&str,NOP);//does not matter
  return str;
}

//0x0D
AWORD inr(uint8_t r){
  AWORD inr;
  setOpCode(&inr,INR);
  setOperand(&inr,r);
  return inr;
}


//0x16
AWORD sbc(){
  AWORD sbc;
  setOpCode(&sbc,SBC);
  setOperand(&sbc,NOP);//does not matter
  return sbc;
}


//0x22
AWORD ret(){
  AWORD ret;
  setOpCode(&ret,RET);
  setOperand(&ret,NOP);//does not matter
  return ret;
}


//0x23
AWORD cpr(){
  AWORD cpr;
  setOpCode(&cpr,CPR);
  setOperand(&cpr,NOP);//does not matter
  return cpr;
}

//0x24
AWORD rti(){
  AWORD rti;
  setOpCode(&rti,RTI);
  setOperand(&rti,NOP);//does not matter
  return rti;
}



//0xFF
AWORD halt(){
  AWORD halt;
  setOpCode(&halt,HLT);
  setOperand(&halt,NOP);//does not matter
  return halt;
}

#endif
