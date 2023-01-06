#ifndef VSETH16_ASM
#define VSETH16_ASM

#define NOP 0x00

#define LIB 0x01 /* LIB r, opr	--- Load Immediate Byte, loads byte operand, single word instruction does not fetch operand word*/
#define LIW 0x02 /* LIW r, op1	--- Load Immediate Word, loads two bytes supplied from following word*/
#define LRA 0x03 /* LRA 		--- Load Relative Address, register A points to memory location which is loaded into register D*/
#define LIA 0x04 /* LIA opr,op1 --- Load Immediate Address, byte operand in instruction determines page, supplied operand determines page address to load word from into D*/
#define PRA 0x05 /* PRA			--- Same as LRA, but pushes to X stack instead of store in D register*/
#define PIA 0x06 /* PIA opr,op1 --- Same as PRA, but pushes word at memory location addressed 24 bit immediate, i.e supply page in operand byte and address in following word*/
#define PSH 0x07 /* PSH r 		--- PuSH value in register A/D to X stack*/
#define POP 0x08 /* POP r 		--- POP value in X stack to register A/D*/ 
#define PTM 0x09 /* PTM 		--- Push To Memory, pop from X stack directly to memory at current page address *A*/
#define STR 0x0A /* STR 		--- SToRe, *A = D*/
#define PEEK 0x0B /* PEEK r 	--- Load top of stack to register without popping*/
#define PSL 0x0C /* PSL	op1 	--- Push literal to stack, literal supplied in word following instruction*/
#define INR 0x0D /* INR r 		--- Increment A/D*/
#define DCR 0x0E /* DCR	r 		--- Decrement A/D*/
#define ADD 0x0F /* ADD	r 		--- Pop top of stack, add with selected register and push back*/
#define SUB 0x10 /* SUB	r 		--- Subtract, same as ADD but with -*/
#define AND 0x11 /* AND	r 		--- AND, again stack pop push*/ 
#define OR 0x12 /* OR r 		--- OR*/
#define XOR 0x13 /* XOR	r 		--- XOR*/
#define NOT 0x14 /* NOT	r 		--- Invert bits in selected register, !!!no stack access!!!*/
#define ADC 0x15 /* ADC	r 		--- Same as ADD but with carry*/ 
#define SBC 0x16 /* SBC 		--- sub with carry*/
#define CMP 0x17 /* CMP	r 		--- Compare selected register with stack top, set ZERO flag if equal, reset it if not equal*/
#define BEQ 0x18 /* BEQ	opr 	--- Branch if EQual, byte operand is signed 8 bit integer that determines how far up down in memory the branch goes*/
#define BNE 0x19 /* BNE	opr 	--- Branch if NOT eEqual*/
#define JMP 0x1A /* JMP	opr, op1--- Unconditional jump to immediate 24 bit address*/
#define BIN 0x1B /* BIN	opr 	--- Branch if Negative*/
#define BGE 0x1C /* BGE	opr 	--- Branch if >=0*/
#define BLE 0x1D /* BLE	opr 	--- Branch if <=0*/
#define BIP 0x1E /* BIP opr 	--- Branch if Positive/not negative*/
#define BIC 0x1F /* BIC	opr 	--- Branch if Carry*/
#define BNC 0x20 /* BNC opr 	--- Branch if not carry*/
#define JSR 0x21 /* JSR	opr,op1	--- Jump to Subroutine at immediate 24 bit address*/
#define RET 0x22 /* RET 		--- Pop status, page, register contents, etc, from call stack to return from subroutine*/
#define CPR 0x23 /* CPR			--- Compare A with D*/
#define RTI 0x24 /* RTI 		--- Return from interrupt, might be redundant with RET*/
#define TPR 0x25 /* TPR	r 		--- Transfer lower byte of selected register to PR (page register, for memory access)*/
#define BSR 0x26 /* BSR	r, opr  --- Bit shift register right by byte operand value*/
#define BSL 0x27 /* BSL	r, opr  --- Bit shift register left, opposite of BSR*/
#define HLT 0xFF /* HLT 		--- HALT, debug instruction, decooded as 0x7F because the register bit in instruction byte of word is masked out prior to decode and execute*/

#endif
