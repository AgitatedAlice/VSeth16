#bits 16
;#ruledef register
;{
;	D => 0x80
;	A => 0x00
;}

#ruledef
{
;0x00: /* NOP			*/
	nop => 0x0000
;0x01: /* LIB	r, opr	*/
	;LIB {r: register}, {value: i8} => (0x01 | r) @ value
	lib.a {value: i8} => 0x01 @ value
	lib.d {value: i8} => 0x81 @ value
;0x02: /* LIW	r, op1	*/
	;LIW {r: register}, {value: i16} => (0x02 || r) @ value
	liw.a {value: i16} => 0x0200 @ value
	liw.d {value: i16} => 0x8200 @ value
;0x03: /* LRA			*/ 
	lra => 0x0300
;0x04: /* LIA	opr,op1 */ 
	lia {p: i8}, {a:i16} => 0x04 @ p @ a
;0x05: /* PRA			*/ 
	pra => 0x0500
;0x06: /* PIA	opr,op1 */ 
	pia {p: i8}, {a:i16} => 0x06 @ p @ a 
;0x07: /* PSH	r		*/ 
	psh.a => 0x0700
	psh.d => 0x8700
;0x08: /* POP	r		*/ 
	pop.a => 0x0800
	pop.d => 0x8800
;0x09: /* PTM			*/ 
	ptm => 0x0900
;0x0A: /* STR			*/
	str => 0x0A00
;0x0B: /* PEEK	r		*/ 
	peek.a => 0x0B00
	peek.d => 0x8B00
;0x0C: /* PSL	op1		*/ 
	psl {value: i16} => 0x0C00 @ value
;0x0D: /* INR	r		*/ 
	ina => 0x0D00
	ind => 0x8D00
;0x0E: /* DCR	r		*/
	dca => 0x0E00
	dcd => 0x8E00
;0x0F: /* ADD	r		*/ 
	add.a => 0x0F00
	add.d => 0x8F00
;0x10: /* SUB	r		*/ 
	sub.a => 0x1000
	sub.d => 0x9000
;0x11: /* AND	r		*/ 
	and.a => 0x1100
	and.d => 0x9100
;0x12: /* OR	r		*/ 
	or.a => 0x1200
	or.d => 0x9200
;0x13: /* XOR	r		*/ 
	xor.a => 0x1300
	xor.d => 0x9300
;0x14: /* NOT	r		*/ 
	not.a => 0x1400
	not.d => 0x9400
;0x15: /* ADC	r		*/ 
	adc.a => 0x1500
	adc.d => 0x9500
;0x16: /* SBC	r		*/ 
	sbc.a => 0x1600
	sbc.d => 0x9600
;0x17: /* CMP	r		*/ 
	cmp.a => 0x1700
	cmp.d => 0x9700
;0x18: /* BEQ	opr		*/ 
	beq {value: s8} => 0x18 @ value
;0x19: /* BNE	opr		*/ 
	bne {value: s8} => 0x19 @ value
;0x1A: /* JMP	opr, op1*/ 
	jmp {p: i8}, {a:i16} => 0x1A @ p @ a
;0x1B: /* BIN	opr		*/ 
	bin {value: s8} => 0x1B @ value
;0x1C: /* BGE	opr		*/ 
	bge {value: s8} => 0x1C @ value
;0x1D: /* BLE	opr		*/ 
	ble {value: s8} => 0x1D @ value
;0x1E: /* BIP	opr		*/ 
	bip {value: s8} => 0x1E @ value
;0x1F: /* BIC	opr		*/ 
	bic {value: s8} => 0x1F @ value
;0x20: /* BNC	opr		*/ 
	bnc {value: s8} => 0x20 @ value
;0x21: /* JSR	opr,op1	*/ 
	jsr {p: i8}, {a:i16} => 0x21 @ p @ a
;0x22: /* RET			*/ 
	ret => 0x2200
;0x23: /* CPR			*/ 
	cpr => 0x2300
;0x24: /* RTI			*/ 
	rti => 0x2400
;0x25: /* TPR	r 		*/ 
	tpr.a => 0x2500
	tpr.d => 0xA500
;0x26: /* BSR	r, opr	*/ 
	bsr.a {value: u8} => 0x26 @ value
	bsr.d {value: u8} => 0xA6 @ value
;0x27: /* BSL	r, opr	*/
	bsl.a {value: u8} => 0x27 @ value
	bsl.a {value: u8} => 0xA7 @ value
;0xFF: /* HLT			*/
	hlt => 0xFF00
}