#ifndef CGAFC_H
#define CGAFC_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aautil.h"

// VSeth16, fantasy computer/ASTK16 VM
// Umpteenth attempt

/*

Annoying block of text about nothing! Cut this down

*/

typedef union {
	uint16_t w;
	uint8_t b[sizeof(uint16_t)];
} AWORD;

typedef struct {
        uint8_t p; //page
        uint16_t a; //address (on page)
} ADR24UI;

#define ASTM_FLG_CARRY	(1 << 0)
#define ASTM_FLG_ZERO	(1 << 1)
#define ASTM_FLG_NEG	(1 << 2)
#define ASTM_FLG_IDIS	(1 << 3)
#define ASTM_FLG_XSOF	(1 << 4)
#define ASTM_FLG_CSOF	(1 << 5)
#define ASTM_FLG_HALT	(1 << 6)

#define ASTM_PIN_RST	0
#define ASTM_PIN_IRQ	1
#define ASTM_PIN_NMI	2

typedef struct {
	unsigned char t; // type 'r' for ROM, 'm' for RAM, 'd' for device, 'f' for non-volatile
	uint16_t *d; // pointer to 64k page
} APAGE;
// Initialize page and reserve memory for it
APAGE InitAPage(unsigned char t){
	printf("Initializing page type %c\n", t);
	return (APAGE){.t = t, .d = (uint16_t *)calloc(0x10000, sizeof(uint16_t))};
}
// Init null page* correctly
APAGE InitNullPage(){
	return (APAGE){.t = 'u', .d = NULL};
}

typedef struct {
	bool HALT;
	uint8_t		SR; // status register contains 8 flags for system operation stuff
	uint8_t		PR; // page register for memory operations
	APAGE		MEM[0x100];//uint16_t	*RAM;//[1 << 16];
	//uint16_t	IR;
	ADR24UI		PC; // 24 bit program counter
	uint16_t	r[2]; // 0 = A 1 = D
	//uint16_t	rA;
	//uint16_t	rD;
	stk816		sX; // X stack, for general purpose
	stk816		sC; // C stack, for call return
} ASTM16;


ASTM16 ASTM_Init(uint16_t IPC, uint8_t IPP);
int ASTM_tick(ASTM16 *vm, uint8_t pins);
void setOpCode(AWORD* instr, uint8_t value);
void setOperand(AWORD* instr, uint8_t value);
uint8_t opCode(AWORD* instr);
uint8_t operand(AWORD* instr);

ASTM16 ASTM_Init(uint16_t IPC, uint8_t IPP){
	//return (ASTM16){.HALT = false, .SR = 0, /*.RAM = (uint16_t *)calloc(0x10000, sizeof(uint16_t)),*/ /*.IR = 0,*/ .PC = IPC, /*.rA = 0, .rD = 0,*/ .r[0] = 0, .r[1] = 0, .sX = S816_Init(), .sC = S816_Init()};
	ASTM16 ret; ret.HALT = false; ret.SR = 0; ret.PC.a = IPC; ret.PC.p = IPP; ret.r[0] = 0; ret.r[1] = 0; ret.sX = S816_Init(); ret.sC = S816_Init();
	uint16_t i;
	for(i = 0; i < 0x100; i++){
		ret.MEM[i] = InitNullPage();//InitPage('m');
	}
	//ret.MEM[16] = InitPage('r'); // boot ROM
	return ret;
}

int ASTM_tick(ASTM16 *vm, uint8_t pins){
	if(vm->HALT) return 10;
	int rval = 0; // return value, keep it 0 if nothing goes wrong
	uint16_t PCinc = 0; // PC is incremented with this + 1, set depending on how many words were fetched by an instruction
	bool jumpen = false;
	// YES INTERRUPTS NOW
	if(AABT(pins, ASTM_PIN_RST)){
		vm->SR = 0; vm->PC.a = 0; vm->PC.p = 0; vm->r[0] = 0; vm->r[1] = 0; vm->sX.p=0; vm->sC.p = 0; printf("\n\n\n---RESET---\n\n\n");
		return 1; // code 1 is VM was reset
	}
	if((AABT(pins, ASTM_PIN_IRQ))&& !(vm->SR&&ASTM_FLG_IDIS)){
		S816_PSH(vm->sC, ((vm->PC.a)+1)); 
		S816_PSH(vm->sC, (uint16_t)(vm->SR)|(uint16_t)((vm->PC.p)<<8)); //S816_PSH(vm->sC, (uint16_t)vm->SR);
		vm->PC.a = vm->MEM[0].d[0xFFFA]; vm->SR = ASTM_FLG_IDIS; printf("\n\n\n---IRQ---\n\n\n");
	}
	if(AABT(pins, ASTM_PIN_NMI)){
		S816_PSH(vm->sC, (vm->PC.a)+1); 
		S816_PSH(vm->sC, (uint16_t)(vm->SR)|(uint16_t)((vm->PC.p)<<8)); //S816_PSH(vm->sC, (uint16_t)vm->SR);
		vm->PC.a = vm->MEM[0].d[0xFFFC]; vm->SR = ASTM_FLG_IDIS; printf("\n\n\n---NMI---\n\n\n");
	}
	// check mem page access safety
	if(vm->MEM[vm->PC.p].t == 'u') {return 20;} // error code 20 PC out of range
	if(vm->MEM[vm->PR].t == 'u') {return 21;} // error code 21 PR out of range
	//if(vm->PC.p > 0x1F) return 20;
	//if(vm->PR > 0x1F) return 21;
	
	// fetch instruction
	uint8_t ipage = vm->PC.p; uint16_t iaddr = vm->PC.a;
	AWORD instr; instr.w = vm->MEM[ipage].d[iaddr]; // upper byte is opcode, lower byte is byte operand
	// isolate R bits from I bits in upper byte
	uint8_t opc = opCode(&instr); uint8_t opr = operand(&instr); uint8_t RS = AABT(opc, 7); uint32_t tmp;
	//uint8_t bI = (AABT(opc, 7)*2)+(AABT(opc, 6)); opc = opc & 0x7F;
	// decode & exec opcode: 0bRIIIIIII
	switch(opc&0x7F){ // apply mask when switching instead of changing opcode?
		case 0x00: /* NOP			*/ break; // Halting function is MOVED TO 0xFF (case is 0x7F)
		case 0x01: /* LIB	r, opr	*/ vm->r[RS] = opr; break;
		case 0x02: /* LIW	r, op1	*/ vm->r[RS] = vm->MEM[ipage].d[iaddr+1]; PCinc = 1; break;
		case 0x03: /* LRA			*/ vm->r[1] = vm->MEM[vm->PR].d[vm->r[0]]; break; // D = *A
		case 0x04: /* LIA	opr,op1 */ if(vm->MEM[opr].t != 'u'){vm->r[1] = vm->MEM[opr].d[vm->MEM[ipage].d[iaddr+1]];} break; // D = *immediate 24bit
		case 0x05: /* PRA			*/ S816_PSH(vm->sX, vm->MEM[vm->PR].d[vm->r[0]]); break; // PUSH *A
		case 0x06: /* PIA	opr,op1 */ S816_PSH(vm->sX, vm->MEM[opr].d[vm->MEM[ipage].d[iaddr+1]]); break; // PUSH *immediate 24bit
		case 0x07: /* PSH	r		*/ S816_PSH(vm->sX, vm->r[RS]); break;
		case 0x08: /* POP	r		*/ S816_POP(vm->sX, vm->r[RS]); break;
		case 0x09: /* PTM			*/ if(vm->MEM[vm->PR].t != 'r'){S816_POP(vm->sX, vm->MEM[vm->PR].d[vm->r[0]]);} break; // POP *A or pop to memory at *A
		case 0x0A: /* STR			*/ if(vm->MEM[vm->PR].t != 'r'){vm->MEM[vm->PR].d[vm->r[0]] = vm->r[1];} break; // *A = D
		case 0x0B: /* PEEK	r		*/ vm->r[RS] = vm->sX.mem[vm->sX.p-1]; break;
		case 0x0C: /* PSL	op1		*/ S816_PSH(vm->sX, vm->MEM[ipage].d[iaddr+1]); PCinc = 1; break;
		case 0x0D: /* INR	r		*/ vm->r[RS]++; break;
		case 0x0E: /* DCR	r		*/ vm->r[RS]--; break;
		case 0x0F: /* ADD	r		*/ S816_POP(vm->sX, tmp); S816_PSH(vm->sX, (uint16_t)tmp+vm->r[RS]); if(vm->sX.mem[vm->sX.p-1] == 0){vm->SR |= ASTM_FLG_ZERO;}else{vm->SR &=~ASTM_FLG_ZERO;} break;
		case 0x10: /* SUB	r		*/ S816_POP(vm->sX, tmp); S816_PSH(vm->sX, (uint16_t)tmp-vm->r[RS]); if(vm->sX.mem[vm->sX.p-1] == 0){vm->SR |= ASTM_FLG_ZERO;}else{vm->SR &=~ASTM_FLG_ZERO;} break;
		case 0x11: /* AND	r		*/ S816_POP(vm->sX, tmp); S816_PSH(vm->sX, (uint16_t)tmp&vm->r[RS]); if(vm->sX.mem[vm->sX.p-1] == 0){vm->SR |= ASTM_FLG_ZERO;}else{vm->SR &=~ASTM_FLG_ZERO;} break;
		case 0x12: /* OR	r		*/ S816_POP(vm->sX, tmp); S816_PSH(vm->sX, (uint16_t)tmp|vm->r[RS]); if(vm->sX.mem[vm->sX.p-1] == 0){vm->SR |= ASTM_FLG_ZERO;}else{vm->SR &=~ASTM_FLG_ZERO;} break;
		case 0x13: /* XOR	r		*/ S816_POP(vm->sX, tmp); S816_PSH(vm->sX, (uint16_t)tmp^vm->r[RS]); if(vm->sX.mem[vm->sX.p-1] == 0){vm->SR |= ASTM_FLG_ZERO;}else{vm->SR &=~ASTM_FLG_ZERO;} break;
		case 0x14: /* NOT	r		*/ S816_PSH(vm->sX, ~(vm->r[RS])); if(vm->sX.mem[vm->sX.p-1] == 0){vm->SR |= ASTM_FLG_ZERO;} break;
		case 0x15: /* ADC	r		*/ S816_POP(vm->sX, tmp); if(tmp+vm->r[RS]>0xFFFF){vm->SR|=ASTM_FLG_CARRY;}else{vm->SR&=~ASTM_FLG_CARRY;} S816_PSH(vm->sX, (uint16_t)tmp+vm->r[RS]); if(vm->sX.mem[vm->sX.p-1] == 0){vm->SR |= ASTM_FLG_ZERO;}else{vm->SR &=~ASTM_FLG_ZERO;} break;
		case 0x16: /* SBC	r		*/ S816_POP(vm->sX, tmp); if((int32_t)(tmp+vm->r[RS])<0){vm->SR|=ASTM_FLG_CARRY;}else{vm->SR&=~ASTM_FLG_CARRY;} S816_PSH(vm->sX, (uint16_t)tmp-vm->r[RS]); if(vm->sX.mem[vm->sX.p-1] == 0){vm->SR |= ASTM_FLG_ZERO;}else{vm->SR&=~ASTM_FLG_ZERO;} if(vm->sX.mem[vm->sX.p-1]&(1<<15)){vm->SR |= ASTM_FLG_NEG;}else{vm->SR &=~ASTM_FLG_NEG;} break;
		case 0x17: /* CMP	r		*/ (vm->sX.mem[vm->sX.p-1] == vm->r[RS]) ? (vm->SR |= ASTM_FLG_ZERO) : (vm->SR &= ~ASTM_FLG_ZERO); break; // compare stack top with register, set zero flag if equal
		case 0x18: /* BEQ	opr		*/ if(vm->SR && ASTM_FLG_ZERO){ vm->PC.a = (uint16_t)((int16_t)vm->PC.a+(int16_t)((int8_t)opr));jumpen=true;} break; // branch if equal/zero (remember to implement flag setting in ALUops :D) address is relative and treated as a signed integer
		case 0x19: /* BNE	opr		*/ if(!(vm->SR && ASTM_FLG_ZERO)){ vm->PC.a = (uint16_t)((int16_t)vm->PC.a+(int16_t)((int8_t)opr));jumpen=true;} break; // branch if NOT equal/zero (remember to implement flag setting in ALUops :D) address is relative and treated as a signed integer
		case 0x1A: /* JMP	opr, op1*/ vm->PC.a = vm->MEM[ipage].d[iaddr+1]; vm->PC.p = opr; jumpen = true; break; // thanks god unconditional jumps to 16 bit addresses are so simple :S
		case 0x1B: /* BIN	opr		*/ if(vm->SR && ASTM_FLG_NEG){ vm->PC.a = (uint16_t)((int16_t)vm->PC.a+(int16_t)((int8_t)opr));jumpen=true;} break;
		case 0x1C: /* BGE	opr		*/ if(!(vm->SR && ASTM_FLG_NEG)){ vm->PC.a = (uint16_t)((int16_t)vm->PC.a+(int16_t)((int8_t)opr));jumpen=true;} break;
		case 0x1D: /* BLE	opr		*/ if((vm->SR && ASTM_FLG_NEG)||(vm->SR && ASTM_FLG_ZERO)){ vm->PC.a = (uint16_t)((int16_t)vm->PC.a+(int16_t)((int8_t)opr));jumpen=true;} break;
		case 0x1E: /* BIP	opr		*/ if(!((vm->SR && ASTM_FLG_NEG)||(vm->SR && ASTM_FLG_ZERO))){ vm->PC.a = (uint16_t)((int16_t)vm->PC.a+(int16_t)((int8_t)opr));jumpen=true;} break;
		case 0x1F: /* BIC	opr		*/ if(vm->SR && ASTM_FLG_CARRY){ vm->PC.a = (uint16_t)((int16_t)vm->PC.a+(int16_t)((int8_t)opr));jumpen=true;} break;
		case 0x20: /* BNC	opr		*/ if(!(vm->SR && ASTM_FLG_CARRY)){ vm->PC.a = (uint16_t)((int16_t)vm->PC.a+(int16_t)((int8_t)opr));jumpen=true;} break;
		case 0x21: /* JSR	opr,op1	*/ S816_PSH(vm->sC, (vm->PC.a+2)); S816_PSH(vm->sC, (uint16_t)(vm->SR)|(uint16_t)((vm->PC.p)<<8)); vm->SR = 0; vm->PC.a = vm->MEM[ipage].d[iaddr+1]; vm->PC.p=opr; jumpen=true; break;
		case 0x22: /* RET			*/ S816_POP(vm->sC, tmp); vm->SR=(uint8_t)AALB(tmp); vm->PC.p=(uint8_t)AAUB(tmp); S816_POP(vm->sC, vm->PC.a); jumpen=true; break;
		case 0x23: /* CPR			*/ (vm->r[0] == vm->r[1]) ? (vm->SR |= ASTM_FLG_ZERO) : (vm->SR &= ~ASTM_FLG_ZERO); break; // compare A with D, set zero flag if equal
		case 0x24: /* RTI			*/ S816_POP(vm->sC, tmp); vm->SR=(uint8_t)AALB(tmp); vm->PC.p=(uint8_t)AAUB(tmp); S816_POP(vm->sC, vm->PC.a); vm->SR&=~ASTM_FLG_IDIS; jumpen=true; break; // may not even be necessary as IRQ/NMI would likely be set as relevant by the program being interrupted
		case 0x25: /* TPR	r 		*/ vm->PR = (uint8_t)vm->r[RS]; break; // Sorry for the mess but we are too far in and i made too big a hand assembly test program to care about the poorly ordered instructions
		case 0x26: /* BSR	r, opr	*/ vm->r[RS] >>= AALN(opr); break; // isolating lower nibble because only bitshifts from 0-15 make sense on a 16bit sys
		case 0x27: /* BSL	r, opr	*/ vm->r[RS] <<= AALN(opr); break;
		case 0x7F: /* HLT			*/ if(RS==1){vm->SR |= ASTM_FLG_HALT; vm->HALT = true; rval = 10; break;}else{break;}
		default: break;
	}
	
	
	// increment according to ival
	if(!jumpen) {
		uint32_t tmp2 = vm->PC.a + (PCinc+1);
		if(tmp2 < 0x10000) {vm->PC.a = (uint16_t)tmp2;}
		else { vm->PC.p++; tmp2 -= 0xFFFF; vm->PC.a = (uint16_t)(tmp2); printf("paging!\n");}
		//vm->PC += ;
	}
	
	return rval;
}


// I somehow forgot i was doing a stack machine for a moment there 
 	// decode & exec opcode: 0bRRIIIIII
//	switch(opc){
//		case 0x00: /* NOP 			*/ break;
//		case 0x01: /* LIB	r, ob	*/ vm->r[AABT(bI, 1)]=opr; break;//(AABT(bI, 7)) ? vm->rD : vm->rA = opr; break;
//		case 0x02: /* LIW	r, o1	*/ vm->r[AABT(bI, 1)]=vm->RAM[vm->PC+1]; PCinc = 1; break;//(AABT(bI, 7)) ? vm->rD : vm->rA = vm->RAM[vm->PC+1]; PCinc = 1; break;
//		case 0x03: /* Trr			*/ vm->r[AABT(bI, 1)] = vm->r[AABT(bI, 0)]; break;
//		case 0x04: /* PSH	r		*/ S816_PSH(vm->sX, vm->r[AABT(bI, 1)]); break;
//		case 0x05: /* POP	r		*/ S816_POP(vm->sX, vm->r[AABT(bI, 1)]); break;
//		case 0x06: /* STr	ob		*/ vm->RAM[opr] = vm->r[AABT(bI, 1)]; break;
//		case 0x07: /* STr	o1		*/ vm->RAM[vm->PC+1] = vm->r[AABT(bI, 1)]; PCinc = 1; break;
//		case 0x08: /* STr	*r		*/ vm->RAM[vm->r[AABT(bI, 0)]] = vm->r[AABT(bI, 1)]; break;
//		case 0x09: /* PTM	*r		*/ S816_POP(vm->sX, vm->RAM[vm->r[AABT(bI, 1)]]); break; // pop to memory, might be super useful!
//		case 0x0A: /* ADD	r, r 	*/ vm->r[AABT(bI, 1)] += vm->r[AABT(bI, 0)]; break;
//		case 0x0B: /* SUB	r, r 	*/ vm->r[AABT(bI, 1)] -= vm->r[AABT(bI, 0)]; break;
//		case 0x0C: /* ADB	r, ob	*/ vm->r[AABT(bI, 1)] += opr; break;
//		case 0x0D: /* ADW	r, o1	*/ vm->r[AABT(bI, 1)] += vm->RAM[vm->PC+1]; PCinc = 1; break;
//		case 0x0E: /* SIB	r, ob	*/ vm->r[AABT(bI, 1)] -= opr; break;
//		case 0x0F: /* SIW	r, o1	*/ vm->r[AABT(bI, 1)] -= vm->RAM[vm->PC+1]; PCinc = 1; break;
//		case 0x10: /* AND	r, r 	*/ vm->r[AABT(bI, 1)] &= vm->r[AABT(bI, 0)]; break;
//		case 0x11: /* OR	r, r	*/ vm->r[AABT(bI, 1)] |= vm->r[AABT(bI, 0)]; break;
//		case 0x12: /* XOR	r, r	*/ vm->r[AABT(bI, 1)] ^= vm->r[AABT(bI, 0)]; break;
//		case 0x13: /* NOT	r		*/ vm->r[AABT(bI, 1)] = ~(vm->r[AABT(bI, 1)]); break;
//		case 0x14: /* BSU	r 		*/ vm->r[AABT(bI, 1)] <<= 1;
//		case 0x15: /* BSD	r 		*/ vm->r[AABT(bI, 1)] >>= 1;
//		case 0x16: /* CPR	r		*/ uint8_t tmpSR = 0x00; tempSR = (((ASTM_FLG_NEG)*(vm->r[AABT(bI, 1)]==0))|((ASTM_FLG_NEG)*(AABT(vm->r[AABT(bI, 1)],7)))); break;
//		default: break;
//	}

// no more random deprecated legacy code below here, what could be salvaged has been salvaged

void setOpCode(AWORD* instr, uint8_t value){
  instr->b[1] = value;
}

void setOperand(AWORD* instr, uint8_t value){
  instr->b[0] = value;
}

uint8_t opCode(AWORD* instr){
  return instr->b[1];
}

uint8_t operand(AWORD* instr){
  return instr->b[0];
}

#endif
