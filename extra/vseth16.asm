#bits 16
#ruledef register
{
	D => 0x80
	A => 0x00
}

#ruledef
{
;0x00: /* NOP			*/
	nop => 0x0000
;0x01: /* LIB	r, opr	*/
	;LIB {r: register}, {value: i8} => (0x01 | r) @ value`8
	lib.a {value: i8} => 0x01 @ value
	lib.d {value: i8} => 0x81 @ value
;0x02: /* LIW	r, op1	*/
	;LIW {r: register}, {value: i16} => (0x02 || r) @ value`16
	liw.a {value: i16} => 0x0200 @ value
	liw.d {value: i16} => 0x8200 @ value
;0x03: /* LRA			*/ 
	lra => 0x0300
;0x04: /* LIA	opr,op1 */ 
	lia {p: i8}, {a:i16} => 0x04 @ p @ a
;0x05: /* PRA			*/ 
	pra => 0x0500
;0x06: /* PIA	opr,op1 */ 
	
;0x07: /* PSH	r		*/ 
;0x08: /* POP	r		*/ 
;0x09: /* PTM			*/ 
;0x0A: /* STR			*/ 
;0x0B: /* PEEK	r		*/ 
;0x0C: /* PSL	op1		*/ 
;0x0D: /* INR	r		*/ 
;0x0E: /* DCR	r		*/ 
;0x0F: /* ADD	r		*/ 
;0x10: /* SUB	r		*/ 
;0x11: /* AND	r		*/ 
;0x12: /* OR	r		*/ 
;0x13: /* XOR	r		*/ 
;0x14: /* NOT	r		*/ 
;0x15: /* ADC	r		*/ 
;0x16: /* SBC	r		*/ 
;0x17: /* CMP	r		*/ 
;0x18: /* BEQ	opr		*/ 
;0x19: /* BNE	opr		*/ 
;0x1A: /* JMP	opr, op1*/ 
;0x1B: /* BIN	opr		*/ 
;0x1C: /* BGE	opr		*/ 
;0x1D: /* BLE	opr		*/ 
;0x1E: /* BIP	opr		*/ 
;0x1F: /* BIC	opr		*/ 
;0x20: /* BNC	opr		*/ 
;0x21: /* JSR	opr,op1	*/ 
;0x22: /* RET			*/ 
;0x23: /* CPR			*/ 
;0x24: /* RTI			*/ 
;0x25: /* TPR	r 		*/ 
;0x26: /* BSR	r, opr	*/ 
;0x27: /* BSL	r, opr	*/
;0x7F: /* HLT			*/ 
}