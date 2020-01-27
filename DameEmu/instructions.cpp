#include "DameEmu.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#define debug_msg(x) cout << x
#else
#define debug_msg(x)
#endif

#define isAddHalfCarry(x, y) ((((x & 0xF) + (y & 0xF)) & 0x10) == 0x10)

#define nn (uint16_t)(memory[PC + 2] << 8 | memory[PC + 1])
#define n (uint8_t)(memory[PC + 1])
#define e (int8_t)(memory[PC + 1])

DameEmu::Instruction DameEmu::instructions[256] = {
	{"NOP", 1, &UNKNOWN},				//00
	{"LD BC, %04X", 3, &UNKNOWN},		//01
	{"LD (BC), A", 1, &UNKNOWN},		//02
	{"INC BC", 1, &UNKNOWN},			//03
	{"INC B", 1, &UNKNOWN},				//04
	{"DEC B", 1, &UNKNOWN},				//05
	{"LD B, %02X", 2, &UNKNOWN},		//06
	{"RLCA", 1, &UNKNOWN},				//07
	{"LD (%04X), SP", 3, &UNKNOWN},		//08
	{"ADD HL, BC", 1, &UNKNOWN},		//09
	{"LD A, (BC)", 1, &UNKNOWN},		//0A
	{"DEC BC", 1, &UNKNOWN},			//0B
	{"INC C", 1, &UNKNOWN},				//0C
	{"DEC C", 1, &UNKNOWN},				//0D
	{"LD C, %02X", 2, &UNKNOWN},		//0E
	{"RRCA", 1, &UNKNOWN},				//0F
	{"STOP", 1, &UNKNOWN},				//10
	{"LD DE, %04X", 3, &UNKNOWN},		//11
	{"LD (DE), A", 1, &UNKNOWN},		//12
	{"INC DE", 1, &UNKNOWN},			//13
	{"INC D", 1, &UNKNOWN},				//14
	{"DEC D", 1, &UNKNOWN},				//15
	{"LD D, %02X", 2, &UNKNOWN},		//16
	{"RLA", 1, &UNKNOWN},				//17
	{"JR %02X", 2, &UNKNOWN},			//18
	{"ADD HL, DE", 1, &UNKNOWN},		//19
	{"LD A, (DE)", 1, &UNKNOWN},		//1A
	{"DEC DE", 1, &UNKNOWN},			//1B
	{"INC E", 1, &UNKNOWN},				//1C
	{"DEC E", 1, &UNKNOWN},				//1D
	{"LD E, %02X", 2, &UNKNOWN},		//1E
	{"RRA", 1, &UNKNOWN},				//1F
	{"JR NZ, %02X", 2, &JR_NZ},			//20
	{"LD HL, %04X", 3, &UNKNOWN},		//21
	{"LD (HL+), A", 1, &UNKNOWN},		//22
	{"INC HL", 1, &UNKNOWN},			//23
	{"INC H", 1, &UNKNOWN},				//24
	{"DEC H", 1, &UNKNOWN},				//25
	{"LD H, %02X", 2, &UNKNOWN},		//26
	{"DAA", 1, &UNKNOWN},				//27
	{"JR Z, %02X", 2, &UNKNOWN},		//28
	{"ADD HL, HL", 1, &UNKNOWN},		//29
	{"LD A, (HL+)", 1, &UNKNOWN},		//2A
	{"DEC HL", 1, &UNKNOWN},			//2B
	{"INC L", 1, &UNKNOWN},				//2C
	{"DEC L", 1, &UNKNOWN},				//2D
	{"LD L, %02X", 2, &UNKNOWN},		//2E
	{"CPL", 1, &UNKNOWN},				//2F
	{"JR NC, %02X", 2, &UNKNOWN},		//30
	{"LD SP, %04X", 3, &UNKNOWN},		//31
	{"LD (HL-), A", 1, &UNKNOWN},		//32
	{"INC SP", 1, &UNKNOWN},			//33
	{"INC (HL)", 1, &UNKNOWN},			//34
	{"DEC (HL)", 1, &UNKNOWN},			//35
	{"LD (HL), %02X", 2, &UNKNOWN},		//36
	{"SCF", 1, &UNKNOWN},				//37
	{"JR C, %02X", 2, &UNKNOWN},		//38
	{"ADD HL, SP", 1, &UNKNOWN},		//39
	{"LD A, (HL-)", 1, &UNKNOWN},		//3A
	{"DEC SP", 1, &UNKNOWN},			//3B
	{"INC A", 1, &UNKNOWN},				//3C
	{"DEC A", 1, &UNKNOWN},				//3D
	{"LD A, %02X", 2, &UNKNOWN},		//3E
	{"CCF", 1, &UNKNOWN},				//3F
	{"LD B, B", 1, &UNKNOWN},			//40
	{"LD B, C", 1, &UNKNOWN},			//41
	{"LD B, D", 1, &UNKNOWN},			//42
	{"LD B, E", 1, &UNKNOWN},			//43
	{"LD B, H", 1, &UNKNOWN},			//44
	{"LD B, L", 1, &UNKNOWN},			//45
	{"LD B, (HL)", 1, &UNKNOWN},		//46
	{"LD B, A", 1, &UNKNOWN},			//47
	{"LD C, B", 1, &UNKNOWN},			//48
	{"LD C, C", 1, &UNKNOWN},			//49
	{"LD C, D", 1, &UNKNOWN},			//4A
	{"LD C, E", 1, &UNKNOWN},			//4B
	{"LD C, H", 1, &UNKNOWN},			//4C
	{"LD C, L", 1, &UNKNOWN},			//4D
	{"LD C, (HL)", 1, &UNKNOWN},		//4E
	{"LD C, A", 1, &UNKNOWN},			//4F
	{"LD D, B", 1, &UNKNOWN},			//50
	{"LD D, C", 1, &UNKNOWN},			//51
	{"LD D, D", 1, &UNKNOWN},			//52
	{"LD D, E", 1, &UNKNOWN},			//53
	{"LD D, H", 1, &UNKNOWN},			//54
	{"LD D, L", 1, &UNKNOWN},			//55
	{"LD D, (HL)", 1, &UNKNOWN},		//56
	{"LD D, A", 1, &UNKNOWN},			//57
	{"LD E, B", 1, &UNKNOWN},			//58
	{"LD E, C", 1, &UNKNOWN},			//59
	{"LD E, D", 1, &UNKNOWN},			//5A
	{"LD E, E", 1, &UNKNOWN},			//5B
	{"LD E, H", 1, &UNKNOWN},			//5C
	{"LD E, L", 1, &UNKNOWN},			//5D
	{"LD E, (HL)", 1, &UNKNOWN},		//5E
	{"LD E, A", 1, &UNKNOWN},			//5F
	{"LD H, B", 1, &UNKNOWN},			//60
	{"LD H, C", 1, &UNKNOWN},			//61
	{"LD H, D", 1, &UNKNOWN},			//62
	{"LD H, E", 1, &UNKNOWN},			//63
	{"LD H, H", 1, &UNKNOWN},			//64
	{"LD H, L", 1, &UNKNOWN},			//65
	{"LD H, (HL)", 1, &UNKNOWN},		//66
	{"LD H, A", 1, &UNKNOWN},			//67
	{"LD L, B", 1, &UNKNOWN},			//68
	{"LD L, C", 1, &UNKNOWN},			//69
	{"LD L, D", 1, &UNKNOWN},			//6A
	{"LD L, E", 1, &UNKNOWN},			//6B
	{"LD L, H", 1, &UNKNOWN},			//6C
	{"LD L, L", 1, &UNKNOWN},			//6D
	{"LD L, (HL)", 1, &UNKNOWN},		//6E
	{"LD L, A", 1, &UNKNOWN},			//6F
	{"LD (HL), B", 1, &UNKNOWN},		//70
	{"LD (HL), C", 1, &UNKNOWN},		//71
	{"LD (HL), D", 1, &UNKNOWN},		//72
	{"LD (HL), E", 1, &UNKNOWN},		//73
	{"LD (HL), H", 1, &UNKNOWN},		//74
	{"LD (HL), L", 1, &UNKNOWN},		//75
	{"HALT", 1, &UNKNOWN},				//76
	{"LD (HL), A", 1, &UNKNOWN},		//77
	{"LD A, B", 1, &UNKNOWN},			//78
	{"LD A, C", 1, &UNKNOWN},			//79
	{"LD A, D", 1, &UNKNOWN},			//7A
	{"LD A, E", 1, &UNKNOWN},			//7B
	{"LD A, H", 1, &UNKNOWN},			//7C
	{"LD A, L", 1, &UNKNOWN},			//7D
	{"LD A, (HL)", 1, &UNKNOWN},		//7E
	{"LD A, A", 1, &UNKNOWN},			//7F
	{"ADD B", 1, &UNKNOWN},				//80
	{"ADD C", 1, &UNKNOWN},				//81
	{"ADD D", 1, &UNKNOWN},				//82
	{"ADD E", 1, &UNKNOWN},				//83
	{"ADD H", 1, &UNKNOWN},				//84
	{"ADD L", 1, &UNKNOWN},				//85
	{"ADD (HL)", 1, &UNKNOWN},			//86
	{"ADD A", 1, &UNKNOWN},				//87
	{"ADC B", 1, &UNKNOWN},				//88
	{"ADC C", 1, &UNKNOWN},				//89
	{"ADC D", 1, &UNKNOWN},				//8A
	{"ADC E", 1, &UNKNOWN},				//8B
	{"ADC H", 1, &UNKNOWN},				//8C
	{"ADC L", 1, &UNKNOWN},				//8D
	{"ADC (HL)", 1, &UNKNOWN},			//8E
	{"ADC A", 1, &UNKNOWN},				//8F
	{"SUB B", 1, &UNKNOWN},				//90
	{"SUB C", 1, &UNKNOWN},				//91
	{"SUB D", 1, &UNKNOWN},				//92
	{"SUB E", 1, &UNKNOWN},				//93
	{"SUB H", 1, &UNKNOWN},				//94
	{"SUB L", 1, &UNKNOWN},				//95
	{"SUB (HL)", 1, &UNKNOWN},			//96
	{"SUB A", 1, &UNKNOWN},				//97
	{"SBC B", 1, &UNKNOWN},				//98
	{"SBC C", 1, &UNKNOWN},				//99
	{"SBC D", 1, &UNKNOWN},				//9A
	{"SBC E", 1, &UNKNOWN},				//9B
	{"SBC H", 1, &UNKNOWN},				//9C
	{"SBC L", 1, &UNKNOWN},				//9D
	{"SBC (HL)", 1, &UNKNOWN},			//9E
	{"SBC A", 1, &UNKNOWN},				//9F
	{"AND B", 1, &UNKNOWN},				//A0
	{"AND C", 1, &UNKNOWN},				//A1
	{"AND D", 1, &UNKNOWN},				//A2
	{"AND E", 1, &UNKNOWN},				//A3
	{"AND H", 1, &UNKNOWN},				//A4
	{"AND L", 1, &UNKNOWN},				//A5
	{"AND (HL)", 1, &UNKNOWN},			//A6
	{"AND A", 1, &UNKNOWN},				//A7
	{"XOR B", 1, &UNKNOWN},				//A8
	{"XOR C", 1, &UNKNOWN},				//A9
	{"XOR D", 1, &UNKNOWN},				//AA
	{"XOR E", 1, &UNKNOWN},				//AB
	{"XOR H", 1, &UNKNOWN},				//AC
	{"XOR L", 1, &UNKNOWN},				//AD
	{"XOR (HL)", 1, &UNKNOWN},			//AE
	{"XOR A", 1, &UNKNOWN},				//AF
	{"OR B", 1, &UNKNOWN},				//B0
	{"OR C", 1, &UNKNOWN},				//B1
	{"OR D", 1, &UNKNOWN},				//B2
	{"OR E", 1, &UNKNOWN},				//B3
	{"OR H", 1, &UNKNOWN},				//B4
	{"OR L", 1, &UNKNOWN},				//B5
	{"OR (HL)", 1, &UNKNOWN},			//B6
	{"OR A", 1, &UNKNOWN},				//B7
	{"CP B", 1, &UNKNOWN},				//B8
	{"CP C", 1, &UNKNOWN},				//B9
	{"CP D", 1, &UNKNOWN},				//BA
	{"CP E", 1, &UNKNOWN},				//BB
	{"CP H", 1, &UNKNOWN},				//BC
	{"CP L", 1, &UNKNOWN},				//BD
	{"CP (HL)", 1, &UNKNOWN},			//BE
	{"CP A", 1, &UNKNOWN},				//BF
	{"RET NZ", 1, &UNKNOWN},			//C0
	{"POP BC", 1, &UNKNOWN},			//C1
	{"JP NZ, %04X", 3, &UNKNOWN},		//C2
	{"JP %04X", 3, &UNKNOWN},			//C3
	{"CALL NZ, %04X", 3, &UNKNOWN},		//C4
	{"PUSH BC", 1, &UNKNOWN},			//C5
	{"ADD %02X", 2, &UNKNOWN},			//C6
	{"RST 0x00", 1, &UNKNOWN},			//C7
	{"RET Z", 1, &UNKNOWN},				//C8
	{"RET", 1, &UNKNOWN},				//C9
	{"JP Z, %04X", 3, &UNKNOWN},		//CA
	{"CB %02X - ", 2, &UNKNOWN},		//CB
	{"CALL Z, %04X", 3, &UNKNOWN},		//CC
	{"CALL %04X", 3, &UNKNOWN},			//CD
	{"ADC %02X", 2, &UNKNOWN},			//CE
	{"RST 0x08", 1, &UNKNOWN},			//CF
	{"RET NC", 1, &UNKNOWN},			//D0
	{"POP DE", 1, &UNKNOWN},			//D1
	{"JP NC, %04X", 3, &UNKNOWN},		//D2
	{"Undefined OP", 1, &UNDEFINED},	//D3
	{"CALL NC, %04X", 3, &UNKNOWN},		//D4
	{"PUSH DE", 1, &UNKNOWN},			//D5
	{"SUB %02X", 2, &UNKNOWN},			//D6
	{"RST 0x10", 1, &UNKNOWN},			//D7
	{"RET C", 1, &UNKNOWN},				//D8
	{"RETI", 1, &UNKNOWN},				//D9
	{"JP C, %04X", 3, &UNKNOWN},		//DA
	{"Undefined OP", 1, &UNDEFINED},	//DB
	{"CALL C, %04X", 3, &UNKNOWN},		//DC
	{"Undefined OP", 1, &UNDEFINED},	//DD
	{"SBC %02X", 2, &UNKNOWN},			//DE
	{"RST 0x18", 1, &UNKNOWN},			//DF
	{"LD (FF00+%02X), A", 2, &UNKNOWN},	//E0
	{"POP HL", 1, &UNKNOWN},			//E1
	{"LD (FF00+C), A", 1, &UNKNOWN},	//E2
	{"Undefined OP", 1, &UNDEFINED},	//E3
	{"Undefined OP", 1, &UNDEFINED},	//E4
	{"PUSH HL", 1, &UNKNOWN},			//E5
	{"AND %02X", 2, &UNKNOWN},			//E6
	{"RST 0x20", 1, &UNKNOWN},			//E7
	{"ADD SP, %02X", 2, &UNKNOWN},		//E8
	{"JP HL", 1, &UNKNOWN},				//E9
	{"LD (%04X), A", 3, &UNKNOWN},		//EA
	{"Undefined OP", 1, &UNDEFINED},	//EB
	{"Undefined OP", 1, &UNDEFINED},	//EC
	{"Undefined OP", 1, &UNDEFINED},	//ED
	{"XOR %02X", 2, &UNKNOWN},			//EE
	{"RST 0x28", 1, &UNKNOWN},			//EF
	{"LD A, (FF00+%02X)", 2, &UNKNOWN},	//F0
	{"POP AF", 1, &UNKNOWN},			//F1
	{"LD A, (FF00+C)", 1, &UNKNOWN},	//F2
	{"DI", 1, &UNKNOWN},				//F3
	{"Undefined OP", 1, &UNDEFINED},	//F4
	{"PUSH AF", 1, &UNKNOWN},			//F5
	{"OR %02X", 2, &UNKNOWN},			//F6
	{"RST 0x30", 1, &UNKNOWN},			//F7
	{"LD HL, SP+%02X", 2, &UNKNOWN},	//F8
	{"LD SP, HL", 1, &UNKNOWN},			//F9
	{"LD A, (%04X)", 3, &UNKNOWN},		//FA
	{"EI", 1, &UNKNOWN},				//FB
	{"Undefined OP", 1, &UNDEFINED},	//FC
	{"Undefined OP", 1, &UNDEFINED},	//FD
	{"CP %02X", 2, &UNKNOWN},			//FE
	{"RST 0x38", 1, &UNKNOWN},			//FF
};

void DameEmu::UNDEFINED() {

}

void DameEmu::UNKNOWN() {
	debug_msg("Unknown OP: " << (int)memory[PC] << endl);
	
	status = HALT;
}

void DameEmu::UNKNOWN_CB(uint8_t opcode) {
	debug_msg("Unknown OP: CB " << (int)opcode << endl);
}

void DameEmu::LD_r_n(uint8_t& r) {
	debug_msg("LD " << (int)r << ", " << (int)n << endl);
	
	r = n;

	cycles += 2;
	PC += 2;
}

void DameEmu::LD_HL_n() {
	debug_msg("LD (" << HL << "), " << (int)n << endl);

	memory[HL] = n;

	cycles += 3;
	PC += 2;
}

void DameEmu::LD_r_r(uint8_t& r1, uint8_t& r2) {
	debug_msg("LD " << (int)r1 << ", " << (int)r2 << ";" << endl);

	r1 = r2;

	cycles += 1;
	PC += 1;
}

void DameEmu::LD_r_HL(uint8_t& r) {
	debug_msg("LD " << (int)r << ", (" << HL << ");" << endl);

	r = memory[HL];

	cycles += 2;
	PC += 1;
}

void DameEmu::LD_HL_r(uint8_t& r) {
	debug_msg("LD (" << HL << "), " << (int)r << ";" << endl);

	memory[HL] = r;

	cycles += 2;
	PC += 1;
}

void DameEmu::LD_BC() {
	BC = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD BC, " << nn << endl);
}

void DameEmu::LD_BC_A() {
	memory[BC] = A;

	cycles += 2;
	PC += 1;

	debug_msg("LD (BC), A; (" << BC << ") <- " << (int)A << endl);
}

void DameEmu::LD_DE() {
	DE = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD DE, " << nn << endl);
}

void DameEmu::LD_DE_A() {
	memory[DE] = A;

	cycles += 2;
	PC += 1;

	debug_msg("LD (DE), A; (" << BC << ") <- " << (int)A << endl);
}

void DameEmu::JR_NZ() {
	cycles += 2;
	PC += 2;

	if (FLAG_CHECK(FLAG_ZERO) == 0) {
		cycles += 1;
		PC += e;

		debug_msg("True, PC <- " << PC << endl);
	}
	else {
		debug_msg("False, PC <- " << PC << endl);
	}
}

void DameEmu::LD_HL() {
	HL = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD HL, " << nn << endl);
}

void DameEmu::LD_HLI_A() {
	memory[HL] = A;
	HL++;

	cycles += 2;
	PC += 1;

	debug_msg("LD (HL+), A; (" << HL - 1 << ") <- " << (int)A << endl);
}

void DameEmu::LD_SP() {
	SP = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD SP, " << nn << endl);
}

void DameEmu::LD_HLD_A() {
	memory[HL] = A;
	HL--;

	cycles += 2;
	PC += 1;

	debug_msg("LD (HL-), A; (" << HL+1 << ") <- " << (int)A << endl);
}

void DameEmu::INC(uint8_t& r) {
	isAddHalfCarry(r, 1) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	r++;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 1;
	PC += 1;

	debug_msg("INC " << r - 1 << ";" << endl);
}

void DameEmu::INC_HL() {
	isAddHalfCarry(memory[HL], 1) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	memory[HL]++;
	(memory[HL] == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 3;
	PC += 1;

	debug_msg("INC " << (int)memory[HL] - 1 << ";" << endl);
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
	PC += 1;

	debug_msg("AND B = " << (int)A << endl);
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
	PC += 1;

	debug_msg("AND C = " << (int)A << endl);
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
	PC += 1;

	debug_msg("AND D = " << (int)A << endl);
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
	PC += 1;

	debug_msg("AND E = " << (int)A << endl);
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
	PC += 1;

	debug_msg("AND H = " << (int)A << endl);
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
	PC += 1;

	debug_msg("AND L = " << (int)A << endl);
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
	PC += 1;

	debug_msg("AND (HL) = " << (int)A << endl);
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
	PC += 1;

	debug_msg("AND A = " << (int)A << endl);
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
	PC += 1;

	debug_msg("XOR B = " << (int)A << endl);
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
	PC += 1;

	debug_msg("XOR C = " << (int)A << endl);
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
	PC += 1;

	debug_msg("XOR D = " << (int)A << endl);
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
	PC += 1;

	debug_msg("XOR E = " << (int)A << endl);
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
	PC += 1;

	debug_msg("XOR H = " << (int)A << endl);
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
	PC += 1;

	debug_msg("XOR L = " << (int)A << endl);
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
	PC += 1;

	debug_msg("XOR (HL) = " << (int)A << endl);
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
	PC += 1;

	debug_msg("XOR A = " << (int)A << endl);
}

void DameEmu::LD_C_A() {
	memory[0xFF00 + C] = A;

	cycles += 2;
	PC += 1;

	debug_msg("LD (" << 0xFF00 + (int)C << "), A;" << endl);
}

void DameEmu::LD_A_C() {
	A = memory[0xFF00 + C];

	cycles += 2;
	PC += 1;

	debug_msg("LD A, (" << 0xFF00 + (int)C << ");" << endl);
}

//CB Instructions
void DameEmu::BIT(uint8_t b, uint8_t& r) {
	(r & (1 << b)) ? FLAG_CLEAR(FLAG_ZERO) : FLAG_SET(FLAG_ZERO);

	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 2;
	PC += 2;

	debug_msg("BIT " << (int)b << ", " << (int)r << "; Z <- " << (int)FLAG_CHECK(FLAG_ZERO) << endl);
}

void DameEmu::BIT_HL(uint8_t b) {
	(memory[HL] & (1 << b)) ? FLAG_CLEAR(FLAG_ZERO) : FLAG_SET(FLAG_ZERO);

	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 3;
	PC += 2;

	debug_msg("BIT " << b << ", (HL); Z <- " << (int)FLAG_CHECK(FLAG_ZERO) << endl);
}