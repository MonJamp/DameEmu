#include "DameEmu.h"

#ifdef _DEBUG
#include <cstdio>
#define debug_msg(...) printf(__VA_ARGS__)
#else
#define debug_msg(x)
#endif

#define isAddHalfCarry(x, y) ((((x & 0xF) + (y & 0xF)) & 0x10) == 0x10)

#define nn (uint16_t)(IR & 0xFFFF)
#define n  (uint8_t)(IR & 0xFF)
#define e  (int8_t)(IR & 0xFF)

DameEmu::Instruction DameEmu::instructions[256] = {
	{"NOP", 1, &UNIMPLEMENTED},				//00
	{"LD BC, %04X", 3, &UNIMPLEMENTED},		//01
	{"LD (BC), A", 1, &UNIMPLEMENTED},		//02
	{"INC BC", 1, &UNIMPLEMENTED},			//03
	{"INC B", 1, &UNIMPLEMENTED},				//04
	{"DEC B", 1, &UNIMPLEMENTED},				//05
	{"LD B, %02X", 2, &UNIMPLEMENTED},		//06
	{"RLCA", 1, &UNIMPLEMENTED},				//07
	{"LD (%04X), SP", 3, &UNIMPLEMENTED},		//08
	{"ADD HL, BC", 1, &UNIMPLEMENTED},		//09
	{"LD A, (BC)", 1, &UNIMPLEMENTED},		//0A
	{"DEC BC", 1, &UNIMPLEMENTED},			//0B
	{"INC C", 1, &UNIMPLEMENTED},				//0C
	{"DEC C", 1, &UNIMPLEMENTED},				//0D
	{"LD C, %02X", 2, &UNIMPLEMENTED},		//0E
	{"RRCA", 1, &UNIMPLEMENTED},				//0F
	{"STOP", 1, &UNIMPLEMENTED},				//10
	{"LD DE, %04X", 3, &UNIMPLEMENTED},		//11
	{"LD (DE), A", 1, &UNIMPLEMENTED},		//12
	{"INC DE", 1, &UNIMPLEMENTED},			//13
	{"INC D", 1, &UNIMPLEMENTED},				//14
	{"DEC D", 1, &UNIMPLEMENTED},				//15
	{"LD D, %02X", 2, &UNIMPLEMENTED},		//16
	{"RLA", 1, &UNIMPLEMENTED},				//17
	{"JR %02X", 2, &UNIMPLEMENTED},			//18
	{"ADD HL, DE", 1, &UNIMPLEMENTED},		//19
	{"LD A, (DE)", 1, &UNIMPLEMENTED},		//1A
	{"DEC DE", 1, &UNIMPLEMENTED},			//1B
	{"INC E", 1, &UNIMPLEMENTED},				//1C
	{"DEC E", 1, &UNIMPLEMENTED},				//1D
	{"LD E, %02X", 2, &UNIMPLEMENTED},		//1E
	{"RRA", 1, &UNIMPLEMENTED},				//1F
	{"JR NZ, %02X", 2, &JR_NZ},			//20
	{"LD HL, %04X", 3, &UNIMPLEMENTED},		//21
	{"LD (HL+), A", 1, &UNIMPLEMENTED},		//22
	{"INC HL", 1, &UNIMPLEMENTED},			//23
	{"INC H", 1, &UNIMPLEMENTED},				//24
	{"DEC H", 1, &UNIMPLEMENTED},				//25
	{"LD H, %02X", 2, &UNIMPLEMENTED},		//26
	{"DAA", 1, &UNIMPLEMENTED},				//27
	{"JR Z, %02X", 2, &UNIMPLEMENTED},		//28
	{"ADD HL, HL", 1, &UNIMPLEMENTED},		//29
	{"LD A, (HL+)", 1, &UNIMPLEMENTED},		//2A
	{"DEC HL", 1, &UNIMPLEMENTED},			//2B
	{"INC L", 1, &UNIMPLEMENTED},				//2C
	{"DEC L", 1, &UNIMPLEMENTED},				//2D
	{"LD L, %02X", 2, &UNIMPLEMENTED},		//2E
	{"CPL", 1, &UNIMPLEMENTED},				//2F
	{"JR NC, %02X", 2, &UNIMPLEMENTED},		//30
	{"LD SP, %04X", 3, &UNIMPLEMENTED},		//31
	{"LD (HL-), A", 1, &UNIMPLEMENTED},		//32
	{"INC SP", 1, &UNIMPLEMENTED},			//33
	{"INC (HL)", 1, &UNIMPLEMENTED},			//34
	{"DEC (HL)", 1, &UNIMPLEMENTED},			//35
	{"LD (HL), %02X", 2, &UNIMPLEMENTED},		//36
	{"SCF", 1, &UNIMPLEMENTED},				//37
	{"JR C, %02X", 2, &UNIMPLEMENTED},		//38
	{"ADD HL, SP", 1, &UNIMPLEMENTED},		//39
	{"LD A, (HL-)", 1, &UNIMPLEMENTED},		//3A
	{"DEC SP", 1, &UNIMPLEMENTED},			//3B
	{"INC A", 1, &UNIMPLEMENTED},				//3C
	{"DEC A", 1, &UNIMPLEMENTED},				//3D
	{"LD A, %02X", 2, &UNIMPLEMENTED},		//3E
	{"CCF", 1, &UNIMPLEMENTED},				//3F
	{"LD B, B", 1, &UNIMPLEMENTED},			//40
	{"LD B, C", 1, &UNIMPLEMENTED},			//41
	{"LD B, D", 1, &UNIMPLEMENTED},			//42
	{"LD B, E", 1, &UNIMPLEMENTED},			//43
	{"LD B, H", 1, &UNIMPLEMENTED},			//44
	{"LD B, L", 1, &UNIMPLEMENTED},			//45
	{"LD B, (HL)", 1, &UNIMPLEMENTED},		//46
	{"LD B, A", 1, &UNIMPLEMENTED},			//47
	{"LD C, B", 1, &UNIMPLEMENTED},			//48
	{"LD C, C", 1, &UNIMPLEMENTED},			//49
	{"LD C, D", 1, &UNIMPLEMENTED},			//4A
	{"LD C, E", 1, &UNIMPLEMENTED},			//4B
	{"LD C, H", 1, &UNIMPLEMENTED},			//4C
	{"LD C, L", 1, &UNIMPLEMENTED},			//4D
	{"LD C, (HL)", 1, &UNIMPLEMENTED},		//4E
	{"LD C, A", 1, &UNIMPLEMENTED},			//4F
	{"LD D, B", 1, &UNIMPLEMENTED},			//50
	{"LD D, C", 1, &UNIMPLEMENTED},			//51
	{"LD D, D", 1, &UNIMPLEMENTED},			//52
	{"LD D, E", 1, &UNIMPLEMENTED},			//53
	{"LD D, H", 1, &UNIMPLEMENTED},			//54
	{"LD D, L", 1, &UNIMPLEMENTED},			//55
	{"LD D, (HL)", 1, &UNIMPLEMENTED},		//56
	{"LD D, A", 1, &UNIMPLEMENTED},			//57
	{"LD E, B", 1, &UNIMPLEMENTED},			//58
	{"LD E, C", 1, &UNIMPLEMENTED},			//59
	{"LD E, D", 1, &UNIMPLEMENTED},			//5A
	{"LD E, E", 1, &UNIMPLEMENTED},			//5B
	{"LD E, H", 1, &UNIMPLEMENTED},			//5C
	{"LD E, L", 1, &UNIMPLEMENTED},			//5D
	{"LD E, (HL)", 1, &UNIMPLEMENTED},		//5E
	{"LD E, A", 1, &UNIMPLEMENTED},			//5F
	{"LD H, B", 1, &UNIMPLEMENTED},			//60
	{"LD H, C", 1, &UNIMPLEMENTED},			//61
	{"LD H, D", 1, &UNIMPLEMENTED},			//62
	{"LD H, E", 1, &UNIMPLEMENTED},			//63
	{"LD H, H", 1, &UNIMPLEMENTED},			//64
	{"LD H, L", 1, &UNIMPLEMENTED},			//65
	{"LD H, (HL)", 1, &UNIMPLEMENTED},		//66
	{"LD H, A", 1, &UNIMPLEMENTED},			//67
	{"LD L, B", 1, &UNIMPLEMENTED},			//68
	{"LD L, C", 1, &UNIMPLEMENTED},			//69
	{"LD L, D", 1, &UNIMPLEMENTED},			//6A
	{"LD L, E", 1, &UNIMPLEMENTED},			//6B
	{"LD L, H", 1, &UNIMPLEMENTED},			//6C
	{"LD L, L", 1, &UNIMPLEMENTED},			//6D
	{"LD L, (HL)", 1, &UNIMPLEMENTED},		//6E
	{"LD L, A", 1, &UNIMPLEMENTED},			//6F
	{"LD (HL), B", 1, &UNIMPLEMENTED},		//70
	{"LD (HL), C", 1, &UNIMPLEMENTED},		//71
	{"LD (HL), D", 1, &UNIMPLEMENTED},		//72
	{"LD (HL), E", 1, &UNIMPLEMENTED},		//73
	{"LD (HL), H", 1, &UNIMPLEMENTED},		//74
	{"LD (HL), L", 1, &UNIMPLEMENTED},		//75
	{"HALT", 1, &UNIMPLEMENTED},				//76
	{"LD (HL), A", 1, &UNIMPLEMENTED},		//77
	{"LD A, B", 1, &UNIMPLEMENTED},			//78
	{"LD A, C", 1, &UNIMPLEMENTED},			//79
	{"LD A, D", 1, &UNIMPLEMENTED},			//7A
	{"LD A, E", 1, &UNIMPLEMENTED},			//7B
	{"LD A, H", 1, &UNIMPLEMENTED},			//7C
	{"LD A, L", 1, &UNIMPLEMENTED},			//7D
	{"LD A, (HL)", 1, &UNIMPLEMENTED},		//7E
	{"LD A, A", 1, &UNIMPLEMENTED},			//7F
	{"ADD B", 1, &UNIMPLEMENTED},				//80
	{"ADD C", 1, &UNIMPLEMENTED},				//81
	{"ADD D", 1, &UNIMPLEMENTED},				//82
	{"ADD E", 1, &UNIMPLEMENTED},				//83
	{"ADD H", 1, &UNIMPLEMENTED},				//84
	{"ADD L", 1, &UNIMPLEMENTED},				//85
	{"ADD (HL)", 1, &UNIMPLEMENTED},			//86
	{"ADD A", 1, &UNIMPLEMENTED},				//87
	{"ADC B", 1, &UNIMPLEMENTED},				//88
	{"ADC C", 1, &UNIMPLEMENTED},				//89
	{"ADC D", 1, &UNIMPLEMENTED},				//8A
	{"ADC E", 1, &UNIMPLEMENTED},				//8B
	{"ADC H", 1, &UNIMPLEMENTED},				//8C
	{"ADC L", 1, &UNIMPLEMENTED},				//8D
	{"ADC (HL)", 1, &UNIMPLEMENTED},			//8E
	{"ADC A", 1, &UNIMPLEMENTED},				//8F
	{"SUB B", 1, &UNIMPLEMENTED},				//90
	{"SUB C", 1, &UNIMPLEMENTED},				//91
	{"SUB D", 1, &UNIMPLEMENTED},				//92
	{"SUB E", 1, &UNIMPLEMENTED},				//93
	{"SUB H", 1, &UNIMPLEMENTED},				//94
	{"SUB L", 1, &UNIMPLEMENTED},				//95
	{"SUB (HL)", 1, &UNIMPLEMENTED},			//96
	{"SUB A", 1, &UNIMPLEMENTED},				//97
	{"SBC B", 1, &UNIMPLEMENTED},				//98
	{"SBC C", 1, &UNIMPLEMENTED},				//99
	{"SBC D", 1, &UNIMPLEMENTED},				//9A
	{"SBC E", 1, &UNIMPLEMENTED},				//9B
	{"SBC H", 1, &UNIMPLEMENTED},				//9C
	{"SBC L", 1, &UNIMPLEMENTED},				//9D
	{"SBC (HL)", 1, &UNIMPLEMENTED},			//9E
	{"SBC A", 1, &UNIMPLEMENTED},				//9F
	{"AND B", 1, &UNIMPLEMENTED},				//A0
	{"AND C", 1, &UNIMPLEMENTED},				//A1
	{"AND D", 1, &UNIMPLEMENTED},				//A2
	{"AND E", 1, &UNIMPLEMENTED},				//A3
	{"AND H", 1, &UNIMPLEMENTED},				//A4
	{"AND L", 1, &UNIMPLEMENTED},				//A5
	{"AND (HL)", 1, &UNIMPLEMENTED},			//A6
	{"AND A", 1, &UNIMPLEMENTED},				//A7
	{"XOR B", 1, &UNIMPLEMENTED},				//A8
	{"XOR C", 1, &UNIMPLEMENTED},				//A9
	{"XOR D", 1, &UNIMPLEMENTED},				//AA
	{"XOR E", 1, &UNIMPLEMENTED},				//AB
	{"XOR H", 1, &UNIMPLEMENTED},				//AC
	{"XOR L", 1, &UNIMPLEMENTED},				//AD
	{"XOR (HL)", 1, &UNIMPLEMENTED},			//AE
	{"XOR A", 1, &UNIMPLEMENTED},				//AF
	{"OR B", 1, &UNIMPLEMENTED},				//B0
	{"OR C", 1, &UNIMPLEMENTED},				//B1
	{"OR D", 1, &UNIMPLEMENTED},				//B2
	{"OR E", 1, &UNIMPLEMENTED},				//B3
	{"OR H", 1, &UNIMPLEMENTED},				//B4
	{"OR L", 1, &UNIMPLEMENTED},				//B5
	{"OR (HL)", 1, &UNIMPLEMENTED},			//B6
	{"OR A", 1, &UNIMPLEMENTED},				//B7
	{"CP B", 1, &UNIMPLEMENTED},				//B8
	{"CP C", 1, &UNIMPLEMENTED},				//B9
	{"CP D", 1, &UNIMPLEMENTED},				//BA
	{"CP E", 1, &UNIMPLEMENTED},				//BB
	{"CP H", 1, &UNIMPLEMENTED},				//BC
	{"CP L", 1, &UNIMPLEMENTED},				//BD
	{"CP (HL)", 1, &UNIMPLEMENTED},			//BE
	{"CP A", 1, &UNIMPLEMENTED},				//BF
	{"RET NZ", 1, &UNIMPLEMENTED},			//C0
	{"POP BC", 1, &UNIMPLEMENTED},			//C1
	{"JP NZ, %04X", 3, &UNIMPLEMENTED},		//C2
	{"JP %04X", 3, &UNIMPLEMENTED},			//C3
	{"CALL NZ, %04X", 3, &UNIMPLEMENTED},		//C4
	{"PUSH BC", 1, &UNIMPLEMENTED},			//C5
	{"ADD %02X", 2, &UNIMPLEMENTED},			//C6
	{"RST 0x00", 1, &UNIMPLEMENTED},			//C7
	{"RET Z", 1, &UNIMPLEMENTED},				//C8
	{"RET", 1, &UNIMPLEMENTED},				//C9
	{"JP Z, %04X", 3, &UNIMPLEMENTED},		//CA
	{"CB %02X - ", 2, &UNIMPLEMENTED},		//CB
	{"CALL Z, %04X", 3, &UNIMPLEMENTED},		//CC
	{"CALL %04X", 3, &UNIMPLEMENTED},			//CD
	{"ADC %02X", 2, &UNIMPLEMENTED},			//CE
	{"RST 0x08", 1, &UNIMPLEMENTED},			//CF
	{"RET NC", 1, &UNIMPLEMENTED},			//D0
	{"POP DE", 1, &UNIMPLEMENTED},			//D1
	{"JP NC, %04X", 3, &UNIMPLEMENTED},		//D2
	{"Undefined OP", 1, &UNDEFINED},	//D3
	{"CALL NC, %04X", 3, &UNIMPLEMENTED},		//D4
	{"PUSH DE", 1, &UNIMPLEMENTED},			//D5
	{"SUB %02X", 2, &UNIMPLEMENTED},			//D6
	{"RST 0x10", 1, &UNIMPLEMENTED},			//D7
	{"RET C", 1, &UNIMPLEMENTED},				//D8
	{"RETI", 1, &UNIMPLEMENTED},				//D9
	{"JP C, %04X", 3, &UNIMPLEMENTED},		//DA
	{"Undefined OP", 1, &UNDEFINED},	//DB
	{"CALL C, %04X", 3, &UNIMPLEMENTED},		//DC
	{"Undefined OP", 1, &UNDEFINED},	//DD
	{"SBC %02X", 2, &UNIMPLEMENTED},			//DE
	{"RST 0x18", 1, &UNIMPLEMENTED},			//DF
	{"LD (FF00+%02X), A", 2, &UNIMPLEMENTED},	//E0
	{"POP HL", 1, &UNIMPLEMENTED},			//E1
	{"LD (FF00+C), A", 1, &UNIMPLEMENTED},	//E2
	{"Undefined OP", 1, &UNDEFINED},	//E3
	{"Undefined OP", 1, &UNDEFINED},	//E4
	{"PUSH HL", 1, &UNIMPLEMENTED},			//E5
	{"AND %02X", 2, &UNIMPLEMENTED},			//E6
	{"RST 0x20", 1, &UNIMPLEMENTED},			//E7
	{"ADD SP, %02X", 2, &UNIMPLEMENTED},		//E8
	{"JP HL", 1, &UNIMPLEMENTED},				//E9
	{"LD (%04X), A", 3, &UNIMPLEMENTED},		//EA
	{"Undefined OP", 1, &UNDEFINED},	//EB
	{"Undefined OP", 1, &UNDEFINED},	//EC
	{"Undefined OP", 1, &UNDEFINED},	//ED
	{"XOR %02X", 2, &UNIMPLEMENTED},			//EE
	{"RST 0x28", 1, &UNIMPLEMENTED},			//EF
	{"LD A, (FF00+%02X)", 2, &UNIMPLEMENTED},	//F0
	{"POP AF", 1, &UNIMPLEMENTED},			//F1
	{"LD A, (FF00+C)", 1, &UNIMPLEMENTED},	//F2
	{"DI", 1, &UNIMPLEMENTED},				//F3
	{"Undefined OP", 1, &UNDEFINED},	//F4
	{"PUSH AF", 1, &UNIMPLEMENTED},			//F5
	{"OR %02X", 2, &UNIMPLEMENTED},			//F6
	{"RST 0x30", 1, &UNIMPLEMENTED},			//F7
	{"LD HL, SP+%02X", 2, &UNIMPLEMENTED},	//F8
	{"LD SP, HL", 1, &UNIMPLEMENTED},			//F9
	{"LD A, (%04X)", 3, &UNIMPLEMENTED},		//FA
	{"EI", 1, &UNIMPLEMENTED},				//FB
	{"Undefined OP", 1, &UNDEFINED},	//FC
	{"Undefined OP", 1, &UNDEFINED},	//FD
	{"CP %02X", 2, &UNIMPLEMENTED},			//FE
	{"RST 0x38", 1, &UNIMPLEMENTED},			//FF
};

void DameEmu::UNDEFINED() {
	debug_msg("\nUndefined instruction!");

	status = HALT;
}

void DameEmu::UNIMPLEMENTED() {
	debug_msg("\nUnimplemented instruction! OP: %06X", IR);
	
	status = HALT;
}

void DameEmu::LD_r_n(uint8_t& r) {
	r = n;

	cycles += 2;
}

void DameEmu::LD_HL_n() {
	memory[HL] = n;

	cycles += 3;
}

void DameEmu::LD_r_r(uint8_t& r1, uint8_t& r2) {
	r1 = r2;

	cycles += 1;
}

void DameEmu::LD_r_HL(uint8_t& r) {
	r = memory[HL];

	cycles += 2;
}

void DameEmu::LD_HL_r(uint8_t& r) {
	memory[HL] = r;

	cycles += 2;
}

void DameEmu::LD_BC() {
	BC = nn;

	cycles += 3;
}

void DameEmu::LD_BC_A() {
	memory[BC] = A;

	cycles += 2;
}

void DameEmu::LD_DE() {
	DE = nn;

	cycles += 3;
}

void DameEmu::LD_DE_A() {
	memory[DE] = A;

	cycles += 2;
}

void DameEmu::JR_NZ() {
	cycles += 2;

	if (FLAG_CHECK(FLAG_ZERO) == 0) {
		cycles += 1;
		PC += e;

		debug_msg("True, PC <- %02X\n", PC);
	}
	else {
		debug_msg("False, PC <- %02X\n", PC);
	}
}

void DameEmu::LD_HL() {
	HL = nn;

	cycles += 3;
}

void DameEmu::LD_HLI_A() {
	memory[HL] = A;
	HL++;

	cycles += 2;
}

void DameEmu::LD_SP() {
	SP = nn;

	cycles += 3;
}

void DameEmu::LD_HLD_A() {
	memory[HL] = A;
	HL--;

	cycles += 2;
}

void DameEmu::INC(uint8_t& r) {
	isAddHalfCarry(r, 1) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	r++;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::INC_HL() {
	isAddHalfCarry(memory[HL], 1) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	memory[HL]++;
	(memory[HL] == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 3;
}

void DameEmu::AND_B() {
	A &= B;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::AND_C() {
	A &= C;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::AND_D() {
	A &= D;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::AND_E() {
	A &= E;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::AND_H() {
	A &= H;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::AND_L() {
	A &= L;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::AND_HL() {
	A &= memory[HL];

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 2;
}

void DameEmu::AND_A() {
	A &= A;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::XOR_B() {
	A ^= B;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::XOR_C() {
	A ^= C;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::XOR_D() {
	A ^= D;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::XOR_E() {
	A ^= E;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::XOR_H() {
	A ^= H;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::XOR_L() {
	A ^= L;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::XOR_HL() {
	A ^= (HL);

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 2;
}

void DameEmu::XOR_A() {
	A ^= A;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::LD_C_A() {
	memory[0xFF00 + C] = A;

	cycles += 2;
}

void DameEmu::LD_A_C() {
	A = memory[0xFF00 + C];

	cycles += 2;
}

//CB Instructions
void DameEmu::BIT(uint8_t b, uint8_t& r) {
	(r & (1 << b)) ? FLAG_CLEAR(FLAG_ZERO) : FLAG_SET(FLAG_ZERO);

	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 2;
}

void DameEmu::BIT_HL(uint8_t b) {
	(memory[HL] & (1 << b)) ? FLAG_CLEAR(FLAG_ZERO) : FLAG_SET(FLAG_ZERO);

	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 3;
}