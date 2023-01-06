#ifndef VSETH16_ASM
#define VSETH16_ASM

#define NOP 0x00

#define LIB 0x01 /* LIB r, opr	*/
#define LIW 0x02 /* LIW r, op1	*/
#define LRA 0x03 /* LRA */
#define LIA 0x04 /* LIA opr,op1 */
#define PRA 0x05 /* PRA	*/
#define PIA 0x06 /* PIA opr,op1 */
#define PSH 0x07 /* PSH r */
#define POP 0x08 /* POP r */ 
#define PTM 0x09 /* PTM */
#define STR 0x0A /* STR */
#define PEEK 0x0B /* PEEK r */
#define PSL 0x0C /* PSL	op1 */
#define INR 0x0D /* INR r */
#define DCR 0x0E /* DCR	r */
#define ADD 0x0F /* ADD	r */
#define SUB 0x10 /* SUB	r */
#define AND 0x11 /* AND	r */ 
#define OR 0x12 /* OR r */
#define XOR 0x13 /* XOR	r */
#define NOT 0x14 /* NOT	r */
#define ADC 0x15 /* ADC	r */ 
#define SBC 0x16 /* SBC */
#define CMP 0x17 /* CMP	r */
#define BEQ 0x18 /* BEQ	opr */
#define BNE 0x19 /* BNE	opr */
#define JMP 0x1A /* JMP	opr, op1 */
#define BIN 0x1B /* BIN	opr */
#define BGE 0x1C /* BGE	opr */
#define BLE 0x1D /* BLE	opr */
#define BIP 0x1E /* BIP opr */
#define BIC 0x1F /* BIC	opr */
#define BNC 0x20 /* BNC opr */
#define JSR 0x21 /* JSR	opr,op1	*/
#define RET 0x22 /* RET */
#define CPR 0x23 /* CPR	*/
#define RTI 0x24 /* RTI */
#define TPR 0x25 /* TPR	r */
#define BSR 0x26 /* BSR	r, opr */
#define BSL 0x27 /* BSL	r, opr */
#define HLT 0x7F /* HLT */

#endif
