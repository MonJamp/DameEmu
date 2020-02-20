#include "CPU.h"

#ifdef _DEBUG
#include <cstdio>
#define debug_msg(...) printf(__VA_ARGS__)
#else
#define debug_msg(x)
#endif

#define isHalfCarry(x, y, result) (((x ^ y ^ result) & 0x10) == 0x10)
#define isHalfCarry16(x, y, result) (((x ^ y ^ result) & 0x1000) == 0x1000)

#define nn (operand)
#define n  static_cast<uint8_t>(operand)
#define e  static_cast<int8_t>(operand)

CPU::Instruction CPU::instructions[256] = {
	{"NOP", 1, &CPU::NOP},							//00
	{"LD BC, %04X", 3, &CPU::LD_BC},				//01
	{"LD (BC), A", 1, &CPU::LD_BC_A},				//02
	{"INC BC", 1, &CPU::INC_BC},					//03
	{"INC B", 1, &CPU::INC_B},						//04
	{"DEC B", 1, &CPU::DEC_B},						//05
	{"LD B, %02X", 2, &CPU::LD_B_n},				//06
	{"RLCA", 1, &CPU::RLCA},						//07
	{"LD (%04X), SP", 3, &CPU::LD_nn_SP},			//08
	{"ADD HL, BC", 1, &CPU::ADD_HL_BC},				//09
	{"LD A, (BC)", 1, &CPU::LD_A_BC},				//0A
	{"DEC BC", 1, &CPU::DEC_BC},					//0B
	{"INC C", 1, &CPU::INC_C},						//0C
	{"DEC C", 1, &CPU::DEC_C},						//0D
	{"LD C, %02X", 2, &CPU::LD_C_n},				//0E
	{"RRCA", 1, &CPU::RRCA},						//0F
	{"STOP", 1, &CPU::STOP},						//10
	{"LD DE, %04X", 3, &CPU::LD_DE},				//11
	{"LD (DE), A", 1, &CPU::LD_DE_A},				//12
	{"INC DE", 1, &CPU::INC_DE},					//13
	{"INC D", 1, &CPU::INC_D},						//14
	{"DEC D", 1, &CPU::DEC_D},						//15
	{"LD D, %02X", 2, &CPU::LD_D_n},				//16
	{"RLA", 1, &CPU::RLA},							//17
	{"JR %02X", 2, &CPU::JR},						//18
	{"ADD HL, DE", 1, &CPU::ADD_HL_DE},				//19
	{"LD A, (DE)", 1, &CPU::LD_A_DE},				//1A
	{"DEC DE", 1, &CPU::DEC_DE},					//1B
	{"INC E", 1, &CPU::INC_E},						//1C
	{"DEC E", 1, &CPU::DEC_E},						//1D
	{"LD E, %02X", 2, &CPU::LD_E_n},				//1E
	{"RRA", 1, &CPU::RRA},							//1F
	{"JR NZ, %02X", 2, &CPU::JR_NZ},				//20
	{"LD HL, %04X", 3, &CPU::LD_HL},				//21
	{"LD (HL+), A", 1, &CPU::LD_HLI_A},				//22
	{"INC HL", 1, &CPU::INC_HL},					//23
	{"INC H", 1, &CPU::INC_H},						//24
	{"DEC H", 1, &CPU::DEC_H},						//25
	{"LD H, %02X", 2, &CPU::LD_H_n},				//26
	{"DAA", 1, &CPU::DAA},							//27
	{"JR Z, %02X", 2, &CPU::JR_Z},					//28
	{"ADD HL, HL", 1, &CPU::ADD_HL_HL},				//29
	{"LD A, (HL+)", 1, &CPU::LD_A_HLI},				//2A
	{"DEC HL", 1, &CPU::DEC_HL},					//2B
	{"INC L", 1, &CPU::INC_L},						//2C
	{"DEC L", 1, &CPU::DEC_L},						//2D
	{"LD L, %02X", 2, &CPU::LD_L_n},				//2E
	{"CPL", 1, &CPU::CPL},							//2F
	{"JR NC, %02X", 2, &CPU::JR_NC},				//30
	{"LD SP, %04X", 3, &CPU::LD_SP},				//31
	{"LD (HL-), A", 1, &CPU::LD_HLD_A},				//32
	{"INC SP", 1, &CPU::INC_SP},					//33
	{"INC (HL)", 1, &CPU::INC_HLP},					//34
	{"DEC (HL)", 1, &CPU::DEC_HLP},					//35
	{"LD (HL), %02X", 2, &CPU::LD_HL_n},			//36
	{"SCF", 1, &CPU::SCF},							//37
	{"JR C, %02X", 2, &CPU::JR_C},					//38
	{"ADD HL, SP", 1, &CPU::ADD_HL_SP},				//39
	{"LD A, (HL-)", 1, &CPU::LD_A_HLD},				//3A
	{"DEC SP", 1, &CPU::DEC_SP},					//3B
	{"INC A", 1, &CPU::INC_A},						//3C
	{"DEC A", 1, &CPU::DEC_A},						//3D
	{"LD A, %02X", 2, &CPU::LD_A_n},				//3E
	{"CCF", 1, &CPU::CCF},							//3F
	{"LD B, B", 1, &CPU::LD_B_B},					//40
	{"LD B, C", 1, &CPU::LD_B_C},					//41
	{"LD B, D", 1, &CPU::LD_B_D},					//42
	{"LD B, E", 1, &CPU::LD_B_E},					//43
	{"LD B, H", 1, &CPU::LD_B_H},					//44
	{"LD B, L", 1, &CPU::LD_B_L},					//45
	{"LD B, (HL)", 1, &CPU::LD_B_HL},				//46
	{"LD B, A", 1, &CPU::LD_B_A},					//47
	{"LD C, B", 1, &CPU::LD_C_B},					//48
	{"LD C, C", 1, &CPU::LD_C_C},					//49
	{"LD C, D", 1, &CPU::LD_C_D},					//4A
	{"LD C, E", 1, &CPU::LD_C_E},					//4B
	{"LD C, H", 1, &CPU::LD_C_H},					//4C
	{"LD C, L", 1, &CPU::LD_C_L},					//4D
	{"LD C, (HL)", 1, &CPU::LD_C_HL},				//4E
	{"LD C, A", 1, &CPU::LD_C_A},					//4F
	{"LD D, B", 1, &CPU::LD_D_B},					//50
	{"LD D, C", 1, &CPU::LD_D_C},					//51
	{"LD D, D", 1, &CPU::LD_D_D},					//52
	{"LD D, E", 1, &CPU::LD_D_E},					//53
	{"LD D, H", 1, &CPU::LD_D_H},					//54
	{"LD D, L", 1, &CPU::LD_D_L},					//55
	{"LD D, (HL)", 1, &CPU::LD_D_HL},				//56
	{"LD D, A", 1, &CPU::LD_D_A},					//57
	{"LD E, B", 1, &CPU::LD_E_B},					//58
	{"LD E, C", 1, &CPU::LD_E_C},					//59
	{"LD E, D", 1, &CPU::LD_E_D},					//5A
	{"LD E, E", 1, &CPU::LD_E_E},					//5B
	{"LD E, H", 1, &CPU::LD_E_H},					//5C
	{"LD E, L", 1, &CPU::LD_E_L},					//5D
	{"LD E, (HL)", 1, &CPU::LD_E_HL},				//5E
	{"LD E, A", 1, &CPU::LD_E_A},					//5F
	{"LD H, B", 1, &CPU::LD_H_B},					//60
	{"LD H, C", 1, &CPU::LD_H_C},					//61
	{"LD H, D", 1, &CPU::LD_H_D},					//62
	{"LD H, E", 1, &CPU::LD_H_E},					//63
	{"LD H, H", 1, &CPU::LD_H_H},					//64
	{"LD H, L", 1, &CPU::LD_H_L},					//65
	{"LD H, (HL)", 1, &CPU::LD_H_HL},				//66
	{"LD H, A", 1, &CPU::LD_H_A},					//67
	{"LD L, B", 1, &CPU::LD_L_B},					//68
	{"LD L, C", 1, &CPU::LD_L_C},					//69
	{"LD L, D", 1, &CPU::LD_L_D},					//6A
	{"LD L, E", 1, &CPU::LD_L_E},					//6B
	{"LD L, H", 1, &CPU::LD_L_H},					//6C
	{"LD L, L", 1, &CPU::LD_L_L},					//6D
	{"LD L, (HL)", 1, &CPU::LD_L_HL},				//6E
	{"LD L, A", 1, &CPU::LD_L_A},					//6F
	{"LD (HL), B", 1, &CPU::LD_HL_B},				//70
	{"LD (HL), C", 1, &CPU::LD_HL_C},				//71
	{"LD (HL), D", 1, &CPU::LD_HL_D},				//72
	{"LD (HL), E", 1, &CPU::LD_HL_E},				//73
	{"LD (HL), H", 1, &CPU::LD_HL_H},				//74
	{"LD (HL), L", 1, &CPU::LD_HL_L},				//75
	{"HALT", 1, &CPU::HALT},						//76
	{"LD (HL), A", 1, &CPU::LD_HL_A},				//77
	{"LD A, B", 1, &CPU::LD_A_B},					//78
	{"LD A, C", 1, &CPU::LD_A_C},					//79
	{"LD A, D", 1, &CPU::LD_A_D},					//7A
	{"LD A, E", 1, &CPU::LD_A_E},					//7B
	{"LD A, H", 1, &CPU::LD_A_H},					//7C
	{"LD A, L", 1, &CPU::LD_A_L},					//7D
	{"LD A, (HL)", 1, &CPU::LD_A_HL},				//7E
	{"LD A, A", 1, &CPU::LD_A_A},					//7F
	{"ADD B", 1, &CPU::ADD_B},						//80
	{"ADD C", 1, &CPU::ADD_C},						//81
	{"ADD D", 1, &CPU::ADD_D},						//82
	{"ADD E", 1, &CPU::ADD_E},						//83
	{"ADD H", 1, &CPU::ADD_H},						//84
	{"ADD L", 1, &CPU::ADD_L},						//85
	{"ADD (HL)", 1, &CPU::ADD_HL},					//86
	{"ADD A", 1, &CPU::ADD_A},						//87
	{"ADC B", 1, &CPU::ADC_B},						//88
	{"ADC C", 1, &CPU::ADC_C},						//89
	{"ADC D", 1, &CPU::ADC_D},						//8A
	{"ADC E", 1, &CPU::ADC_E},						//8B
	{"ADC H", 1, &CPU::ADC_H},						//8C
	{"ADC L", 1, &CPU::ADC_L},						//8D
	{"ADC (HL)", 1, &CPU::ADC_HL},					//8E
	{"ADC A", 1, &CPU::ADC_A},						//8F
	{"SUB B", 1, &CPU::SUB_B},						//90
	{"SUB C", 1, &CPU::SUB_C},						//91
	{"SUB D", 1, &CPU::SUB_D},						//92
	{"SUB E", 1, &CPU::SUB_E},						//93
	{"SUB H", 1, &CPU::SUB_H},						//94
	{"SUB L", 1, &CPU::SUB_L},						//95
	{"SUB (HL)", 1, &CPU::SUB_HL},					//96
	{"SUB A", 1, &CPU::SUB_A},						//97
	{"SBC B", 1, &CPU::SBC_B},						//98
	{"SBC C", 1, &CPU::SBC_C},						//99
	{"SBC D", 1, &CPU::SBC_D},						//9A
	{"SBC E", 1, &CPU::SBC_E},						//9B
	{"SBC H", 1, &CPU::SBC_H},						//9C
	{"SBC L", 1, &CPU::SBC_L},						//9D
	{"SBC (HL)", 1, &CPU::SBC_HL},					//9E
	{"SBC A", 1, &CPU::SBC_A},						//9F
	{"AND B", 1, &CPU::AND_B},						//A0
	{"AND C", 1, &CPU::AND_C},						//A1
	{"AND D", 1, &CPU::AND_D},						//A2
	{"AND E", 1, &CPU::AND_E},						//A3
	{"AND H", 1, &CPU::AND_H},						//A4
	{"AND L", 1, &CPU::AND_L},						//A5
	{"AND (HL)", 1, &CPU::AND_HL},					//A6
	{"AND A", 1, &CPU::AND_A},						//A7
	{"XOR B", 1, &CPU::XOR_B},						//A8
	{"XOR C", 1, &CPU::XOR_C},						//A9
	{"XOR D", 1, &CPU::XOR_D},						//AA
	{"XOR E", 1, &CPU::XOR_E},						//AB
	{"XOR H", 1, &CPU::XOR_H},						//AC
	{"XOR L", 1, &CPU::XOR_L},						//AD
	{"XOR (HL)", 1, &CPU::XOR_HL},					//AE
	{"XOR A", 1, &CPU::XOR_A},						//AF
	{"OR B", 1, &CPU::OR_B},						//B0
	{"OR C", 1, &CPU::OR_C},						//B1
	{"OR D", 1, &CPU::OR_D},						//B2
	{"OR E", 1, &CPU::OR_E},						//B3
	{"OR H", 1, &CPU::OR_H},						//B4
	{"OR L", 1, &CPU::OR_L},						//B5
	{"OR (HL)", 1, &CPU::OR_HL},					//B6
	{"OR A", 1, &CPU::OR_A},						//B7
	{"CP B", 1, &CPU::CP_B},						//B8
	{"CP C", 1, &CPU::CP_C},						//B9
	{"CP D", 1, &CPU::CP_D},						//BA
	{"CP E", 1, &CPU::CP_E},						//BB
	{"CP H", 1, &CPU::CP_H},						//BC
	{"CP L", 1, &CPU::CP_L},						//BD
	{"CP (HL)", 1, &CPU::CP_HL},					//BE
	{"CP A", 1, &CPU::CP_A},						//BF
	{"RET NZ", 1, &CPU::RET_NZ},					//C0
	{"POP BC", 1, &CPU::POP_BC},					//C1
	{"JP NZ, %04X", 3, &CPU::JP_NZ},				//C2
	{"JP %04X", 3, &CPU::JP},						//C3
	{"CALL NZ, %04X", 3, &CPU::CALL_NZ},			//C4
	{"PUSH BC", 1, &CPU::PUSH_BC},					//C5
	{"ADD %02X", 2, &CPU::ADD_n},					//C6
	{"RST 0x00", 1, &CPU::RST_00},					//C7
	{"RET Z", 1, &CPU::RET_Z},						//C8
	{"RET", 1, &CPU::RET},							//C9
	{"JP Z, %04X", 3, &CPU::JP_Z},					//CA
	{"CB %02X - ", 2, &CPU::CB},					//CB
	{"CALL Z, %04X", 3, &CPU::CALL_Z},				//CC
	{"CALL %04X", 3, &CPU::CALL},					//CD
	{"ADC %02X", 2, &CPU::ADC_n},					//CE
	{"RST 0x08", 1, &CPU::RST_08},					//CF
	{"RET NC", 1, &CPU::RET_NC},					//D0
	{"POP DE", 1, &CPU::POP_DE},					//D1
	{"JP NC, %04X", 3, &CPU::JP_NC},				//D2
	{"Undefined OP", 1, &CPU::UNDEFINED},			//D3
	{"CALL NC, %04X", 3, &CPU::CALL_NC},			//D4
	{"PUSH DE", 1, &CPU::PUSH_DE},					//D5
	{"SUB %02X", 2, &CPU::SUB_n},					//D6
	{"RST 0x10", 1, &CPU::RST_10},					//D7
	{"RET C", 1, &CPU::RET_C},						//D8
	{"RETI", 1, &CPU::RETI},						//D9
	{"JP C, %04X", 3, &CPU::JP_C},					//DA
	{"Undefined OP", 1, &CPU::UNDEFINED},			//DB
	{"CALL C, %04X", 3, &CPU::CALL_C},				//DC
	{"Undefined OP", 1, &CPU::UNDEFINED},			//DD
	{"SBC %02X", 2, &CPU::SBC_n},					//DE
	{"RST 0x18", 1, &CPU::RST_18},					//DF
	{"LD (FF00+%02X), A", 2, &CPU::LDH_n_A},		//E0
	{"POP HL", 1, &CPU::POP_HL},					//E1
	{"LD (FF00+C), A", 1, &CPU::LD_Ca_A},			//E2
	{"Undefined OP", 1, &CPU::UNDEFINED},			//E3
	{"Undefined OP", 1, &CPU::UNDEFINED},			//E4
	{"PUSH HL", 1, &CPU::PUSH_HL},					//E5
	{"AND %02X", 2, &CPU::AND_n},					//E6
	{"RST 0x20", 1, &CPU::RST_20},					//E7
	{"ADD SP, %02X", 2, &CPU::ADD_SP_e},			//E8
	{"JP HL", 1, &CPU::JP_HL},						//E9
	{"LD (%04X), A", 3, &CPU::LD_nn_A},				//EA
	{"Undefined OP", 1, &CPU::UNDEFINED},			//EB
	{"Undefined OP", 1, &CPU::UNDEFINED},			//EC
	{"Undefined OP", 1, &CPU::UNDEFINED},			//ED
	{"XOR %02X", 2, &CPU::XOR_n},					//EE
	{"RST 0x28", 1, &CPU::RST_28},					//EF
	{"LD A, (FF00+%02X)", 2, &CPU::LDH_A_n},		//F0
	{"POP AF", 1, &CPU::POP_AF},					//F1
	{"LD A, (FF00+C)", 1, &CPU::LD_A_Ca},			//F2
	{"DI", 1, &CPU::DI},							//F3
	{"Undefined OP", 1, &CPU::UNDEFINED},			//F4
	{"PUSH AF", 1, &CPU::PUSH_AF},					//F5
	{"OR %02X", 2, &CPU::OR_n},						//F6
	{"RST 0x30", 1, &CPU::RST_30},					//F7
	{"LD HL, SP+%02X", 2, &CPU::LD_HL_SP_e},		//F8
	{"LD SP, HL", 1, &CPU::LD_SP_HL},				//F9
	{"LD A, (%04X)", 3, &CPU::LD_A_nn},				//FA
	{"EI", 1, &CPU::EI},							//FB
	{"Undefined OP", 1, &CPU::UNDEFINED},			//FC
	{"Undefined OP", 1, &CPU::UNDEFINED},			//FD
	{"CP %02X", 2, &CPU::CP_n},						//FE
	{"RST 0x38", 1, &CPU::RST_38},					//FF
};

void CPU::UNDEFINED() {
	debug_msg("\nUndefined instruction! OP: %X", operand);
}

void CPU::UNIMPLEMENTED() {
	debug_msg("\nUnimplemented instruction! OP: %X", operand);
}

void CPU::NOP() {
	cycles += 1;
}

void CPU::STOP() {
	//TODO: Stop system clock, oscialltor circuit, and LCD controller
	debug_msg("\nSTOP instruction has been called but it isn't properly implemented!");
	
	cycles += 1;
}

void CPU::HALT() {
	//TODO: Stop system clock. HALT mode is exited when interrupt request/enable flag are set
	debug_msg("\nHALT instruction has been called but it isn't properly implemented!");

	cycles += 1;
}

void CPU::DI() {
	interupt_master_enable = false;

	cycles += 1;
}

void CPU::EI() {
	interupt_master_enable = true;

	cycles += 1;
}

void CPU::LD_r_n(uint8_t& r) {
	r = n;

	cycles += 2;
}

void CPU::LD_r_r(uint8_t& r1, uint8_t r2) {
	r1 = r2;

	cycles += 1;
}

void CPU::LD_r_HL(uint8_t& r) {
	r = mmu.Read(HL);

	cycles += 2;
}

void CPU::LD_HL_r(uint8_t r) {
	mmu.Write(HL, r);

	cycles += 2;
}

void CPU::LD_E_n() { LD_r_n(E); }
void CPU::LD_L_n() { LD_r_n(L); }

void CPU::ADD_HL(uint16_t rr) {
	uint32_t result = HL + rr;
	(result > UINT16_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry16(HL, rr, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	HL = result;

	cycles += 2;
}

void CPU::ADD_HL_BC() { ADD_HL(BC); }
void CPU::ADD_HL_DE() { ADD_HL(DE); }
void CPU::ADD_HL_HL() { ADD_HL(HL); }
void CPU::ADD_HL_SP() { ADD_HL(SP); }

void CPU::ADD_SP_e() {
	uint32_t result = SP + e;
	(result > UINT16_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry16(SP, e, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE | FLAG_ZERO);
	SP = result;

	cycles += 4;
}

void CPU::ADD(uint8_t r) {
	uint16_t result = A + r;
	A = A + r;
	(result > UINT8_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);
	
	cycles += 1;
}

void CPU::ADD_A() { ADD(A); }
void CPU::ADD_B() { ADD(B); }
void CPU::ADD_C() { ADD(C); }
void CPU::ADD_D() { ADD(D); }
void CPU::ADD_E() { ADD(E); }
void CPU::ADD_H() { ADD(H); }
void CPU::ADD_L() { ADD(L); }
void CPU::ADD_HL() { ADD(mmu.Read(HL)); cycles += 1; }
void CPU::ADD_n() { ADD(n);  cycles += 1; }

void CPU::ADC(uint8_t r) {
	uint16_t result = A + r;
	A = A + r;
	(result > UINT8_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	A += FLAG_CHECK(FLAG_CARRY);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);
	
	cycles += 1;
}

void CPU::ADC_A() { ADC(A); }
void CPU::ADC_B() { ADC(B); }
void CPU::ADC_C() { ADC(C); }
void CPU::ADC_D() { ADC(D); }
void CPU::ADC_E() { ADC(E); }
void CPU::ADC_H() { ADC(H); }
void CPU::ADC_L() { ADC(L); }
void CPU::ADC_HL() { ADC(mmu.Read(HL)); cycles += 1; }
void CPU::ADC_n() { ADC(n);  cycles += 1; }

void CPU::SUB(uint8_t r) {
	uint8_t result = A - r;
	(A < r) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_SET(FLAG_HALFCARRY);
	A = result;
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_SET(FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::SUB_A() { SUB(A); }
void CPU::SUB_B() { SUB(B); }
void CPU::SUB_C() { SUB(C); }
void CPU::SUB_D() { SUB(D); }
void CPU::SUB_E() { SUB(E); }
void CPU::SUB_H() { SUB(H); }
void CPU::SUB_L() { SUB(L); }
void CPU::SUB_HL() { SUB(mmu.Read(HL)); cycles += 1; }
void CPU::SUB_n() { SUB(n);  cycles += 1; }

void CPU::SBC(uint8_t r) {
	uint8_t result = A - r;
	(A < r) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_SET(FLAG_HALFCARRY);
	A = result - FLAG_CHECK(FLAG_CARRY);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_SET(FLAG_NEGATIVE);
	
	cycles += 1;
}

void CPU::SBC_A() { SBC(A); }
void CPU::SBC_B() { SBC(B); }
void CPU::SBC_C() { SBC(C); }
void CPU::SBC_D() { SBC(D); }
void CPU::SBC_E() { SBC(E); }
void CPU::SBC_H() { SBC(H); }
void CPU::SBC_L() { SBC(L); }
void CPU::SBC_HL() { SBC(mmu.Read(HL)); cycles += 1; }
void CPU::SBC_n() { SBC(n);  cycles += 1; }

void CPU::CP(uint8_t r) {
	uint8_t result = A - r;
	(result == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	(A < r) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry(A, r, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_SET(FLAG_HALFCARRY);
	FLAG_SET(FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::CP_B() { CP(B); }
void CPU::CP_C() { CP(C); }
void CPU::CP_D() { CP(D); }
void CPU::CP_E() { CP(E); }
void CPU::CP_H() { CP(H); }
void CPU::CP_L() { CP(L); }
void CPU::CP_A() { CP(A); }
void CPU::CP_n() { CP(n);  cycles += 1; }
void CPU::CP_HL() { CP(mmu.Read(HL)); cycles += 1; }

void CPU::DAA() {
	debug_msg("\nDAA instruction not implemnted...\n");
	UNIMPLEMENTED();

	cycles += 1;
}

void CPU::CPL() {
	A = ~A;
	FLAG_SET(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::CCF() {
	FLAG_CHECK(FLAG_CARRY) ? FLAG_CLEAR(FLAG_CARRY) : FLAG_SET(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::SCF() {
	FLAG_SET(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::INC(uint8_t& r) {
	uint8_t result = r + 1;
	isHalfCarry(r, 1, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	r++;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::INC(uint16_t& rr) {
	rr++;

	cycles += 2;
}

void CPU::DEC(uint8_t& r) {
	uint8_t result = r - 1;
	isHalfCarry(r, 1, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	r--;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_SET(FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::DEC(uint16_t& rr) {
	rr--;

	cycles += 2;
}

void CPU::DEC_BC() { DEC(BC); }
void CPU::DEC_DE() { DEC(DE); }
void CPU::DEC_HL() { DEC(HL); }
void CPU::DEC_SP() { DEC(SP); }

void CPU::INC_HLP() { 
	uint8_t value = mmu.Read(HL);
	value++;
	mmu.Write(HL, value);

	cycles += 4; 
}
void CPU::DEC_HLP() { 
	uint8_t value = mmu.Read(HL);
	value--;
	mmu.Write(HL, value);

	cycles += 4; 
}

void CPU::POP(uint16_t& rr) {
	rr = static_cast<uint16_t>(mmu.Read(SP));
	rr |= static_cast<uint16_t>(mmu.Read(SP + 1) << 8);
	SP += 2;

	cycles += 3;
}

void CPU::PUSH(uint16_t rr) {
	uint8_t hi = static_cast<uint8_t>((rr & 0xFF00) >> 8);
	uint8_t lo = static_cast<uint8_t>(rr & 0x00FF);
	mmu.Write(SP - 1, hi);
	mmu.Write(SP - 2, lo);
	SP -= 2;

	cycles += 4;
}

void CPU::RET() {
	uint16_t hi = static_cast<uint16_t>(mmu.Read(SP + 1) << 8);
	uint16_t lo = static_cast<uint16_t>(mmu.Read(SP));
	SP += 2;
	PC = (hi | lo);
	
	cycles += 4;
}

void CPU::RETI() { RET(); interupt_master_enable = true; }

void CPU::RET_NZ() {
	if (!FLAG_CHECK(FLAG_ZERO)) {
		RET();
		cycles += 1;
	}
	else {
		cycles += 2;
	}
}

void CPU::RET_Z() {
	if (FLAG_CHECK(FLAG_ZERO)) {
		RET();
		cycles += 1;
	}
	else {
		cycles += 2;
	}
}

void CPU::RET_NC() {
	if (!FLAG_CHECK(FLAG_CARRY)) {
		RET();
		cycles += 1;
	}
	else {
		cycles += 2;
	}
}

void CPU::RET_C() {
	if (FLAG_CHECK(FLAG_CARRY)) {
		RET();
		cycles += 1;
	}
	else {
		cycles += 2;
	}
}

void CPU::RST(uint8_t t) {
	uint8_t hi = static_cast<uint8_t>((PC & 0xFF00) >> 8);
	uint8_t lo = static_cast<uint8_t>(PC & 0x00FF);
	mmu.Write(SP - 1, hi);
	mmu.Write(SP - 2, lo);
	SP -= 2;
	PC = t;

	cycles += 4;
}

void CPU::RST_00() { RST(0x00); }
void CPU::RST_08() { RST(0x08); }
void CPU::RST_10() { RST(0x10); }
void CPU::RST_18() { RST(0x18); }
void CPU::RST_20() { RST(0x20); }
void CPU::RST_28() { RST(0x28); }
void CPU::RST_30() { RST(0x30); }
void CPU::RST_38() { RST(0x38); }

void CPU::JP() {
	PC = nn;

	cycles += 4;
}

void CPU::JP_HL() {
	PC = HL;

	cycles += 1;
}

void CPU::JP_NZ() {
	if (!FLAG_CHECK(FLAG_ZERO)) {
		JP();
	}
	else {
		cycles += 3;
	}
}

void CPU::JP_Z() {
	if (FLAG_CHECK(FLAG_ZERO)) {
		JP();
	}
	else {
		cycles += 3;
	}
}

void CPU::JP_NC() {
	if (!FLAG_CHECK(FLAG_CARRY)) {
		JP();
	}
	else {
		cycles += 3;
	}
}

void CPU::JP_C() {
	if (FLAG_CHECK(FLAG_CARRY)) {
		JP();
	}
	else {
		cycles += 3;
	}
}


void CPU::JR() {
	PC += e;

	cycles += 3;
}

void CPU::JR_NZ() {
	if (!FLAG_CHECK(FLAG_ZERO)) {
		JR();
	}
	else {
		cycles += 2;
	}
}

void CPU::JR_Z() {
	if (FLAG_CHECK(FLAG_ZERO)) {
		JR();
	}
	else {
		cycles += 2;
	}
}

void CPU::JR_NC() {
	if (!FLAG_CHECK(FLAG_CARRY)) {
		JR();
	}
	else {
		cycles += 2;
	}
}

void CPU::JR_C() {
	if (FLAG_CHECK(FLAG_CARRY)) {
		JR();
	}
	else {
		cycles += 2;
	}
}

void CPU::CALL() {
	uint8_t hi = static_cast<uint8_t>((PC & 0xFF00) >> 8);
	uint8_t lo = static_cast<uint8_t>(PC & 0x00FF);
	mmu.Write(SP - 1, hi);
	mmu.Write(SP - 2, lo);
	SP = SP - 2;
	PC = nn;

	cycles += 6;
}

void CPU::CALL_NZ() {
	if (!FLAG_CHECK(FLAG_ZERO)) {
		CALL();
	}
	else {
		cycles += 3;
	}
}

void CPU::CALL_Z() {
	if (FLAG_CHECK(FLAG_ZERO)) {
		CALL();
	}
	else {
		cycles += 3;
	}
}

void CPU::CALL_NC() {
	if (!FLAG_CHECK(FLAG_CARRY)) {
		CALL();
	}
	else {
		cycles += 3;
	}
}

void CPU::CALL_C() {
	if (FLAG_CHECK(FLAG_CARRY)) {
		CALL();
	}
	else {
		cycles += 3;
	}
}

void CPU::LD_BC() {
	BC = nn;

	cycles += 3;
}

void CPU::LD_BC_A() {
	mmu.Write(BC, A);

	cycles += 2;
}

void CPU::INC_BC() { INC(BC); };
void CPU::INC_B() { INC(B); }
void CPU::DEC_B() { DEC(B); }
void CPU::LD_B_n() { LD_r_n(B); }

void CPU::RLCA() {
	(A & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	A = (A << 1) | FLAG_CHECK(FLAG_CARRY);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::LD_A_BC() {
	A = mmu.Read(BC);

	cycles += 2;
}

void CPU::INC_C() { INC(C); }
void CPU::DEC_C() { DEC(C); }

void CPU::LD_C_n() { LD_r_n(C); }

void CPU::RRCA() {
	(A & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	A = (A >> 1) | (FLAG_CHECK(FLAG_CARRY) << 7);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::LD_DE() {
	DE = nn;

	cycles += 3;
}

void CPU::LD_DE_A() {
	mmu.Write(DE, A);

	cycles += 2;
}

void CPU::INC_DE() { INC(DE); }
void CPU::INC_D() { INC(D); }
void CPU::DEC_D() { DEC(D); }
void CPU::LD_D_n() { LD_r_n(D); }

void CPU::RLA() {
	uint8_t carry = FLAG_CHECK(FLAG_CARRY);
	(A & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	A = (A << 1) & carry;
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::LD_A_DE() {
	A = mmu.Read(DE);

	cycles += 2;
}

void CPU::INC_E() { INC(E); }
void CPU::DEC_E() { DEC(E); }

void CPU::RRA() {
	uint8_t carry = FLAG_CHECK(FLAG_CARRY);
	(A & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	A = (A >> 1) | (carry << 7);
	(A == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 1;
}

void CPU::LD_HL() {
	HL = nn;

	cycles += 3;
}

void CPU::LD_HLI_A() {
	mmu.Write(HL, A);
	HL++;

	cycles += 2;
}

void CPU::INC_HL() { INC(L); }
void CPU::INC_H() { INC(H); }
void CPU::DEC_H() { DEC(H); }
void CPU::LD_H_n() { LD_r_n(H); }

void CPU::LD_A_HLI() {
	A = mmu.Read(HL);
	HL++;

	cycles += 2;
}

void CPU::INC_L() { INC(L); }
void CPU::DEC_L() { DEC(L); }

void CPU::LD_SP() {
	SP = nn;

	cycles += 3;
}

void CPU::LD_HLD_A() {
	mmu.Write(HL, A);
	HL--;

	cycles += 2;
}

void CPU::INC_SP() { INC(SP); }

void CPU::LD_HL_n() {
	mmu.Write(HL, n);

	cycles += 3;
}

void CPU::DEC_A() { DEC(A); }

void CPU::LD_A_HLD() {
	A = mmu.Read(HL);
	HL--;

	cycles += 2;
}

void CPU::INC_A() { INC(A); }
void CPU::LD_A_n() { LD_r_n(A); }
void CPU::LD_B_B() { LD_r_r(B, B); }
void CPU::LD_B_C() { LD_r_r(B, C); }
void CPU::LD_B_D() { LD_r_r(B, D); }
void CPU::LD_B_E() { LD_r_r(B, E); }
void CPU::LD_B_H() { LD_r_r(B, H); }
void CPU::LD_B_L() { LD_r_r(B, L); }
void CPU::LD_B_HL() { LD_r_HL(B); }
void CPU::LD_B_A() { LD_r_r(B, A); }
void CPU::LD_C_B() { LD_r_r(C, B); }
void CPU::LD_C_C() { LD_r_r(C, C); }
void CPU::LD_C_D() { LD_r_r(C, D); }
void CPU::LD_C_E() { LD_r_r(C, E); }
void CPU::LD_C_H() { LD_r_r(C, H); }
void CPU::LD_C_L() { LD_r_r(C, L); }
void CPU::LD_C_HL() { LD_r_HL(C); }
void CPU::LD_C_A() { LD_r_r(C, A); }
void CPU::LD_D_B() { LD_r_r(D, B); }
void CPU::LD_D_C() { LD_r_r(D, C); }
void CPU::LD_D_D() { LD_r_r(D, D); }
void CPU::LD_D_E() { LD_r_r(D, E); }
void CPU::LD_D_H() { LD_r_r(D, H); }
void CPU::LD_D_L() { LD_r_r(D, L); }
void CPU::LD_D_HL() { LD_r_HL(D); }
void CPU::LD_D_A() { LD_r_r(D, A); }
void CPU::LD_E_B() { LD_r_r(E, B); }
void CPU::LD_E_C() { LD_r_r(E, C); }
void CPU::LD_E_D() { LD_r_r(E, D); }
void CPU::LD_E_E() { LD_r_r(E, E); }
void CPU::LD_E_H() { LD_r_r(E, H); }
void CPU::LD_E_L() { LD_r_r(E, L); }
void CPU::LD_E_HL() { LD_r_HL(E); }
void CPU::LD_E_A() { LD_r_r(E, A); }
void CPU::LD_H_B() { LD_r_r(H, B); }
void CPU::LD_H_C() { LD_r_r(H, C); }
void CPU::LD_H_D() { LD_r_r(H, D); }
void CPU::LD_H_E() { LD_r_r(H, E); }
void CPU::LD_H_H() { LD_r_r(H, H); }
void CPU::LD_H_L() { LD_r_r(H, L); }
void CPU::LD_H_HL() { LD_r_HL(H); }
void CPU::LD_H_A() { LD_r_r(H, A); }
void CPU::LD_L_B() { LD_r_r(L, B); }
void CPU::LD_L_C() { LD_r_r(L, C); }
void CPU::LD_L_D() { LD_r_r(L, D); }
void CPU::LD_L_E() { LD_r_r(L, E); }
void CPU::LD_L_H() { LD_r_r(L, H); }
void CPU::LD_L_L() { LD_r_r(L, L); }
void CPU::LD_L_HL() { LD_r_HL(L); }
void CPU::LD_L_A() { LD_r_r(L, A); }
void CPU::LD_HL_B() { LD_HL_r(B); }
void CPU::LD_HL_C() { LD_HL_r(C); }
void CPU::LD_HL_D() { LD_HL_r(D); }
void CPU::LD_HL_E() { LD_HL_r(E); }
void CPU::LD_HL_H() { LD_HL_r(H); }
void CPU::LD_HL_L() { LD_HL_r(L); }
void CPU::LD_HL_A() { LD_HL_r(A); }
void CPU::LD_A_B() { LD_r_r(A, B); }
void CPU::LD_A_C() { LD_r_r(A, C); }
void CPU::LD_A_D() { LD_r_r(A, D); }
void CPU::LD_A_E() { LD_r_r(A, E); }
void CPU::LD_A_H() { LD_r_r(A, H); }
void CPU::LD_A_L() { LD_r_r(A, L); }
void CPU::LD_A_HL() { LD_r_HL(A); }
void CPU::LD_A_A() { LD_r_r(A, A); }

void CPU::AND(uint8_t r) {
	A &= r;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void CPU::AND_A() { AND(A); }
void CPU::AND_B() { AND(B); }
void CPU::AND_C() { AND(C); }
void CPU::AND_D() { AND(D); }
void CPU::AND_E() { AND(E); }
void CPU::AND_H() { AND(H); }
void CPU::AND_L() { AND(L); }
void CPU::AND_HL() { AND(mmu.Read(HL)); cycles += 1; }
void CPU::AND_n() { AND(n); cycles += 1; }

void CPU::OR(uint8_t r) {
	A |= r;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void CPU::OR_A() { OR(A); }
void CPU::OR_B() { OR(B); }
void CPU::OR_C() { OR(C); }
void CPU::OR_D() { OR(D); }
void CPU::OR_E() { OR(E); }
void CPU::OR_H() { OR(H); }
void CPU::OR_L() { OR(L); }
void CPU::OR_HL() { OR(mmu.Read(HL)); cycles += 1; }
void CPU::OR_n() { OR(n); cycles += 1; }

void CPU::XOR(uint8_t r) {
	A ^= r;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
}

void CPU::XOR_A() { XOR(A); }
void CPU::XOR_B() { XOR(B); }
void CPU::XOR_C() { XOR(C); }
void CPU::XOR_D() { XOR(D); }
void CPU::XOR_E() { XOR(E); }
void CPU::XOR_H() { XOR(H); }
void CPU::XOR_L() { XOR(L); }
void CPU::XOR_HL() { XOR(mmu.Read(HL)); cycles += 1; }
void CPU::XOR_n() { XOR(n); cycles += 1; }

void CPU::POP_BC() { POP(BC); }
void CPU::PUSH_BC() { PUSH(BC); }

void CPU::POP_DE() { POP(DE); }
void CPU::PUSH_DE() { PUSH(DE); }

void CPU::LDH_n_A() {
	mmu.Write(0xFF00 + n, A);

	cycles += 3;
}

void CPU::LD_nn_SP() {
	uint8_t hi = mmu.Read(SP - 1);
	uint8_t lo = mmu.Read(SP);
	mmu.Write(nn + 1, hi);
	mmu.Write(nn, lo);

	cycles += 5;
}

void CPU::LD_HL_SP_e() {
	uint32_t result = SP + e;
	(result > UINT16_MAX) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	isHalfCarry16(SP, e, result) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE | FLAG_ZERO);
	HL = result;

	cycles += 3;
}

void CPU::LD_SP_HL() {
	SP = HL;

	cycles += 2;
}

void CPU::POP_HL() { POP(HL); }

void CPU::LD_Ca_A() {
	mmu.Write(0xFF00 + C, A);

	cycles += 2;
}

void CPU::PUSH_HL() { PUSH(HL); }

void CPU::LD_nn_A() {
	mmu.Write(nn, A);

	cycles += 4;
}

void CPU::LDH_A_n() {
	A = mmu.Read(0xFF00 + n);

	cycles += 3;
}

void CPU::POP_AF() { POP(AF);  }

void CPU::LD_A_Ca() {
	A = mmu.Read(0xFF00 + C);

	cycles += 2;
}

void CPU::PUSH_AF() { PUSH(AF); }

void CPU::LD_A_nn() {
	A = mmu.Read(nn);

	cycles += 4;
}