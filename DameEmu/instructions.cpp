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
	{"NOP", 1, &UNIMPLEMENTED},					//00
	{"LD BC, %04X", 3, &LD_BC},					//01
	{"LD (BC), A", 1, &LD_BC_A},				//02
	{"INC BC", 1, &UNIMPLEMENTED},				//03
	{"INC B", 1, &UNIMPLEMENTED},				//04
	{"DEC B", 1, &UNIMPLEMENTED},				//05
	{"LD B, %02X", 2, &LD_B_n},					//06
	{"RLCA", 1, &UNIMPLEMENTED},				//07
	{"LD (%04X), SP", 3, &UNIMPLEMENTED},		//08
	{"ADD HL, BC", 1, &UNIMPLEMENTED},			//09
	{"LD A, (BC)", 1, &LD_A_BC},				//0A
	{"DEC BC", 1, &UNIMPLEMENTED},				//0B
	{"INC C", 1, &INC_C},						//0C
	{"DEC C", 1, &UNIMPLEMENTED},				//0D
	{"LD C, %02X", 2, &LD_C_n},					//0E
	{"RRCA", 1, &UNIMPLEMENTED},				//0F
	{"STOP", 1, &UNIMPLEMENTED},				//10
	{"LD DE, %04X", 3, &LD_DE},					//11
	{"LD (DE), A", 1, &LD_DE_A},				//12
	{"INC DE", 1, &UNIMPLEMENTED},				//13
	{"INC D", 1, &UNIMPLEMENTED},				//14
	{"DEC D", 1, &UNIMPLEMENTED},				//15
	{"LD D, %02X", 2, &LD_D_n},					//16
	{"RLA", 1, &UNIMPLEMENTED},					//17
	{"JR %02X", 2, &UNIMPLEMENTED},				//18
	{"ADD HL, DE", 1, &UNIMPLEMENTED},			//19
	{"LD A, (DE)", 1, &LD_A_DE},				//1A
	{"DEC DE", 1, &UNIMPLEMENTED},				//1B
	{"INC E", 1, &UNIMPLEMENTED},				//1C
	{"DEC E", 1, &UNIMPLEMENTED},				//1D
	{"LD E, %02X", 2, &UNIMPLEMENTED},			//1E
	{"RRA", 1, &UNIMPLEMENTED},					//1F
	{"JR NZ, %02X", 2, &JR_NZ},					//20
	{"LD HL, %04X", 3, &LD_HL},					//21
	{"LD (HL+), A", 1, &UNIMPLEMENTED},			//22
	{"INC HL", 1, &UNIMPLEMENTED},				//23
	{"INC H", 1, &UNIMPLEMENTED},				//24
	{"DEC H", 1, &UNIMPLEMENTED},				//25
	{"LD H, %02X", 2, &LD_H_n},					//26
	{"DAA", 1, &UNIMPLEMENTED},					//27
	{"JR Z, %02X", 2, &UNIMPLEMENTED},			//28
	{"ADD HL, HL", 1, &UNIMPLEMENTED},			//29
	{"LD A, (HL+)", 1, &LD_A_HLI},				//2A
	{"DEC HL", 1, &UNIMPLEMENTED},				//2B
	{"INC L", 1, &UNIMPLEMENTED},				//2C
	{"DEC L", 1, &UNIMPLEMENTED},				//2D
	{"LD L, %02X", 2, &UNIMPLEMENTED},			//2E
	{"CPL", 1, &UNIMPLEMENTED},					//2F
	{"JR NC, %02X", 2, &UNIMPLEMENTED},			//30
	{"LD SP, %04X", 3, &LD_SP},					//31
	{"LD (HL-), A", 1, &LD_HLD_A},				//32
	{"INC SP", 1, &UNIMPLEMENTED},				//33
	{"INC (HL)", 1, &UNIMPLEMENTED},			//34
	{"DEC (HL)", 1, &UNIMPLEMENTED},			//35
	{"LD (HL), %02X", 2, &LD_HL_n},				//36
	{"SCF", 1, &UNIMPLEMENTED},					//37
	{"JR C, %02X", 2, &UNIMPLEMENTED},			//38
	{"ADD HL, SP", 1, &UNIMPLEMENTED},			//39
	{"LD A, (HL-)", 1, &LD_A_HLD},				//3A
	{"DEC SP", 1, &UNIMPLEMENTED},				//3B
	{"INC A", 1, &UNIMPLEMENTED},				//3C
	{"DEC A", 1, &UNIMPLEMENTED},				//3D
	{"LD A, %02X", 2, &LD_A_n},					//3E
	{"CCF", 1, &UNIMPLEMENTED},					//3F
	{"LD B, B", 1, &LD_B_B},					//40
	{"LD B, C", 1, &LD_B_C},					//41
	{"LD B, D", 1, &LD_B_D},					//42
	{"LD B, E", 1, &LD_B_E},					//43
	{"LD B, H", 1, &LD_B_H},					//44
	{"LD B, L", 1, &LD_B_L},					//45
	{"LD B, (HL)", 1, &LD_B_HL},				//46
	{"LD B, A", 1, &LD_B_A},					//47
	{"LD C, B", 1, &LD_C_B},					//48
	{"LD C, C", 1, &LD_C_C},					//49
	{"LD C, D", 1, &LD_C_D},					//4A
	{"LD C, E", 1, &LD_C_E},					//4B
	{"LD C, H", 1, &LD_C_H},					//4C
	{"LD C, L", 1, &LD_C_L},					//4D
	{"LD C, (HL)", 1, &LD_C_HL},				//4E
	{"LD C, A", 1, &LD_C_A},					//4F
	{"LD D, B", 1, &LD_D_B},					//50
	{"LD D, C", 1, &LD_D_C},					//51
	{"LD D, D", 1, &LD_D_D},					//52
	{"LD D, E", 1, &LD_D_E},					//53
	{"LD D, H", 1, &LD_D_H},					//54
	{"LD D, L", 1, &LD_D_L},					//55
	{"LD D, (HL)", 1, &LD_D_HL},				//56
	{"LD D, A", 1, &LD_D_A},					//57
	{"LD E, B", 1, &LD_E_B},					//58
	{"LD E, C", 1, &LD_E_C},					//59
	{"LD E, D", 1, &LD_E_D},					//5A
	{"LD E, E", 1, &LD_E_E},					//5B
	{"LD E, H", 1, &LD_E_H},					//5C
	{"LD E, L", 1, &LD_E_L},					//5D
	{"LD E, (HL)", 1, &LD_E_HL},				//5E
	{"LD E, A", 1, &LD_E_A},					//5F
	{"LD H, B", 1, &LD_H_B},					//60
	{"LD H, C", 1, &LD_H_C},					//61
	{"LD H, D", 1, &LD_H_D},					//62
	{"LD H, E", 1, &LD_H_E},					//63
	{"LD H, H", 1, &LD_H_H},					//64
	{"LD H, L", 1, &LD_H_L},					//65
	{"LD H, (HL)", 1, &LD_H_HL},				//66
	{"LD H, A", 1, &LD_H_A},					//67
	{"LD L, B", 1, &LD_L_B},					//68
	{"LD L, C", 1, &LD_L_C},					//69
	{"LD L, D", 1, &LD_L_D},					//6A
	{"LD L, E", 1, &LD_L_E},					//6B
	{"LD L, H", 1, &LD_L_H},					//6C
	{"LD L, L", 1, &LD_L_L},					//6D
	{"LD L, (HL)", 1, &LD_L_HL},				//6E
	{"LD L, A", 1, &LD_L_A},					//6F
	{"LD (HL), B", 1, &LD_HL_B},				//70
	{"LD (HL), C", 1, &LD_HL_C},				//71
	{"LD (HL), D", 1, &LD_HL_D},				//72
	{"LD (HL), E", 1, &LD_HL_E},				//73
	{"LD (HL), H", 1, &LD_HL_H},				//74
	{"LD (HL), L", 1, &LD_HL_L},				//75
	{"HALT", 1, &UNIMPLEMENTED},				//76
	{"LD (HL), A", 1, &LD_HL_A},				//77
	{"LD A, B", 1, &UNIMPLEMENTED},				//78
	{"LD A, C", 1, &UNIMPLEMENTED},				//79
	{"LD A, D", 1, &UNIMPLEMENTED},				//7A
	{"LD A, E", 1, &UNIMPLEMENTED},				//7B
	{"LD A, H", 1, &UNIMPLEMENTED},				//7C
	{"LD A, L", 1, &UNIMPLEMENTED},				//7D
	{"LD A, (HL)", 1, &UNIMPLEMENTED},			//7E
	{"LD A, A", 1, &UNIMPLEMENTED},				//7F
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
	{"AND B", 1, &AND_B},						//A0
	{"AND C", 1, &AND_C},						//A1
	{"AND D", 1, &AND_D},						//A2
	{"AND E", 1, &AND_E},						//A3
	{"AND H", 1, &AND_H},						//A4
	{"AND L", 1, &AND_L},						//A5
	{"AND (HL)", 1, &AND_HL},					//A6
	{"AND A", 1, &AND_A},						//A7
	{"XOR B", 1, &XOR_B},						//A8
	{"XOR C", 1, &XOR_C},						//A9
	{"XOR D", 1, &XOR_D},						//AA
	{"XOR E", 1, &XOR_E},						//AB
	{"XOR H", 1, &XOR_H},						//AC
	{"XOR L", 1, &XOR_L},						//AD
	{"XOR (HL)", 1, &XOR_HL},					//AE
	{"XOR A", 1, &XOR_A},						//AF
	{"OR B", 1, &UNIMPLEMENTED},				//B0
	{"OR C", 1, &UNIMPLEMENTED},				//B1
	{"OR D", 1, &UNIMPLEMENTED},				//B2
	{"OR E", 1, &UNIMPLEMENTED},				//B3
	{"OR H", 1, &UNIMPLEMENTED},				//B4
	{"OR L", 1, &UNIMPLEMENTED},				//B5
	{"OR (HL)", 1, &UNIMPLEMENTED},				//B6
	{"OR A", 1, &UNIMPLEMENTED},				//B7
	{"CP B", 1, &UNIMPLEMENTED},				//B8
	{"CP C", 1, &UNIMPLEMENTED},				//B9
	{"CP D", 1, &UNIMPLEMENTED},				//BA
	{"CP E", 1, &UNIMPLEMENTED},				//BB
	{"CP H", 1, &UNIMPLEMENTED},				//BC
	{"CP L", 1, &UNIMPLEMENTED},				//BD
	{"CP (HL)", 1, &UNIMPLEMENTED},				//BE
	{"CP A", 1, &UNIMPLEMENTED},				//BF
	{"RET NZ", 1, &UNIMPLEMENTED},				//C0
	{"POP BC", 1, &UNIMPLEMENTED},				//C1
	{"JP NZ, %04X", 3, &UNIMPLEMENTED},			//C2
	{"JP %04X", 3, &UNIMPLEMENTED},				//C3
	{"CALL NZ, %04X", 3, &UNIMPLEMENTED},		//C4
	{"PUSH BC", 1, &UNIMPLEMENTED},				//C5
	{"ADD %02X", 2, &UNIMPLEMENTED},			//C6
	{"RST 0x00", 1, &UNIMPLEMENTED},			//C7
	{"RET Z", 1, &UNIMPLEMENTED},				//C8
	{"RET", 1, &UNIMPLEMENTED},					//C9
	{"JP Z, %04X", 3, &UNIMPLEMENTED},			//CA
	{"CB %02X - ", 2, &CB},						//CB
	{"CALL Z, %04X", 3, &UNIMPLEMENTED},		//CC
	{"CALL %04X", 3, &CALL},					//CD
	{"ADC %02X", 2, &UNIMPLEMENTED},			//CE
	{"RST 0x08", 1, &UNIMPLEMENTED},			//CF
	{"RET NC", 1, &UNIMPLEMENTED},				//D0
	{"POP DE", 1, &UNIMPLEMENTED},				//D1
	{"JP NC, %04X", 3, &UNIMPLEMENTED},			//D2
	{"Undefined OP", 1, &UNDEFINED},			//D3
	{"CALL NC, %04X", 3, &UNIMPLEMENTED},		//D4
	{"PUSH DE", 1, &UNIMPLEMENTED},				//D5
	{"SUB %02X", 2, &UNIMPLEMENTED},			//D6
	{"RST 0x10", 1, &UNIMPLEMENTED},			//D7
	{"RET C", 1, &UNIMPLEMENTED},				//D8
	{"RETI", 1, &UNIMPLEMENTED},				//D9
	{"JP C, %04X", 3, &UNIMPLEMENTED},			//DA
	{"Undefined OP", 1, &UNDEFINED},			//DB
	{"CALL C, %04X", 3, &UNIMPLEMENTED},		//DC
	{"Undefined OP", 1, &UNDEFINED},			//DD
	{"SBC %02X", 2, &UNIMPLEMENTED},			//DE
	{"RST 0x18", 1, &UNIMPLEMENTED},			//DF
	{"LD (FF00+%02X), A", 2, &LDH_n_A},			//E0
	{"POP HL", 1, &UNIMPLEMENTED},				//E1
	{"LD (FF00+C), A", 1, &LD_Ca_A},			//E2
	{"Undefined OP", 1, &UNDEFINED},			//E3
	{"Undefined OP", 1, &UNDEFINED},			//E4
	{"PUSH HL", 1, &UNIMPLEMENTED},				//E5
	{"AND %02X", 2, &UNIMPLEMENTED},			//E6
	{"RST 0x20", 1, &UNIMPLEMENTED},			//E7
	{"ADD SP, %02X", 2, &UNIMPLEMENTED},		//E8
	{"JP HL", 1, &UNIMPLEMENTED},				//E9
	{"LD (%04X), A", 3, &LD_nn_A},				//EA
	{"Undefined OP", 1, &UNDEFINED},			//EB
	{"Undefined OP", 1, &UNDEFINED},			//EC
	{"Undefined OP", 1, &UNDEFINED},			//ED
	{"XOR %02X", 2, &UNIMPLEMENTED},			//EE
	{"RST 0x28", 1, &UNIMPLEMENTED},			//EF
	{"LD A, (FF00+%02X)", 2, &LDH_A_n},			//F0
	{"POP AF", 1, &UNIMPLEMENTED},				//F1
	{"LD A, (FF00+C)", 1, &LD_A_Ca},			//F2
	{"DI", 1, &UNIMPLEMENTED},					//F3
	{"Undefined OP", 1, &UNDEFINED},			//F4
	{"PUSH AF", 1, &UNIMPLEMENTED},				//F5
	{"OR %02X", 2, &UNIMPLEMENTED},				//F6
	{"RST 0x30", 1, &UNIMPLEMENTED},			//F7
	{"LD HL, SP+%02X", 2, &UNIMPLEMENTED},		//F8
	{"LD SP, HL", 1, &UNIMPLEMENTED},			//F9
	{"LD A, (%04X)", 3, &LD_A_nn},				//FA
	{"EI", 1, &UNIMPLEMENTED},					//FB
	{"Undefined OP", 1, &UNDEFINED},			//FC
	{"Undefined OP", 1, &UNDEFINED},			//FD
	{"CP %02X", 2, &UNIMPLEMENTED},				//FE
	{"RST 0x38", 1, &UNIMPLEMENTED},			//FF
};

void DameEmu::UNDEFINED() {
	debug_msg("\nUndefined instruction! OP: %X", IR);

	status = HALT;
}

void DameEmu::UNIMPLEMENTED() {
	debug_msg("\nUnimplemented instruction! OP: %X", IR);
	
	status = HALT;
}

void DameEmu::LD_r_n(uint8_t& r) {
	r = n;

	cycles += 2;
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

void DameEmu::LD_BC() {
	BC = nn;

	cycles += 3;
}

void DameEmu::LD_BC_A() {
	memory[BC] = A;

	cycles += 2;
}

void DameEmu::LD_B_n() { LD_r_n(B); }

void DameEmu::LD_A_BC() {
	A = memory[BC];

	cycles += 2;
}

void DameEmu::INC_C() { INC(C); }

void DameEmu::LD_C_n() { LD_r_n(C); }

void DameEmu::LD_DE() {
	DE = nn;

	cycles += 3;
}

void DameEmu::LD_DE_A() {
	memory[DE] = A;

	cycles += 2;
}

void DameEmu::LD_D_n() { LD_r_n(D); }

void DameEmu::LD_A_DE() {
	A = memory[DE];

	cycles += 2;
}

void DameEmu::JR_NZ() {
	cycles += 2;

	if (FLAG_CHECK(FLAG_ZERO) == 0) {
		cycles += 1;
		PC += e;

		debug_msg(";\tTrue, PC <- %02X", PC);
	}
	else {
		debug_msg(";\tFalse, PC <- %02X", PC);
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

void DameEmu::LD_H_n() { LD_r_n(H); }

void DameEmu::LD_A_HLI() {
	A = memory[HL];
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

void DameEmu::LD_HL_n() {
	memory[HL] = n;

	cycles += 3;
}

void DameEmu::LD_A_HLD() {
	A = memory[HL];
	HL--;

	cycles += 2;
}

void DameEmu::LD_A_n() { LD_r_n(A); }

void DameEmu::LD_B_B() { LD_r_r(B, B); }
void DameEmu::LD_B_C() { LD_r_r(B, C); }
void DameEmu::LD_B_D() { LD_r_r(B, D); }
void DameEmu::LD_B_E() { LD_r_r(B, E); }
void DameEmu::LD_B_H() { LD_r_r(B, H); }
void DameEmu::LD_B_L() { LD_r_r(B, L); }
void DameEmu::LD_B_HL() { LD_r_HL(B); }
void DameEmu::LD_B_A() { LD_r_r(B, A); }
void DameEmu::LD_C_B() { LD_r_r(C, B); }
void DameEmu::LD_C_C() { LD_r_r(C, C); }
void DameEmu::LD_C_D() { LD_r_r(C, D); }
void DameEmu::LD_C_E() { LD_r_r(C, E); }
void DameEmu::LD_C_H() { LD_r_r(C, H); }
void DameEmu::LD_C_L() { LD_r_r(C, L); }
void DameEmu::LD_C_HL() { LD_r_HL(C); }
void DameEmu::LD_C_A() { LD_r_r(C, A); }
void DameEmu::LD_D_B() { LD_r_r(D, B); }
void DameEmu::LD_D_C() { LD_r_r(D, C); }
void DameEmu::LD_D_D() { LD_r_r(D, D); }
void DameEmu::LD_D_E() { LD_r_r(D, E); }
void DameEmu::LD_D_H() { LD_r_r(D, H); }
void DameEmu::LD_D_L() { LD_r_r(D, L); }
void DameEmu::LD_D_HL() { LD_r_HL(D); }
void DameEmu::LD_D_A() { LD_r_r(D, A); }
void DameEmu::LD_E_B() { LD_r_r(E, B); }
void DameEmu::LD_E_C() { LD_r_r(E, C); }
void DameEmu::LD_E_D() { LD_r_r(E, D); }
void DameEmu::LD_E_E() { LD_r_r(E, E); }
void DameEmu::LD_E_H() { LD_r_r(E, H); }
void DameEmu::LD_E_L() { LD_r_r(E, L); }
void DameEmu::LD_E_HL() { LD_r_HL(E); }
void DameEmu::LD_E_A() { LD_r_r(E, A); }
void DameEmu::LD_H_B() { LD_r_r(H, B); }
void DameEmu::LD_H_C() { LD_r_r(H, C); }
void DameEmu::LD_H_D() { LD_r_r(H, D); }
void DameEmu::LD_H_E() { LD_r_r(H, E); }
void DameEmu::LD_H_H() { LD_r_r(H, H); }
void DameEmu::LD_H_L() { LD_r_r(H, L); }
void DameEmu::LD_H_HL() { LD_r_HL(H); }
void DameEmu::LD_H_A() { LD_r_r(H, A); }
void DameEmu::LD_L_B() { LD_r_r(L, B); }
void DameEmu::LD_L_C() { LD_r_r(L, C); }
void DameEmu::LD_L_D() { LD_r_r(L, D); }
void DameEmu::LD_L_E() { LD_r_r(L, E); }
void DameEmu::LD_L_H() { LD_r_r(L, H); }
void DameEmu::LD_L_L() { LD_r_r(L, L); }
void DameEmu::LD_L_HL() { LD_r_HL(L); }
void DameEmu::LD_L_A() { LD_r_r(L, A); }
void DameEmu::LD_HL_B() { LD_HL_r(B); }
void DameEmu::LD_HL_C() { LD_HL_r(C); }
void DameEmu::LD_HL_D() { LD_HL_r(D); }
void DameEmu::LD_HL_E() { LD_HL_r(E); }
void DameEmu::LD_HL_H() { LD_HL_r(H); }
void DameEmu::LD_HL_L() { LD_HL_r(L); }
void DameEmu::LD_HL_A() { LD_HL_r(A); }

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

void DameEmu::CALL() {
	memory[SP - 1] = (PC & 0xFF00) >> 8;
	memory[SP - 2] = (PC & 0x00FF);
	SP = SP - 2;
	PC = nn;

	cycles += 6;
}

void DameEmu::LDH_n_A() {
	memory[0xFF00 + n] = A;

	cycles += 3;
}

void DameEmu::LD_Ca_A() {
	memory[0xFF00 + C] = A;

	cycles += 2;
}

void DameEmu::LD_nn_A() {
	memory[nn] = A;

	cycles += 4;
}

void DameEmu::LDH_A_n() {
	A = memory[0xFF00 + n];

	cycles += 3;
}

void DameEmu::LD_A_Ca() {
	A = memory[0xFF00 + C];

	cycles += 2;
}

void DameEmu::LD_A_nn() {
	A = memory[nn];

	cycles += 4;
}