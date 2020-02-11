#include "DameEmu.h"

#ifdef _DEBUG
#include <cstdio>
#define debug_msg(...) printf(__VA_ARGS__)
#else
#define debug_msg(x)
#endif

#define isHalfCarry(x, y, result) (((x ^ y ^ result) & 0x10) == 0x10)
#define isHalfCarry16(x, y, result) (((x ^ y ^ result) & 0x1000) == 0x1000)

#define nn (uint16_t)(IR & 0xFFFF)
#define n  (uint8_t)(IR & 0xFF)
#define e  (int8_t)(IR & 0xFF)

DameEmu::Instruction DameEmu::instructions[256] = {
	{"NOP", 1, &DameEmu::NOP},							//00
	{"LD BC, %04X", 3, &DameEmu::LD_BC},				//01
	{"LD (BC), A", 1, &DameEmu::LD_BC_A},				//02
	{"INC BC", 1, &DameEmu::INC_BC},					//03
	{"INC B", 1, &DameEmu::INC_B},						//04
	{"DEC B", 1, &DameEmu::DEC_B},						//05
	{"LD B, %02X", 2, &DameEmu::LD_B_n},				//06
	{"RLCA", 1, &DameEmu::RLCA},						//07
	{"LD (%04X), SP", 3, &DameEmu::LD_nn_SP},			//08
	{"ADD HL, BC", 1, &DameEmu::ADD_HL_BC},				//09
	{"LD A, (BC)", 1, &DameEmu::LD_A_BC},				//0A
	{"DEC BC", 1, &DameEmu::DEC_BC},					//0B
	{"INC C", 1, &DameEmu::INC_C},						//0C
	{"DEC C", 1, &DameEmu::DEC_C},						//0D
	{"LD C, %02X", 2, &DameEmu::LD_C_n},				//0E
	{"RRCA", 1, &DameEmu::RRCA},						//0F
	{"STOP", 1, &DameEmu::STOP},						//10
	{"LD DE, %04X", 3, &DameEmu::LD_DE},				//11
	{"LD (DE), A", 1, &DameEmu::LD_DE_A},				//12
	{"INC DE", 1, &DameEmu::INC_DE},					//13
	{"INC D", 1, &DameEmu::INC_D},						//14
	{"DEC D", 1, &DameEmu::DEC_D},						//15
	{"LD D, %02X", 2, &DameEmu::LD_D_n},				//16
	{"RLA", 1, &DameEmu::RLA},							//17
	{"JR %02X", 2, &DameEmu::JR},						//18
	{"ADD HL, DE", 1, &DameEmu::ADD_HL_DE},				//19
	{"LD A, (DE)", 1, &DameEmu::LD_A_DE},				//1A
	{"DEC DE", 1, &DameEmu::DEC_DE},					//1B
	{"INC E", 1, &DameEmu::INC_E},						//1C
	{"DEC E", 1, &DameEmu::DEC_E},						//1D
	{"LD E, %02X", 2, &DameEmu::LD_E_n},				//1E
	{"RRA", 1, &DameEmu::RRA},							//1F
	{"JR NZ, %02X", 2, &DameEmu::JR_NZ},				//20
	{"LD HL, %04X", 3, &DameEmu::LD_HL},				//21
	{"LD (HL+), A", 1, &DameEmu::LD_HLI_A},				//22
	{"INC HL", 1, &DameEmu::INC_HL},					//23
	{"INC H", 1, &DameEmu::INC_H},						//24
	{"DEC H", 1, &DameEmu::DEC_H},						//25
	{"LD H, %02X", 2, &DameEmu::LD_H_n},				//26
	{"DAA", 1, &DameEmu::DAA},							//27
	{"JR Z, %02X", 2, &DameEmu::JR_Z},					//28
	{"ADD HL, HL", 1, &DameEmu::ADD_HL_HL},				//29
	{"LD A, (HL+)", 1, &DameEmu::LD_A_HLI},				//2A
	{"DEC HL", 1, &DameEmu::DEC_HL},					//2B
	{"INC L", 1, &DameEmu::INC_L},						//2C
	{"DEC L", 1, &DameEmu::DEC_L},						//2D
	{"LD L, %02X", 2, &DameEmu::LD_L_n},				//2E
	{"CPL", 1, &DameEmu::CPL},							//2F
	{"JR NC, %02X", 2, &DameEmu::JR_NC},				//30
	{"LD SP, %04X", 3, &DameEmu::LD_SP},				//31
	{"LD (HL-), A", 1, &DameEmu::LD_HLD_A},				//32
	{"INC SP", 1, &DameEmu::INC_SP},					//33
	{"INC (HL)", 1, &DameEmu::INC_HLP},					//34
	{"DEC (HL)", 1, &DameEmu::DEC_HLP},					//35
	{"LD (HL), %02X", 2, &DameEmu::LD_HL_n},			//36
	{"SCF", 1, &DameEmu::SCF},							//37
	{"JR C, %02X", 2, &DameEmu::JR_C},					//38
	{"ADD HL, SP", 1, &DameEmu::ADD_HL_SP},				//39
	{"LD A, (HL-)", 1, &DameEmu::LD_A_HLD},				//3A
	{"DEC SP", 1, &DameEmu::DEC_SP},					//3B
	{"INC A", 1, &DameEmu::INC_A},						//3C
	{"DEC A", 1, &DameEmu::DEC_A},						//3D
	{"LD A, %02X", 2, &DameEmu::LD_A_n},				//3E
	{"CCF", 1, &DameEmu::CCF},							//3F
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
	{"HALT", 1, &DameEmu::HALT},						//76
	{"LD (HL), A", 1, &DameEmu::LD_HL_A},				//77
	{"LD A, B", 1, &DameEmu::LD_A_B},					//78
	{"LD A, C", 1, &DameEmu::LD_A_C},					//79
	{"LD A, D", 1, &DameEmu::LD_A_D},					//7A
	{"LD A, E", 1, &DameEmu::LD_A_E},					//7B
	{"LD A, H", 1, &DameEmu::LD_A_H},					//7C
	{"LD A, L", 1, &DameEmu::LD_A_L},					//7D
	{"LD A, (HL)", 1, &DameEmu::LD_A_HL},				//7E
	{"LD A, A", 1, &DameEmu::LD_A_A},					//7F
	{"ADD B", 1, &DameEmu::ADD_B},						//80
	{"ADD C", 1, &DameEmu::ADD_C},						//81
	{"ADD D", 1, &DameEmu::ADD_D},						//82
	{"ADD E", 1, &DameEmu::ADD_E},						//83
	{"ADD H", 1, &DameEmu::ADD_H},						//84
	{"ADD L", 1, &DameEmu::ADD_L},						//85
	{"ADD (HL)", 1, &DameEmu::ADD_HL},					//86
	{"ADD A", 1, &DameEmu::ADD_A},						//87
	{"ADC B", 1, &DameEmu::ADC_B},						//88
	{"ADC C", 1, &DameEmu::ADC_C},						//89
	{"ADC D", 1, &DameEmu::ADC_D},						//8A
	{"ADC E", 1, &DameEmu::ADC_E},						//8B
	{"ADC H", 1, &DameEmu::ADC_H},						//8C
	{"ADC L", 1, &DameEmu::ADC_L},						//8D
	{"ADC (HL)", 1, &DameEmu::ADC_HL},					//8E
	{"ADC A", 1, &DameEmu::ADC_A},						//8F
	{"SUB B", 1, &DameEmu::SUB_B},						//90
	{"SUB C", 1, &DameEmu::SUB_C},						//91
	{"SUB D", 1, &DameEmu::SUB_D},						//92
	{"SUB E", 1, &DameEmu::SUB_E},						//93
	{"SUB H", 1, &DameEmu::SUB_H},						//94
	{"SUB L", 1, &DameEmu::SUB_L},						//95
	{"SUB (HL)", 1, &DameEmu::SUB_HL},					//96
	{"SUB A", 1, &DameEmu::SUB_A},						//97
	{"SBC B", 1, &DameEmu::SBC_B},						//98
	{"SBC C", 1, &DameEmu::SBC_C},						//99
	{"SBC D", 1, &DameEmu::SBC_D},						//9A
	{"SBC E", 1, &DameEmu::SBC_E},						//9B
	{"SBC H", 1, &DameEmu::SBC_H},						//9C
	{"SBC L", 1, &DameEmu::SBC_L},						//9D
	{"SBC (HL)", 1, &DameEmu::SBC_HL},					//9E
	{"SBC A", 1, &DameEmu::SBC_A},						//9F
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
	{"OR B", 1, &DameEmu::OR_B},						//B0
	{"OR C", 1, &DameEmu::OR_C},						//B1
	{"OR D", 1, &DameEmu::OR_D},						//B2
	{"OR E", 1, &DameEmu::OR_E},						//B3
	{"OR H", 1, &DameEmu::OR_H},						//B4
	{"OR L", 1, &DameEmu::OR_L},						//B5
	{"OR (HL)", 1, &DameEmu::OR_HL},					//B6
	{"OR A", 1, &DameEmu::OR_A},						//B7
	{"CP B", 1, &DameEmu::CP_B},						//B8
	{"CP C", 1, &DameEmu::CP_C},						//B9
	{"CP D", 1, &DameEmu::CP_D},						//BA
	{"CP E", 1, &DameEmu::CP_E},						//BB
	{"CP H", 1, &DameEmu::CP_H},						//BC
	{"CP L", 1, &DameEmu::CP_L},						//BD
	{"CP (HL)", 1, &DameEmu::CP_HL},					//BE
	{"CP A", 1, &DameEmu::CP_A},						//BF
	{"RET NZ", 1, &DameEmu::RET_NZ},					//C0
	{"POP BC", 1, &DameEmu::POP_BC},					//C1
	{"JP NZ, %04X", 3, &DameEmu::JP_NZ},				//C2
	{"JP %04X", 3, &DameEmu::JP},						//C3
	{"CALL NZ, %04X", 3, &DameEmu::CALL_NZ},			//C4
	{"PUSH BC", 1, &DameEmu::PUSH_BC},					//C5
	{"ADD %02X", 2, &DameEmu::ADD_n},					//C6
	{"RST 0x00", 1, &DameEmu::RST_00},					//C7
	{"RET Z", 1, &DameEmu::RET_Z},						//C8
	{"RET", 1, &DameEmu::RET},							//C9
	{"JP Z, %04X", 3, &DameEmu::JP_Z},					//CA
	{"CB %02X - ", 2, &DameEmu::CB},					//CB
	{"CALL Z, %04X", 3, &DameEmu::CALL_Z},				//CC
	{"CALL %04X", 3, &DameEmu::CALL},					//CD
	{"ADC %02X", 2, &DameEmu::ADC_n},					//CE
	{"RST 0x08", 1, &DameEmu::RST_08},					//CF
	{"RET NC", 1, &DameEmu::RET_NC},					//D0
	{"POP DE", 1, &DameEmu::POP_DE},					//D1
	{"JP NC, %04X", 3, &DameEmu::JP_NC},				//D2
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//D3
	{"CALL NC, %04X", 3, &DameEmu::CALL_NC},			//D4
	{"PUSH DE", 1, &DameEmu::PUSH_DE},					//D5
	{"SUB %02X", 2, &DameEmu::SUB_n},					//D6
	{"RST 0x10", 1, &DameEmu::RST_10},					//D7
	{"RET C", 1, &DameEmu::RET_C},						//D8
	{"RETI", 1, &DameEmu::RETI},						//D9
	{"JP C, %04X", 3, &DameEmu::JP_C},					//DA
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//DB
	{"CALL C, %04X", 3, &DameEmu::CALL_C},				//DC
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//DD
	{"SBC %02X", 2, &DameEmu::SBC_n},					//DE
	{"RST 0x18", 1, &DameEmu::RST_18},					//DF
	{"LD (FF00+%02X), A", 2, &DameEmu::LDH_n_A},		//E0
	{"POP HL", 1, &DameEmu::POP_HL},					//E1
	{"LD (FF00+C), A", 1, &DameEmu::LD_Ca_A},			//E2
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//E3
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//E4
	{"PUSH HL", 1, &DameEmu::PUSH_HL},					//E5
	{"AND %02X", 2, &DameEmu::AND_n},					//E6
	{"RST 0x20", 1, &DameEmu::RST_20},					//E7
	{"ADD SP, %02X", 2, &DameEmu::ADD_SP_e},			//E8
	{"JP HL", 1, &DameEmu::JP_HL},						//E9
	{"LD (%04X), A", 3, &DameEmu::LD_nn_A},				//EA
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//EB
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//EC
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//ED
	{"XOR %02X", 2, &DameEmu::XOR_n},					//EE
	{"RST 0x28", 1, &DameEmu::RST_28},					//EF
	{"LD A, (FF00+%02X)", 2, &DameEmu::LDH_A_n},		//F0
	{"POP AF", 1, &DameEmu::POP_AF},					//F1
	{"LD A, (FF00+C)", 1, &DameEmu::LD_A_Ca},			//F2
	{"DI", 1, &DameEmu::DI},							//F3
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//F4
	{"PUSH AF", 1, &DameEmu::PUSH_AF},					//F5
	{"OR %02X", 2, &DameEmu::OR_n},						//F6
	{"RST 0x30", 1, &DameEmu::RST_30},					//F7
	{"LD HL, SP+%02X", 2, &DameEmu::LD_HL_SP_e},		//F8
	{"LD SP, HL", 1, &DameEmu::LD_SP_HL},				//F9
	{"LD A, (%04X)", 3, &DameEmu::LD_A_nn},				//FA
	{"EI", 1, &DameEmu::EI},							//FB
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//FC
	{"Undefined OP", 1, &DameEmu::UNDEFINED},			//FD
	{"CP %02X", 2, &DameEmu::CP_n},						//FE
	{"RST 0x38", 1, &DameEmu::RST_38},					//FF
};

void DameEmu::UNDEFINED() {
	debug_msg("\nUndefined instruction! OP: %X", IR);

	status = EmuStatus::HALT;
}

void DameEmu::UNIMPLEMENTED() {
	debug_msg("\nUnimplemented instruction! OP: %X", IR);
	
	status = EmuStatus::HALT;
}

void DameEmu::NOP() {
	cycles += 1;
}

void DameEmu::STOP() {
	//TODO: Stop system clock, oscialltor circuit, and LCD controller
	debug_msg("\nSTOP instruction has been called but it isn't properly implemented!\n");
	
	cycles += 1;
}

void DameEmu::HALT() {
	//TODO: Stop system clock. HALT mode is exited when interrupt request/enable flag are set
	debug_msg("\nHALT instruction has been called but it isn't properly implemented!\n");

	cycles += 1;
}

void DameEmu::DI() {
	InterruptEnable = 0;

	cycles += 1;
}

void DameEmu::EI() {
	InterruptEnable = 1;

	cycles += 1;
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

void DameEmu::LD_E_n() { LD_r_n(E); }
void DameEmu::LD_L_n() { LD_r_n(L); }

void DameEmu::ADD_HL(uint16_t rr) {
	uint32_t result = HL + rr;
	(result > UINT16_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry16(HL, rr, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	HL = result;

	cycles += 2;
}

void DameEmu::ADD_HL_BC() { ADD_HL(BC); }
void DameEmu::ADD_HL_DE() { ADD_HL(DE); }
void DameEmu::ADD_HL_HL() { ADD_HL(HL); }
void DameEmu::ADD_HL_SP() { ADD_HL(SP); }

void DameEmu::ADD_SP_e() {
	uint32_t result = SP + e;
	(result > UINT16_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry16(SP, e, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE | FLAG_ZERO);
	SP = result;

	cycles += 4;
}

void DameEmu::ADD(uint8_t r) {
	uint16_t result = A + r;
	A = result;
	(result > UINT8_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);
	
	cycles += 1;
}

void DameEmu::ADD_A() { ADD(A); }
void DameEmu::ADD_B() { ADD(B); }
void DameEmu::ADD_C() { ADD(C); }
void DameEmu::ADD_D() { ADD(D); }
void DameEmu::ADD_E() { ADD(E); }
void DameEmu::ADD_H() { ADD(H); }
void DameEmu::ADD_L() { ADD(L); }
void DameEmu::ADD_HL() { ADD(memory[HL]); cycles += 1; }
void DameEmu::ADD_n() { ADD(n);  cycles += 1; }

void DameEmu::ADC(uint8_t r) {
	uint16_t result = A + r;
	A = result;
	(result > UINT8_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	A += FLAG_CHECK(FLAG_CARRY);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);
	
	cycles += 1;
}

void DameEmu::ADC_A() { ADC(A); }
void DameEmu::ADC_B() { ADC(B); }
void DameEmu::ADC_C() { ADC(C); }
void DameEmu::ADC_D() { ADC(D); }
void DameEmu::ADC_E() { ADC(E); }
void DameEmu::ADC_H() { ADC(H); }
void DameEmu::ADC_L() { ADC(L); }
void DameEmu::ADC_HL() { ADC(memory[HL]); cycles += 1; }
void DameEmu::ADC_n() { ADC(n);  cycles += 1; }

void DameEmu::SUB(uint8_t r) {
	uint8_t result = A - r;
	(A < r) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_SET(FLAG_HALFCARRY);
	A = result;
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_SET(FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::SUB_A() { SUB(A); }
void DameEmu::SUB_B() { SUB(B); }
void DameEmu::SUB_C() { SUB(C); }
void DameEmu::SUB_D() { SUB(D); }
void DameEmu::SUB_E() { SUB(E); }
void DameEmu::SUB_H() { SUB(H); }
void DameEmu::SUB_L() { SUB(L); }
void DameEmu::SUB_HL() { SUB(memory[HL]); cycles += 1; }
void DameEmu::SUB_n() { SUB(n);  cycles += 1; }

void DameEmu::SBC(uint8_t r) {
	uint8_t result = A - r;
	(A < r) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_SET(FLAG_HALFCARRY);
	A = result - FLAG_CHECK(FLAG_CARRY);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_SET(FLAG_NEGATIVE);
	
	cycles += 1;
}

void DameEmu::SBC_A() { SBC(A); }
void DameEmu::SBC_B() { SBC(B); }
void DameEmu::SBC_C() { SBC(C); }
void DameEmu::SBC_D() { SBC(D); }
void DameEmu::SBC_E() { SBC(E); }
void DameEmu::SBC_H() { SBC(H); }
void DameEmu::SBC_L() { SBC(L); }
void DameEmu::SBC_HL() { SBC(memory[HL]); cycles += 1; }
void DameEmu::SBC_n() { SBC(n);  cycles += 1; }

void DameEmu::CP(uint8_t r) {
	uint8_t result = A - r;
	(result == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	(A < r) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_SET(FLAG_HALFCARRY);
	FLAG_SET(FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::CP_B() { CP(B); }
void DameEmu::CP_C() { CP(C); }
void DameEmu::CP_D() { CP(D); }
void DameEmu::CP_E() { CP(E); }
void DameEmu::CP_H() { CP(H); }
void DameEmu::CP_L() { CP(L); }
void DameEmu::CP_A() { CP(A); }
void DameEmu::CP_n() { CP(n);  cycles += 1; }
void DameEmu::CP_HL() { CP(memory[HL]); cycles += 1; }

void DameEmu::DAA() {
	debug_msg("\nDAA instruction not implemnted...\n");
	UNIMPLEMENTED();

	cycles += 1;
}

void DameEmu::CPL() {
	A = ~A;
	FLAG_SET(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::CCF() {
	FLAG_CHECK(FLAG_CARRY) ? FLAG_CLEAR(FLAG_CARRY) : FLAG_SET(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void DameEmu::SCF() {
	FLAG_SET(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
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

void DameEmu::DEC(uint16_t& rr) {
	rr--;

	cycles += 2;
}

void DameEmu::DEC_BC() { DEC(BC); }
void DameEmu::DEC_DE() { DEC(DE); }
void DameEmu::DEC_HL() { DEC(HL); }
void DameEmu::DEC_SP() { DEC(SP); }

void DameEmu::INC_HLP() { INC(memory[HL]); cycles += 2; }
void DameEmu::DEC_HLP() { DEC(memory[HL]); cycles += 2; }

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

void DameEmu::RET() {
	PC = (memory[SP] | (memory[SP + 1] << 8));
	SP += 2;

	cycles += 4;
}

void DameEmu::RETI() { RET(); }

void DameEmu::RET_NZ() {
	if (!FLAG_CHECK(FLAG_ZERO)) {
		RET();
		cycles += 1;
	}
	else {
		cycles += 2;
	}
}

void DameEmu::RET_Z() {
	if (FLAG_CHECK(FLAG_ZERO)) {
		RET();
		cycles += 1;
	}
	else {
		cycles += 2;
	}
}

void DameEmu::RET_NC() {
	if (!FLAG_CHECK(FLAG_CARRY)) {
		RET();
		cycles += 1;
	}
	else {
		cycles += 2;
	}
}

void DameEmu::RET_C() {
	if (FLAG_CHECK(FLAG_CARRY)) {
		RET();
		cycles += 1;
	}
	else {
		cycles += 2;
	}
}

void DameEmu::RST(uint8_t t) {
	memory[SP - 1] = (PC & 0xFF00) >> 8;
	memory[SP - 2] = (PC & 0x00FF);
	SP -= 2;
	PC = t;

	cycles += 4;
}

void DameEmu::RST_00() { RST(0x00); }
void DameEmu::RST_08() { RST(0x08); }
void DameEmu::RST_10() { RST(0x10); }
void DameEmu::RST_18() { RST(0x18); }
void DameEmu::RST_20() { RST(0x20); }
void DameEmu::RST_28() { RST(0x28); }
void DameEmu::RST_30() { RST(0x30); }
void DameEmu::RST_38() { RST(0x38); }

void DameEmu::JP() {
	PC = nn;

	cycles += 4;
}

void DameEmu::JP_HL() {
	PC = memory[HL];

	cycles += 1;
}

void DameEmu::JP_NZ() {
	if (!FLAG_CHECK(FLAG_ZERO)) {
		JP();
	}
	else {
		cycles += 3;
	}
}

void DameEmu::JP_Z() {
	if (FLAG_CHECK(FLAG_ZERO)) {
		JP();
	}
	else {
		cycles += 3;
	}
}

void DameEmu::JP_NC() {
	if (!FLAG_CHECK(FLAG_CARRY)) {
		JP();
	}
	else {
		cycles += 3;
	}
}

void DameEmu::JP_C() {
	if (FLAG_CHECK(FLAG_CARRY)) {
		JP();
	}
	else {
		cycles += 3;
	}
}


void DameEmu::JR() {
	PC += e;

	cycles += 3;
}

void DameEmu::JR_NZ() {
	if (!FLAG_CHECK(FLAG_ZERO)) {
		JR();
	}
	else {
		cycles += 2;
	}
}

void DameEmu::JR_Z() {
	if (FLAG_CHECK(FLAG_ZERO)) {
		JR();
	}
	else {
		cycles += 2;
	}
}

void DameEmu::JR_NC() {
	if (!FLAG_CHECK(FLAG_CARRY)) {
		JR();
	}
	else {
		cycles += 2;
	}
}

void DameEmu::JR_C() {
	if (FLAG_CHECK(FLAG_CARRY)) {
		JR();
	}
	else {
		cycles += 2;
	}
}

void DameEmu::CALL() {
	memory[SP - 1] = (PC & 0xFF00) >> 8;
	memory[SP - 2] = (PC & 0x00FF);
	SP = SP - 2;
	PC = nn;

	cycles += 6;
}

void DameEmu::CALL_NZ() {
	if (!FLAG_CHECK(FLAG_ZERO)) {
		CALL();
	}
	else {
		cycles += 3;
	}
}

void DameEmu::CALL_Z() {
	if (FLAG_CHECK(FLAG_ZERO)) {
		CALL();
	}
	else {
		cycles += 3;
	}
}

void DameEmu::CALL_NC() {
	if (!FLAG_CHECK(FLAG_CARRY)) {
		CALL();
	}
	else {
		cycles += 3;
	}
}

void DameEmu::CALL_C() {
	if (FLAG_CHECK(FLAG_CARRY)) {
		CALL();
	}
	else {
		cycles += 3;
	}
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
void DameEmu::LD_A_B() { LD_r_r(A, B); }
void DameEmu::LD_A_C() { LD_r_r(A, C); }
void DameEmu::LD_A_D() { LD_r_r(A, D); }
void DameEmu::LD_A_E() { LD_r_r(A, E); }
void DameEmu::LD_A_H() { LD_r_r(A, H); }
void DameEmu::LD_A_L() { LD_r_r(A, L); }
void DameEmu::LD_A_HL() { LD_r_HL(A); }
void DameEmu::LD_A_A() { LD_r_r(A, A); }

void DameEmu::AND(uint8_t r) {
	A &= r;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::AND_A() { AND(A); }
void DameEmu::AND_B() { AND(B); }
void DameEmu::AND_C() { AND(C); }
void DameEmu::AND_D() { AND(D); }
void DameEmu::AND_E() { AND(E); }
void DameEmu::AND_H() { AND(H); }
void DameEmu::AND_L() { AND(L); }
void DameEmu::AND_HL() { AND(memory[HL]); cycles += 1; }
void DameEmu::AND_n() { AND(n); cycles += 1; }

void DameEmu::OR(uint8_t r) {
	A |= r;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::OR_A() { OR(A); }
void DameEmu::OR_B() { OR(B); }
void DameEmu::OR_C() { OR(C); }
void DameEmu::OR_D() { OR(D); }
void DameEmu::OR_E() { OR(E); }
void DameEmu::OR_H() { OR(H); }
void DameEmu::OR_L() { OR(L); }
void DameEmu::OR_HL() { OR(memory[HL]); cycles += 1; }
void DameEmu::OR_n() { OR(n); cycles += 1; }

void DameEmu::XOR(uint8_t r) {
	A ^= r;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void DameEmu::XOR_A() { XOR(A); }
void DameEmu::XOR_B() { XOR(B); }
void DameEmu::XOR_C() { XOR(C); }
void DameEmu::XOR_D() { XOR(D); }
void DameEmu::XOR_E() { XOR(E); }
void DameEmu::XOR_H() { XOR(H); }
void DameEmu::XOR_L() { XOR(L); }
void DameEmu::XOR_HL() { XOR(memory[HL]); cycles += 1; }
void DameEmu::XOR_n() { XOR(n); cycles += 1; }

void DameEmu::POP_BC() { POP(BC); }
void DameEmu::PUSH_BC() { PUSH(BC); }

void DameEmu::POP_DE() { POP(DE); }
void DameEmu::PUSH_DE() { PUSH(DE); }

void DameEmu::LDH_n_A() {
	memory[0xFF00 + n] = A;

	cycles += 3;
}

void DameEmu::LD_nn_SP() {
	memory[nn] = memory[SP];
	memory[nn + 1] = (memory[SP - 1] << 8);

	cycles += 5;
}

void DameEmu::LD_HL_SP_e() {
	uint32_t result = SP + e;
	(result > UINT16_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry16(SP, e, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE | FLAG_ZERO);
	HL = result;

	cycles += 3;
}

void DameEmu::LD_SP_HL() {
	SP = HL;

	cycles += 2;
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