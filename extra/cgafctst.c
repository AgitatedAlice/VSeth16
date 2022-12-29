#include <stdio.h>
#include "cgafc.h"
#include "aautil.h"

int main() {
	int ri = 0;
	
	
	ASTM16 test = ASTM_Init(0, 0);
	for(uint8_t __i = 0; __i < 0x10; __i++){
		test.MEM[__i] = InitAPage('m');
	}
	
	//test.MEM[1].t = 'r'; // set 1st page as ROM for testing purposes
	
	printf("I am stack woman i build the stacks \n");
	
	test.MEM[0].d[0x0000] = 0x0120; // LIB A, 0x20
	test.MEM[0].d[0x0001] = 0x8200; test.MEM[0].d[0x0002] = 0xDEAD; // LIW D, 0xDEAD
	test.MEM[0].d[0x0003] = 0x0A00; // STR *A, D (0x0020, 0xDEAD)
	test.MEM[0].d[0x0004] = 0x0C00; test.MEM[0].d[0x0005] = 0xBEEF; // PSL 0xBEEF
	test.MEM[0].d[0x0006] = 0x0C00; test.MEM[0].d[0x0007] = 0xDEAD; // PSL 0xDEAD
	test.MEM[0].d[0x0008] = 0x0200; test.MEM[0].d[0x0009] = 0xF00F; // LIW A, 0xF00F
	test.MEM[0].d[0x000A] = 0x0900; // PTM
	test.MEM[0].d[0x000B] = 0x0D00; // INR A
	test.MEM[0].d[0x000C] = 0x0900; // PTM
	test.MEM[0].d[0x000D] = 0x0C00; test.MEM[0].d[0x000E] = 0x0200; // PSL 0x0100
	test.MEM[0].d[0x000F] = 0x01FF; // LIB A, 0xFF
	test.MEM[0].d[0x0010] = 0x8101; // LIB D, 0x01
	test.MEM[0].d[0x0011] = 0x9000; // SUB D (stack top init 0x0100 - 2)
	test.MEM[0].d[0x0012] = 0x1700; // CMP A
	//test.MEM[0].d[0x0013] = 0x1464; // BEQ +237
	test.MEM[0].d[0x0013] = 0x19FD; // BNE -3
	test.MEM[0].d[0x0014] = 0x1A00; test.MEM[0].d[0x0015] = 0x0077; // JMP 0x0077
	test.MEM[0].d[0x0077] = 0x0C00; test.MEM[0].d[0x0078] = 0x0666; // PSL 0x0666
	test.MEM[0].d[0x0079] = 0x1A00; test.MEM[0].d[0x007A] = 0x1000; // JMP 0x1000
	
	test.MEM[0].d[0x1000] = 0x0C00; test.MEM[0].d[0x1001] = 0x0999; // PSL 0x0999
	test.MEM[0].d[0x1002] = 0x2100; test.MEM[0].d[0x1003] = 0x2000; // JSR 0x0_0x2000
	test.MEM[0].d[0x1004] = 0x0C00; test.MEM[0].d[0x1005] = 0xBEE7; // PSL 0xBEE7
	test.MEM[0].d[0x1006] = 0x2101; test.MEM[0].d[0x1007] = 0x0100; // JSR 0x1_0x0100
	// now time to test what happens if we try to write to ROM, or out of initialized range
	test.MEM[0].d[0x1008] = 0x8101; // LIB D 0x01;
	test.MEM[0].d[0x1009] = 0xA500; // TPR A;
	test.MEM[0].d[0x100A] = 0x0200; test.MEM[0].d[0x100B] = 0x0101; // LIW A, 0x0101;
	test.MEM[0].d[0x100C] = 0x8200; test.MEM[0].d[0x100D] = 0xBEEF; // LIW D, 0xBEEF;
	test.MEM[0].d[0x100E] = 0x0A00; // STR
	test.MEM[0].d[0x100F] = 0x2101; test.MEM[0].d[0x1010] = 0x0100;
	test.MEM[0].d[0x1011] = 0x04BE; test.MEM[0].d[0x1012] = 0xBEEF; // LIA 0xBE_0xBEEF;
	test.MEM[0].d[0x1013] = 0xFFFF; // HALT!!!!!
	
	//test.MEM[0].d[0x1008] = 0xFFFF; // HALT!!!!!
	
	
	// subroutine test
	test.MEM[0].d[0x2000] = 0x010A; // LIB A, 0x0A
	test.MEM[0].d[0x2001] = 0x8100; // LIB D, 0x00
	test.MEM[0].d[0x2002] = 0x0C00; test.MEM[0].d[0x2003] = 0xFEEF; // PSL 0xFEEF
	test.MEM[0].d[0x2004] = 0x8D00; // INR D
	test.MEM[0].d[0x2005] = 0x2300; // CPR
	test.MEM[0].d[0x2006] = 0x19FC; // BNE -4 (0x2002)
	test.MEM[0].d[0x2007] = 0x0C00; test.MEM[0].d[0x2008] = 0xFAAF; // PSL 0xFAAF
	test.MEM[0].d[0x2009] = 0x2200; // RET
	
	// off-page srt test
	test.MEM[1].d[0x0100] = 0x0C00; test.MEM[1].d[0x0101] = 0x1337; // PSL 0x1337; cool number! 8)
	// maybe try and trigger a segfault to check if we can kill the illegal access error?
	test.MEM[1].d[0x0102] = 0x2200; // RET
	
	// maskable interrupt code
	// set IRQ vector at 0xFFFA to the start of this
	test.MEM[0].d[0x0100] = 0x0700; // PSH A
	test.MEM[0].d[0x0101] = 0x8700; // PSH D
	test.MEM[0].d[0x0102] = 0x0200; test.MEM[0].d[0x0103] = 0xF00F; // LIW A 0xF00F
	test.MEM[0].d[0x0104] = 0x0C00; test.MEM[0].d[0x0105] = 0xBEEB; // PLI 0xBEEB
	test.MEM[0].d[0x0106] = 0x0900; // PTM (pop to *A=0xF01F)
	test.MEM[0].d[0x0107] = 0x8800; // POP D
	test.MEM[0].d[0x0108] = 0x0800; // POP A
	test.MEM[0].d[0x0109] = 0x2400; // RTI
	test.MEM[0].d[0xFFFA] = 0x0100;
	// nmi test
	test.MEM[0].d[0x0200] = 0x0C00; test.MEM[0].d[0x0201] = 0xDEAD;
	test.MEM[0].d[0x0202] = 0x0C00; test.MEM[0].d[0x0203] = 0xDEAD;
	test.MEM[0].d[0x0204] = 0x0C00; test.MEM[0].d[0x0205] = 0xDEAD;
	
	test.MEM[0].d[0xFFFC] = 0x0200; // NMI@0x0200
	
	// After execution address 0xF00F-0xF010 should contain DEADBEEF
	
	
	// now with our "painfully" hand assembled test program loaded into memory, lets tick through each instruction printing PC, A and D as we go :)
	/*printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);
	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);
	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);
	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);
	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);
	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);
	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);
	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);
	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	ASTM_tick(&test);*/
	int cc = 0; int rs = 0; uint8_t ctrl = 0x00;
	while((rs==0) && cc < 50000003){
		if(cc == 1000){ctrl=0b00000010;} //if(cc == 1004){ctrl=0b00000100;}
		//printf("%i %i PC:  %#04hhx_%#06hx A: %#06hx D: %#06hx SR: %#04hhx PEEKTOP: %#06hx SP: %#04hhx CTRL: %#04hhx\n", rs, cc, test.PC.p, test.PC.a, test.r[0], test.r[1], test.SR, test.sX.mem[test.sX.p-1], test.sX.p, ctrl);
		rs = ASTM_tick(&test, ctrl); ctrl = 0x00;
		
		cc++;
	}
	
	printf("%i %i PC: %#04hhx _ %#06hx A: %#06hx D: %#06hx SR: %#04hhx PEEKTOP: %#06hx SP: %#04hhx CTRL: %#04hhx\n", rs, cc,test.PC.p, test.PC.a, test.r[0], test.r[1], test.SR, test.sX.mem[test.sX.p-1], test.sX.p, ctrl);
	printf("0xF00F: %#06hx\n 0xF010: %#06hx\n", test.MEM[0].d[0xF00F], test.MEM[0].d[0xF010]);
	
	S816_DUMP(&test.sX);
	
	//for(int i = 0; i < 10; i++){
	//	ASTM_tick(&test);
	//	printf("PC: %#06hx A: %#06hx D: %#06hx\n", test.PC, test.r[0], test.r[1]);
	//}
	
	return ri;
}