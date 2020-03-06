#include "CPU.h"
#include "Bus.h"

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

CPU::InstructionJumpTable CPU::jumpTable[256] = {
{&CPU::NOP},							//00
{&CPU::LD_BC},				//01
{&CPU::LD_BC_A},				//02
{&CPU::INC_BC},					//03
{&CPU::INC_B},						//04
{&CPU::DEC_B},						//05
{&CPU::LD_B_n},				//06
{&CPU::RLCA},						//07
{&CPU::LD_nn_SP},			//08
{&CPU::ADD_HL_BC},				//09
{&CPU::LD_A_BC},				//0A
{&CPU::DEC_BC},					//0B
{&CPU::INC_C},						//0C
{&CPU::DEC_C},						//0D
{&CPU::LD_C_n},				//0E
{&CPU::RRCA},						//0F
{&CPU::STOP},						//10
{&CPU::LD_DE},				//11
{&CPU::LD_DE_A},				//12
{&CPU::INC_DE},					//13
{&CPU::INC_D},						//14
{&CPU::DEC_D},						//15
{&CPU::LD_D_n},				//16
{&CPU::RLA},							//17
{&CPU::JR},						//18
{&CPU::ADD_HL_DE},				//19
{&CPU::LD_A_DE},				//1A
{&CPU::DEC_DE},					//1B
{&CPU::INC_E},						//1C
{&CPU::DEC_E},						//1D
{&CPU::LD_E_n},				//1E
{&CPU::RRA},							//1F
{&CPU::JR_NZ},				//20
{&CPU::LD_HL},				//21
{&CPU::LD_HLI_A},				//22
{&CPU::INC_HL},					//23
{&CPU::INC_H},						//24
{&CPU::DEC_H},						//25
{&CPU::LD_H_n},				//26
{&CPU::DAA},							//27
{&CPU::JR_Z},					//28
{&CPU::ADD_HL_HL},				//29
{&CPU::LD_A_HLI},				//2A
{&CPU::DEC_HL},					//2B
{&CPU::INC_L},						//2C
{&CPU::DEC_L},						//2D
{&CPU::LD_L_n},				//2E
{&CPU::CPL},							//2F
{&CPU::JR_NC},				//30
{&CPU::LD_SP},				//31
{&CPU::LD_HLD_A},				//32
{&CPU::INC_SP},					//33
{&CPU::INC_HLP},					//34
{&CPU::DEC_HLP},					//35
{&CPU::LD_HL_n},			//36
{&CPU::SCF},							//37
{&CPU::JR_C},					//38
{&CPU::ADD_HL_SP},				//39
{&CPU::LD_A_HLD},				//3A
{&CPU::DEC_SP},					//3B
{&CPU::INC_A},						//3C
{&CPU::DEC_A},						//3D
{&CPU::LD_A_n},				//3E
{&CPU::CCF},							//3F
{&CPU::LD_B_B},					//40
{&CPU::LD_B_C},					//41
{&CPU::LD_B_D},					//42
{&CPU::LD_B_E},					//43
{&CPU::LD_B_H},					//44
{&CPU::LD_B_L},					//45
{&CPU::LD_B_HL},				//46
{&CPU::LD_B_A},					//47
{&CPU::LD_C_B},					//48
{&CPU::LD_C_C},					//49
{&CPU::LD_C_D},					//4A
{&CPU::LD_C_E},					//4B
{&CPU::LD_C_H},					//4C
{&CPU::LD_C_L},					//4D
{&CPU::LD_C_HL},				//4E
{&CPU::LD_C_A},					//4F
{&CPU::LD_D_B},					//50
{&CPU::LD_D_C},					//51
{&CPU::LD_D_D},					//52
{&CPU::LD_D_E},					//53
{&CPU::LD_D_H},					//54
{&CPU::LD_D_L},					//55
{&CPU::LD_D_HL},				//56
{&CPU::LD_D_A},					//57
{&CPU::LD_E_B},					//58
{&CPU::LD_E_C},					//59
{&CPU::LD_E_D},					//5A
{&CPU::LD_E_E},					//5B
{&CPU::LD_E_H},					//5C
{&CPU::LD_E_L},					//5D
{&CPU::LD_E_HL},				//5E
{&CPU::LD_E_A},					//5F
{&CPU::LD_H_B},					//60
{&CPU::LD_H_C},					//61
{&CPU::LD_H_D},					//62
{&CPU::LD_H_E},					//63
{&CPU::LD_H_H},					//64
{&CPU::LD_H_L},					//65
{&CPU::LD_H_HL},				//66
{&CPU::LD_H_A},					//67
{&CPU::LD_L_B},					//68
{&CPU::LD_L_C},					//69
{&CPU::LD_L_D},					//6A
{&CPU::LD_L_E},					//6B
{&CPU::LD_L_H},					//6C
{&CPU::LD_L_L},					//6D
{&CPU::LD_L_HL},				//6E
{&CPU::LD_L_A},					//6F
{&CPU::LD_HL_B},				//70
{&CPU::LD_HL_C},				//71
{&CPU::LD_HL_D},				//72
{&CPU::LD_HL_E},				//73
{&CPU::LD_HL_H},				//74
{&CPU::LD_HL_L},				//75
{&CPU::HALT},						//76
{&CPU::LD_HL_A},				//77
{&CPU::LD_A_B},					//78
{&CPU::LD_A_C},					//79
{&CPU::LD_A_D},					//7A
{&CPU::LD_A_E},					//7B
{&CPU::LD_A_H},					//7C
{&CPU::LD_A_L},					//7D
{&CPU::LD_A_HL},				//7E
{&CPU::LD_A_A},					//7F
{&CPU::ADD_B},						//80
{&CPU::ADD_C},						//81
{&CPU::ADD_D},						//82
{&CPU::ADD_E},						//83
{&CPU::ADD_H},						//84
{&CPU::ADD_L},						//85
{&CPU::ADD_HL},					//86
{&CPU::ADD_A},						//87
{&CPU::ADC_B},						//88
{&CPU::ADC_C},						//89
{&CPU::ADC_D},						//8A
{&CPU::ADC_E},						//8B
{&CPU::ADC_H},						//8C
{&CPU::ADC_L},						//8D
{&CPU::ADC_HL},					//8E
{&CPU::ADC_A},						//8F
{&CPU::SUB_B},						//90
{&CPU::SUB_C},						//91
{&CPU::SUB_D},						//92
{&CPU::SUB_E},						//93
{&CPU::SUB_H},						//94
{&CPU::SUB_L},						//95
{&CPU::SUB_HL},					//96
{&CPU::SUB_A},						//97
{&CPU::SBC_B},						//98
{&CPU::SBC_C},						//99
{&CPU::SBC_D},						//9A
{&CPU::SBC_E},						//9B
{&CPU::SBC_H},						//9C
{&CPU::SBC_L},						//9D
{&CPU::SBC_HL},					//9E
{&CPU::SBC_A},						//9F
{&CPU::AND_B},						//A0
{&CPU::AND_C},						//A1
{&CPU::AND_D},						//A2
{&CPU::AND_E},						//A3
{&CPU::AND_H},						//A4
{&CPU::AND_L},						//A5
{&CPU::AND_HL},					//A6
{&CPU::AND_A},						//A7
{&CPU::XOR_B},						//A8
{&CPU::XOR_C},						//A9
{&CPU::XOR_D},						//AA
{&CPU::XOR_E},						//AB
{&CPU::XOR_H},						//AC
{&CPU::XOR_L},						//AD
{&CPU::XOR_HL},					//AE
{&CPU::XOR_A},						//AF
{&CPU::OR_B},						//B0
{&CPU::OR_C},						//B1
{&CPU::OR_D},						//B2
{&CPU::OR_E},						//B3
{&CPU::OR_H},						//B4
{&CPU::OR_L},						//B5
{&CPU::OR_HL},					//B6
{&CPU::OR_A},						//B7
{&CPU::CP_B},						//B8
{&CPU::CP_C},						//B9
{&CPU::CP_D},						//BA
{&CPU::CP_E},						//BB
{&CPU::CP_H},						//BC
{&CPU::CP_L},						//BD
{&CPU::CP_HL},					//BE
{&CPU::CP_A},						//BF
{&CPU::RET_NZ},					//C0
{&CPU::POP_BC},					//C1
{&CPU::JP_NZ},				//C2
{&CPU::JP},						//C3
{&CPU::CALL_NZ},			//C4
{&CPU::PUSH_BC},					//C5
{&CPU::ADD_n},					//C6
{&CPU::RST_00},					//C7
{&CPU::RET_Z},						//C8
{&CPU::RET},							//C9
{&CPU::JP_Z},					//CA
{&CPU::CB},					//CB
{&CPU::CALL_Z},				//CC
{&CPU::CALL},					//CD
{&CPU::ADC_n},					//CE
{&CPU::RST_08},					//CF
{&CPU::RET_NC},					//D0
{&CPU::POP_DE},					//D1
{&CPU::JP_NC},				//D2
{&CPU::UNDEFINED},			//D3
{&CPU::CALL_NC},			//D4
{&CPU::PUSH_DE},					//D5
{&CPU::SUB_n},					//D6
{&CPU::RST_10},					//D7
{&CPU::RET_C},						//D8
{&CPU::RETI},						//D9
{&CPU::JP_C},					//DA
{&CPU::UNDEFINED},			//DB
{&CPU::CALL_C},				//DC
{&CPU::UNDEFINED},			//DD
{&CPU::SBC_n},					//DE
{&CPU::RST_18},					//DF
{&CPU::LDH_n_A},		//E0
{&CPU::POP_HL},					//E1
{&CPU::LD_Ca_A},			//E2
{&CPU::UNDEFINED},			//E3
{&CPU::UNDEFINED},			//E4
{&CPU::PUSH_HL},					//E5
{&CPU::AND_n},					//E6
{&CPU::RST_20},					//E7
{&CPU::ADD_SP_e},			//E8
{&CPU::JP_HL},						//E9
{&CPU::LD_nn_A},				//EA
{&CPU::UNDEFINED},			//EB
{&CPU::UNDEFINED},			//EC
{&CPU::UNDEFINED},			//ED
{&CPU::XOR_n},					//EE
{&CPU::RST_28},					//EF
{&CPU::LDH_A_n},		//F0
{&CPU::POP_AF},					//F1
{&CPU::LD_A_Ca},			//F2
{&CPU::DI},							//F3
{&CPU::UNDEFINED},			//F4
{&CPU::PUSH_AF},					//F5
{&CPU::OR_n},						//F6
{&CPU::RST_30},					//F7
{&CPU::LD_HL_SP_e},		//F8
{&CPU::LD_SP_HL},				//F9
{&CPU::LD_A_nn},				//FA
{&CPU::EI},							//FB
{&CPU::UNDEFINED},			//FC
{&CPU::UNDEFINED},			//FD
{&CPU::CP_n},						//FE
{&CPU::RST_38},					//FF
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
	r = bus->Read(HL);

	cycles += 2;
}

void CPU::LD_HL_r(uint8_t r) {
	bus->Write(HL, r);

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
void CPU::ADD_HL() { ADD(bus->Read(HL)); cycles += 1; }
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
void CPU::ADC_HL() { ADC(bus->Read(HL)); cycles += 1; }
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
void CPU::SUB_HL() { SUB(bus->Read(HL)); cycles += 1; }
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
void CPU::SBC_HL() { SBC(bus->Read(HL)); cycles += 1; }
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
void CPU::CP_HL() { CP(bus->Read(HL)); cycles += 1; }

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
	uint8_t value = bus->Read(HL);
	value++;
	bus->Write(HL, value);

	cycles += 4; 
}
void CPU::DEC_HLP() { 
	uint8_t value = bus->Read(HL);
	value--;
	bus->Write(HL, value);

	cycles += 4; 
}

void CPU::POP(uint16_t& rr) {
	rr = static_cast<uint16_t>(bus->Read(SP));
	rr |= static_cast<uint16_t>(bus->Read(SP + 1) << 8);
	SP += 2;

	cycles += 3;
}

void CPU::PUSH(uint16_t rr) {
	uint8_t hi = static_cast<uint8_t>((rr & 0xFF00) >> 8);
	uint8_t lo = static_cast<uint8_t>(rr & 0x00FF);
	bus->Write(SP - 1, hi);
	bus->Write(SP - 2, lo);
	SP -= 2;

	cycles += 4;
}

void CPU::RET() {
	uint16_t hi = static_cast<uint16_t>(bus->Read(SP + 1) << 8);
	uint16_t lo = static_cast<uint16_t>(bus->Read(SP));
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
	bus->Write(SP - 1, hi);
	bus->Write(SP - 2, lo);
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
	bus->Write(SP - 1, hi);
	bus->Write(SP - 2, lo);
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
	bus->Write(BC, A);

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
	A = bus->Read(BC);

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
	bus->Write(DE, A);

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
	A = bus->Read(DE);

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
	bus->Write(HL, A);
	HL++;

	cycles += 2;
}

void CPU::INC_HL() { INC(L); }
void CPU::INC_H() { INC(H); }
void CPU::DEC_H() { DEC(H); }
void CPU::LD_H_n() { LD_r_n(H); }

void CPU::LD_A_HLI() {
	A = bus->Read(HL);
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
	bus->Write(HL, A);
	HL--;

	cycles += 2;
}

void CPU::INC_SP() { INC(SP); }

void CPU::LD_HL_n() {
	bus->Write(HL, n);

	cycles += 3;
}

void CPU::DEC_A() { DEC(A); }

void CPU::LD_A_HLD() {
	A = bus->Read(HL);
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
void CPU::AND_HL() { AND(bus->Read(HL)); cycles += 1; }
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
void CPU::OR_HL() { OR(bus->Read(HL)); cycles += 1; }
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
void CPU::XOR_HL() { XOR(bus->Read(HL)); cycles += 1; }
void CPU::XOR_n() { XOR(n); cycles += 1; }

void CPU::POP_BC() { POP(BC); }
void CPU::PUSH_BC() { PUSH(BC); }

void CPU::POP_DE() { POP(DE); }
void CPU::PUSH_DE() { PUSH(DE); }

void CPU::LDH_n_A() {
	bus->Write(0xFF00 + n, A);

	cycles += 3;
}

void CPU::LD_nn_SP() {
	uint8_t hi = bus->Read(SP - 1);
	uint8_t lo = bus->Read(SP);
	bus->Write(nn + 1, hi);
	bus->Write(nn, lo);

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
	bus->Write(0xFF00 + C, A);

	cycles += 2;
}

void CPU::PUSH_HL() { PUSH(HL); }

void CPU::LD_nn_A() {
	bus->Write(nn, A);

	cycles += 4;
}

void CPU::LDH_A_n() {
	A = bus->Read(0xFF00 + n);

	cycles += 3;
}

void CPU::POP_AF() { POP(AF);  }

void CPU::LD_A_Ca() {
	A = bus->Read(0xFF00 + C);

	cycles += 2;
}

void CPU::PUSH_AF() { PUSH(AF); }

void CPU::LD_A_nn() {
	A = bus->Read(nn);

	cycles += 4;
}