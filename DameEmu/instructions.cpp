#include "DameEmu.h"

#ifdef _DEBUG
#include <cstdio>
#define debug_msg(...) printf(__VA_ARGS__)
#else
#define debug_msg(x)
#endif

#define isHalfCarry(x, y, result) (((x ^ y ^ result) & 0x10) == 0x10)

#define nn (uint16_t)(IR & 0xFFFF)
#define n  (uint8_t)(IR & 0xFF)
#define e  (int8_t)(IR & 0xFF)

DameEmu::Instruction DameEmu::instructions[256] = {
	{"NOP", 1, &DameEmu::UNIMPLEMENTED},				//00
	{"LD BC, %04X", 3, &DameEmu::LD_BC},				//01
	{"LD (BC), A", 1, &DameEmu::LD_BC_A},				//02
	{"INC BC", 1, &DameEmu::INC_BC},					//03
	{"INC B", 1, &DameEmu::INC_B},						//04
	{"DEC B", 1, &DameEmu::DEC_B},						//05
	{"LD B, %02X", 2, &DameEmu::LD_B_n},				//06
	{"RLCA", 1, &DameEmu::UNIMPLEMENTED},				//07
	{"LD (%04X), SP", 3, &DameEmu::UNIMPLEMENTED},		//08
	{"ADD HL, BC", 1, &DameEmu::UNIMPLEMENTED},			//09
	{"LD A, (BC)", 1, &DameEmu::LD_A_BC},				//0A
	{"DEC BC", 1, &DameEmu::UNIMPLEMENTED},				//0B
	{"INC C", 1, &DameEmu::INC_C},						//0C
	{"DEC C", 1, &DameEmu::DEC_C},						//0D
	{"LD C, %02X", 2, &DameEmu::LD_C_n},				//0E
	{"RRCA", 1, &DameEmu::UNIMPLEMENTED},				//0F
	{"STOP", 1, &DameEmu::UNIMPLEMENTED},				//10
	{"LD DE, %04X", 3, &DameEmu::LD_DE},				//11
	{"LD (DE), A", 1, &DameEmu::LD_DE_A},				//12
	{"INC DE", 1, &DameEmu::INC_DE},					//13
	{"INC D", 1, &DameEmu::INC_D},						//14
	{"DEC D", 1, &DameEmu::DEC_D},						//15
	{"LD D, %02X", 2, &DameEmu::LD_D_n},				//16
	{"RLA", 1, &DameEmu::RLA},							//17
	{"JR %02X", 2, &DameEmu::UNIMPLEMENTED},			//18
	{"ADD HL, DE", 1, &DameEmu::UNIMPLEMENTED},			//19
	{"LD A, (DE)", 1, &DameEmu::LD_A_DE},				//1A
	{"DEC DE", 1, &DameEmu::UNIMPLEMENTED},				//1B
	{"INC E", 1, &DameEmu::INC_E},						//1C
	{"DEC E", 1, &DameEmu::DEC_E},						//1D
	{"LD E, %02X", 2, &DameEmu::UNIMPLEMENTED},			//1E
	{"RRA", 1, &DameEmu::RRA},							//1F
	{"JR NZ, %02X", 2, &DameEmu::JR_NZ},				//20
	{"LD HL, %04X", 3, &DameEmu::LD_HL},				//21
	{"LD (HL+), A", 1, &DameEmu::LD_HLI_A},				//22
	{"INC HL", 1, &DameEmu::INC_HL},					//23
	{"INC H", 1, &DameEmu::INC_H},						//24
	{"DEC H", 1, &DameEmu::DEC_H},						//25
	{"LD H, %02X", 2, &DameEmu::LD_H_n},				//26
	{"DAA", 1, &DameEmu::UNIMPLEMENTED},				//27
	{"JR Z, %02X", 2, &DameEmu::UNIMPLEMENTED},			//28
	{"ADD HL, HL", 1, &DameEmu::UNIMPLEMENTED},			//29
	{"LD A, (HL+)", 1, &DameEmu::LD_A_HLI},				//2A
	{"DEC HL", 1, &DameEmu::UNIMPLEMENTED},				//2B
	{"INC L", 1, &DameEmu::INC_L},						//2C
	{"DEC L", 1, &DameEmu::DEC_L},						//2D
	{"LD L, %02X", 2, &DameEmu::UNIMPLEMENTED},			//2E
	{"CPL", 1, &DameEmu::UNIMPLEMENTED},				//2F
	{"JR NC, %02X", 2, &DameEmu::UNIMPLEMENTED},		//30
	{"LD SP, %04X", 3, &DameEmu::LD_SP},				//31
	{"LD (HL-), A", 1, &DameEmu::LD_HLD_A},				//32
	{"INC SP", 1, &DameEmu::INC_SP},					//33
	{"INC (HL)", 1, &DameEmu::INC_HLP},					//34
	{"DEC (HL)", 1, &DameEmu::DEC_HL},					//35
	{"LD (HL), %02X", 2, &DameEmu::LD_HL_n},			//36
	{"SCF", 1, &DameEmu::UNIMPLEMENTED},				//37
	{"JR C, %02X", 2, &DameEmu::UNIMPLEMENTED},			//38
	{"ADD HL, SP", 1, &DameEmu::UNIMPLEMENTED},			//39
	{"LD A, (HL-)", 1, &DameEmu::LD_A_HLD},				//3A
	{"DEC SP", 1, &DameEmu::UNIMPLEMENTED},				//3B
	{"INC A", 1, &DameEmu::INC_A},						//3C
	{"DEC A", 1, &DameEmu::DEC_A},						//3D
	{"LD A, %02X", 2, &DameEmu::LD_A_n},				//3E
	{"CCF", 1, &DameEmu::UNIMPLEMENTED},				//3F
	{"LD B, B", 1, &DameEmu::LD_B_B},					//40
	{"LD B, C", 1, &DameEmu::LD_B_C},					//41
	{"LD B, D", 1, &DameEmu::LD_B_D},					//42
	{"LD B, E", 1, &DameEmu::LD_B_E},					//43
	{"LD B, H", 1, &DameEmu::LD_B_H},					//44
	{"LD B, L", 1, &DameEmu::LD_B_L},					//45
	{"LD B, (HL)", 1, &DameEmu::LD_B_HL},				//46
	{"LD B, A", 1, &DameEmu::LD_B_A},					//47
	{"LD C, B", 1, &DameEmu::LD_C_B},					//48
	{"LD C, C", 1, &DameEmu::LD_C_C},					//49
	{"LD C, D", 1, &DameEmu::LD_C_D},					//4A
	{"LD C, E", 1, &DameEmu::LD_C_E},					//4B
	{"LD C, H", 1, &DameEmu::LD_C_H},					//4C
	{"LD C, L", 1, &DameEmu::LD_C_L},					//4D
	{"LD C, (HL)", 1, &DameEmu::LD_C_HL},				//4E
	{"LD C, A", 1, &DameEmu::LD_C_A},					//4F
	{"LD D, B", 1, &DameEmu::LD_D_B},					//50
	{"LD D, C", 1, &DameEmu::LD_D_C},					//51
	{"LD D, D", 1, &DameEmu::LD_D_D},					//52
	{"LD D, E", 1, &DameEmu::LD_D_E},					//53
	{"LD D, H", 1, &DameEmu::LD_D_H},					//54
	{"LD D, L", 1, &DameEmu::LD_D_L},					//55
	{"LD D, (HL)", 1, &DameEmu::LD_D_HL},				//56
	{"LD D, A", 1, &DameEmu::LD_D_A},					//57
	{"LD E, B", 1, &DameEmu::LD_E_B},					//58
	{"LD E, C", 1, &DameEmu::LD_E_C},					//59
	{"LD E, D", 1, &DameEmu::LD_E_D},					//5A
	{"LD E, E", 1, &DameEmu::LD_E_E},					//5B
	{"LD E, H", 1, &DameEmu::LD_E_H},					//5C
	{"LD E, L", 1, &DameEmu::LD_E_L},					//5D
	{"LD E, (HL)", 1, &DameEmu::LD_E_HL},				//5E
	{"LD E, A", 1, &DameEmu::LD_E_A},					//5F
	{"LD H, B", 1, &DameEmu::LD_H_B},					//60
	{"LD H, C", 1, &DameEmu::LD_H_C},					//61
	{"LD H, D", 1, &DameEmu::LD_H_D},					//62
	{"LD H, E", 1, &DameEmu::LD_H_E},					//63
	{"LD H, H", 1, &DameEmu::LD_H_H},					//64
	{"LD H, L", 1, &DameEmu::LD_H_L},					//65
	{"LD H, (HL)", 1, &DameEmu::LD_H_HL},				//66
	{"LD H, A", 1, &DameEmu::LD_H_A},					//67
	{"LD L, B", 1, &DameEmu::LD_L_B},					//68
	{"LD L, C", 1, &DameEmu::LD_L_C},					//69
	{"LD L, D", 1, &DameEmu::LD_L_D},					//6A
	{"LD L, E", 1, &DameEmu::LD_L_E},					//6B
	{"LD L, H", 1, &DameEmu::LD_L_H},					//6C
	{"LD L, L", 1, &DameEmu::LD_L_L},					//6D
	{"LD L, (HL)", 1, &DameEmu::LD_L_HL},				//6E
	{"LD L, A", 1, &DameEmu::LD_L_A},					//6F
	{"LD (HL), B", 1, &DameEmu::LD_HL_B},				//70
	{"LD (HL), C", 1, &DameEmu::LD_HL_C},				//71
	{"LD (HL), D", 1, &DameEmu::LD_HL_D},				//72
	{"LD (HL), E", 1, &DameEmu::LD_HL_E},				//73
	{"LD (HL), H", 1, &DameEmu::LD_HL_H},				//74
	{"LD (HL), L", 1, &DameEmu::LD_HL_L},				//75
	{"HALT", 1, &DameEmu::UNIMPLEMENTED},				//76
	{"LD (HL), A", 1, &DameEmu::LD_HL_A},				//77
	{"LD A, B", 1, &DameEmu::UNIMPLEMENTED},				//78
	{"LD A, C", 1, &DameEmu::UNIMPLEMENTED},				//79
	{"LD A, D", 1, &DameEmu::UNIMPLEMENTED},				//7A
	{"LD A, E", 1, &DameEmu::UNIMPLEMENTED},				//7B
	{"LD A, H", 1, &DameEmu::UNIMPLEMENTED},				//7C
	{"LD A, L", 1, &DameEmu::UNIMPLEMENTED},				//7D
	{"LD A, (HL)", 1, &DameEmu::UNIMPLEMENTED},			//7E
	{"LD A, A", 1, &DameEmu::UNIMPLEMENTED},				//7F
	{"ADD B", 1, &DameEmu::UNIMPLEMENTED},				//80
	{"ADD C", 1, &DameEmu::UNIMPLEMENTED},				//81
	{"ADD D", 1, &DameEmu::UNIMPLEMENTED},				//82
	{"ADD E", 1, &DameEmu::UNIMPLEMENTED},				//83
	{"ADD H", 1, &DameEmu::UNIMPLEMENTED},				//84
	{"ADD L", 1, &DameEmu::UNIMPLEMENTED},				//85
	{"ADD (HL)", 1, &DameEmu::UNIMPLEMENTED},			//86
	{"ADD A", 1, &DameEmu::UNIMPLEMENTED},				//87
	{"ADC B", 1, &DameEmu::UNIMPLEMENTED},				//88
	{"ADC C", 1, &DameEmu::UNIMPLEMENTED},				//89
	{"ADC D", 1, &DameEmu::UNIMPLEMENTED},				//8A
	{"ADC E", 1, &DameEmu::UNIMPLEMENTED},				//8B
	{"ADC H", 1, &DameEmu::UNIMPLEMENTED},				//8C
	{"ADC L", 1, &DameEmu::UNIMPLEMENTED},				//8D
	{"ADC (HL)", 1, &DameEmu::UNIMPLEMENTED},			//8E
	{"ADC A", 1, &DameEmu::UNIMPLEMENTED},				//8F
	{"SUB B", 1, &DameEmu::UNIMPLEMENTED},				//90
	{"SUB C", 1, &DameEmu::UNIMPLEMENTED},				//91
	{"SUB D", 1, &DameEmu::UNIMPLEMENTED},				//92
	{"SUB E", 1, &DameEmu::UNIMPLEMENTED},				//93
	{"SUB H", 1, &DameEmu::UNIMPLEMENTED},				//94
	{"SUB L", 1, &DameEmu::UNIMPLEMENTED},				//95
	{"SUB (HL)", 1, &DameEmu::UNIMPLEMENTED},			//96
	{"SUB A", 1, &DameEmu::UNIMPLEMENTED},				//97
	{"SBC B", 1, &DameEmu::UNIMPLEMENTED},				//98
	{"SBC C", 1, &DameEmu::UNIMPLEMENTED},				//99
	{"SBC D", 1, &DameEmu::UNIMPLEMENTED},				//9A
	{"SBC E", 1, &DameEmu::UNIMPLEMENTED},				//9B
	{"SBC H", 1, &DameEmu::UNIMPLEMENTED},				//9C
	{"SBC L", 1, &DameEmu::UNIMPLEMENTED},				//9D
	{"SBC (HL)", 1, &DameEmu::UNIMPLEMENTED},			//9E
	{"SBC A", 1, &DameEmu::UNIMPLEMENTED},				//9F
	{"AND B", 1, &DameEmu::AND_B},						//A0
	{"AND C", 1, &DameEmu::AND_C},						//A1
	{"AND D", 1, &DameEmu::AND_D},						//A2
	{"AND E", 1, &DameEmu::AND_E},						//A3
	{"AND H", 1, &DameEmu::AND_H},						//A4
	{"AND L", 1, &DameEmu::AND_L},						//A5
	{"AND (HL)", 1, &DameEmu::AND_HL},					//A6
	{"AND A", 1, &DameEmu::AND_A},						//A7
	{"XOR B", 1, &DameEmu::XOR_B},						//A8
	{"XOR C", 1, &DameEmu::XOR_C},						//A9
	{"XOR D", 1, &DameEmu::XOR_D},						//AA
	{"XOR E", 1, &DameEmu::XOR_E},						//AB
	{"XOR H", 1, &DameEmu::XOR_H},						//AC
	{"XOR L", 1, &DameEmu::XOR_L},						//AD
	{"XOR (HL)", 1, &DameEmu::XOR_HL},					//AE
	{"XOR A", 1, &DameEmu::XOR_A},						//AF
	{"OR B", 1, &DameEmu::UNIMPLEMENTED},				//B0
	{"OR C", 1, &DameEmu::UNIMPLEMENTED},				//B1
	{"OR D", 1, &DameEmu::UNIMPLEMENTED},				//B2
	{"OR E", 1, &DameEmu::UNIMPLEMENTED},				//B3
	{"OR H", 1, &DameEmu::UNIMPLEMENTED},				//B4
	{"OR L", 1, &DameEmu::UNIMPLEMENTED},				//B5
	{"OR (HL)", 1, &DameEmu::UNIMPLEMENTED},				//B6
	{"OR A", 1, &DameEmu::UNIMPLEMENTED},				//B7
	{"CP B", 1, &DameEmu::UNIMPLEMENTED},				//B8
	{"CP C", 1, &DameEmu::UNIMPLEMENTED},				//B9
	{"CP D", 1, &DameEmu::UNIMPLEMENTED},				//BA
	{"CP E", 1, &DameEmu::UNIMPLEMENTED},				//BB
	{"CP H", 1, &DameEmu::UNIMPLEMENTED},				//BC
	{"CP L", 1, &DameEmu::UNIMPLEMENTED},				//BD
	{"CP (HL)", 1, &DameEmu::UNIMPLEMENTED},				//BE
	{"CP A", 1, &DameEmu::UNIMPLEMENTED},				//BF
	{"RET NZ", 1, &DameEmu::UNIMPLEMENTED},				//C0
	{"POP BC", 1, &DameEmu::POP_BC},						//C1
	{"JP NZ, %04X", 3, &DameEmu::UNIMPLEMENTED},			//C2
	{"JP %04X", 3, &DameEmu::UNIMPLEMENTED},				//C3
	{"CALL NZ, %04X", 3, &DameEmu::UNIMPLEMENTED},		//C4
	{"PUSH BC", 1, &DameEmu::PUSH_BC},					//C5
	{"ADD %02X", 2, &DameEmu::UNIMPLEMENTED},			//C6
	{"RST 0x00", 1, &DameEmu::UNIMPLEMENTED},			//C7
	{"RET Z", 1, &DameEmu::UNIMPLEMENTED},				//C8
	{"RET", 1, &DameEmu::UNIMPLEMENTED},					//C9
	{"JP Z, %04X", 3, &DameEmu::UNIMPLEMENTED},			//CA
	{"CB %02X - ", 2, &DameEmu::CB},						//CB
	{"CALL Z, %04X", 3, &DameEmu::UNIMPLEMENTED},		//CC
	{"CALL %04X", 3, &DameEmu::CALL},					//CD
	{"ADC %02X", 2, &DameEmu::UNIMPLEMENTED},			//CE
	{"RST 0x08", 1, &DameEmu::UNIMPLEMENTED},			//CF
	{"RET NC", 1, &DameEmu::UNIMPLEMENTED},				//D0
	{"POP DE", 1, &DameEmu::POP_DE},						//D1
	{"JP NC, %04X", 3, &DameEmu::UNIMPLEMENTED},			//D2
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//D3
	{"CALL NC, %04X", 3, &DameEmu::UNIMPLEMENTED},		//D4
	{"PUSH DE", 1, &DameEmu::PUSH_DE},					//D5
	{"SUB %02X", 2, &DameEmu::UNIMPLEMENTED},			//D6
	{"RST 0x10", 1, &DameEmu::UNIMPLEMENTED},			//D7
	{"RET C", 1, &DameEmu::UNIMPLEMENTED},				//D8
	{"RETI", 1, &DameEmu::UNIMPLEMENTED},				//D9
	{"JP C, %04X", 3, &DameEmu::UNIMPLEMENTED},			//DA
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//DB
	{"CALL C, %04X", 3, &DameEmu::UNIMPLEMENTED},		//DC
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//DD
	{"SBC %02X", 2, &DameEmu::UNIMPLEMENTED},			//DE
	{"RST 0x18", 1, &DameEmu::UNIMPLEMENTED},			//DF
	{"LD (FF00+%02X), A", 2, &DameEmu::LDH_n_A},			//E0
	{"POP HL", 1, &DameEmu::POP_HL},						//E1
	{"LD (FF00+C), A", 1, &DameEmu::LD_Ca_A},			//E2
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//E3
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//E4
	{"PUSH HL", 1, &DameEmu::PUSH_HL},					//E5
	{"AND %02X", 2, &DameEmu::UNIMPLEMENTED},			//E6
	{"RST 0x20", 1, &DameEmu::UNIMPLEMENTED},			//E7
	{"ADD SP, %02X", 2, &DameEmu::UNIMPLEMENTED},		//E8
	{"JP HL", 1, &DameEmu::UNIMPLEMENTED},				//E9
	{"LD (%04X), A", 3, &DameEmu::LD_nn_A},				//EA
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//EB
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//EC
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//ED
	{"XOR %02X", 2, &DameEmu::UNIMPLEMENTED},			//EE
	{"RST 0x28", 1, &DameEmu::UNIMPLEMENTED},			//EF
	{"LD A, (FF00+%02X)", 2, &DameEmu::LDH_A_n},			//F0
	{"POP AF", 1, &DameEmu::POP_AF},						//F1
	{"LD A, (FF00+C)", 1, &DameEmu::LD_A_Ca},			//F2
	{"DI", 1, &DameEmu::UNIMPLEMENTED},					//F3
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//F4
	{"PUSH AF", 1, &DameEmu::PUSH_AF},					//F5
	{"OR %02X", 2, &DameEmu::UNIMPLEMENTED},				//F6
	{"RST 0x30", 1, &DameEmu::UNIMPLEMENTED},			//F7
	{"LD HL, SP+%02X", 2, &DameEmu::UNIMPLEMENTED},		//F8
	{"LD SP, HL", 1, &DameEmu::UNIMPLEMENTED},			//F9
	{"LD A, (%04X)", 3, &DameEmu::LD_A_nn},				//FA
	{"EI", 1, &DameEmu::UNIMPLEMENTED},					//FB
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//FC
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//FD
	{"CP %02X", 2, &DameEmu::UNIMPLEMENTED},				//FE
	{"RST 0x38", 1, &DameEmu::UNIMPLEMENTED},			//FF
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
	uint8_t result = r + 1;
	isHalfCarry(r, 1, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	r++;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::INC(uint16_t& rr) {
	rr++;

	cycles += 2;
}

void DameEmu::DEC(uint8_t& r) {
	uint8_t result = r - 1;
	isHalfCarry(r, 1, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	r--;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_SET(FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::INC_HLP() { INC(memory[HL]); cycles += 2; }
void DameEmu::DEC_HL() { DEC(memory[HL]); cycles += 2; }

void DameEmu::POP(uint16_t& rr) {
	rr = memory[SP];
	rr |= (uint16_t)(memory[SP + 1] << 8);
	SP += 2;

	cycles += 3;
}

void DameEmu::PUSH(uint16_t& rr) {
	memory[SP - 1] = (rr & 0xFF00) >> 8;
	memory[SP - 2] = (rr & 0x00FF);
	SP -= 2;

	cycles += 4;
}

void DameEmu::LD_BC() {
	BC = nn;

	cycles += 3;
}

void DameEmu::LD_BC_A() {
	memory[BC] = A;

	cycles += 2;
}

void DameEmu::INC_BC() { INC(BC); };
void DameEmu::INC_B() { INC(B); }
void DameEmu::DEC_B() { DEC(B); }
void DameEmu::LD_B_n() { LD_r_n(B); }

void DameEmu::RLCA() {
	(A & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	A = (A << 1) | FLAG_CHECK(FLAG_CARRY);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::LD_A_BC() {
	A = memory[BC];

	cycles += 2;
}

void DameEmu::INC_C() { INC(C); }
void DameEmu::DEC_C() { DEC(C); }

void DameEmu::LD_C_n() { LD_r_n(C); }

void DameEmu::RRCA() {
	(A & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	A = (A >> 1) | (FLAG_CHECK(FLAG_CARRY) << 7);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::LD_DE() {
	DE = nn;

	cycles += 3;
}

void DameEmu::LD_DE_A() {
	memory[DE] = A;

	cycles += 2;
}

void DameEmu::INC_DE() { INC(DE); }
void DameEmu::INC_D() { INC(D); }
void DameEmu::DEC_D() { DEC(D); }
void DameEmu::LD_D_n() { LD_r_n(D); }

void DameEmu::RLA() {
	uint8_t carry = FLAG_CHECK(FLAG_CARRY);
	(A & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	A = (A << 1) & carry;
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::LD_A_DE() {
	A = memory[DE];

	cycles += 2;
}

void DameEmu::INC_E() { INC(E); }
void DameEmu::DEC_E() { DEC(E); }

void DameEmu::RRA() {
	uint8_t carry = FLAG_CHECK(FLAG_CARRY);
	(A & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	A = (A >> 1) | (carry << 7);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
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

void DameEmu::INC_HL() { INC(L); }
void DameEmu::INC_H() { INC(H); }
void DameEmu::DEC_H() { DEC(H); }
void DameEmu::LD_H_n() { LD_r_n(H); }

void DameEmu::LD_A_HLI() {
	A = memory[HL];
	HL++;

	cycles += 2;
}

void DameEmu::INC_L() { INC(L); }
void DameEmu::DEC_L() { DEC(L); }

void DameEmu::LD_SP() {
	SP = nn;

	cycles += 3;
}

void DameEmu::LD_HLD_A() {
	memory[HL] = A;
	HL--;

	cycles += 2;
}

void DameEmu::INC_SP() { INC(SP); }

void DameEmu::LD_HL_n() {
	memory[HL] = n;

	cycles += 3;
}

void DameEmu::DEC_A() { DEC(A); }

void DameEmu::LD_A_HLD() {
	A = memory[HL];
	HL--;

	cycles += 2;
}

void DameEmu::INC_A() { INC(A); }
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

void DameEmu::POP_BC() { POP(BC); }
void DameEmu::PUSH_BC() { PUSH(BC); }

void DameEmu::CALL() {
	memory[SP - 1] = (PC & 0xFF00) >> 8;
	memory[SP - 2] = (PC & 0x00FF);
	SP = SP - 2;
	PC = nn;

	cycles += 6;
}

void DameEmu::POP_DE() { POP(DE); }
void DameEmu::PUSH_DE() { PUSH(DE); }

void DameEmu::LDH_n_A() {
	memory[0xFF00 + n] = A;

	cycles += 3;
}

void DameEmu::POP_HL() { POP(HL); }

void DameEmu::LD_Ca_A() {
	memory[0xFF00 + C] = A;

	cycles += 2;
}

void DameEmu::PUSH_HL() { PUSH(HL); }

void DameEmu::LD_nn_A() {
	memory[nn] = A;

	cycles += 4;
}

void DameEmu::LDH_A_n() {
	A = memory[0xFF00 + n];

	cycles += 3;
}

void DameEmu::POP_AF() { POP(AF);  }

void DameEmu::LD_A_Ca() {
	A = memory[0xFF00 + C];

	cycles += 2;
}

void DameEmu::PUSH_AF() { PUSH(AF); }

void DameEmu::LD_A_nn() {
	A = memory[nn];

	cycles += 4;
}