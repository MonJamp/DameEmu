#include "CPU.h"
#include "Bus.h"

CPU::InstructionJumpTable CPU::cb_jumpTable[256] = {
{&CPU::RLC_B},					//00
{&CPU::RLC_C},					//01
{&CPU::RLC_D},					//02
{&CPU::RLC_E},					//03
{&CPU::RLC_H},					//04
{&CPU::RLC_L},					//05
{&CPU::RLC_HL},				//06
{&CPU::RLC_A},					//07
{&CPU::RRC_B},					//08
{&CPU::RRC_C},					//09
{&CPU::RRC_D},					//0A
{&CPU::RRC_E},					//0B
{&CPU::RRC_H},					//0C
{&CPU::RRC_L},					//0D
{&CPU::RRC_HL},				//0E
{&CPU::RRC_A},					//0F
{&CPU::RL_B},					//10
{&CPU::RL_C},					//11
{&CPU::RL_D},					//12
{&CPU::RL_E},					//13
{&CPU::RL_H},					//14
{&CPU::RL_L},					//15
{&CPU::RL_HL},				//16
{&CPU::RL_A},					//17
{&CPU::RR_B},					//18
{&CPU::RR_C},					//19
{&CPU::RR_D},					//1A
{&CPU::RR_E},					//1B
{&CPU::RR_H},					//1C
{&CPU::RR_L},					//1D
{&CPU::RR_HL},				//1E
{&CPU::RR_A},					//1F
{&CPU::SLA_B},					//20
{&CPU::SLA_C},					//21
{&CPU::SLA_D},					//22
{&CPU::SLA_E},					//23
{&CPU::SLA_H},					//24
{&CPU::SLA_L},					//25
{&CPU::SLA_HL},				//26
{&CPU::SRA_A},					//27
{&CPU::SRA_B},					//28
{&CPU::SRA_C},					//29
{&CPU::SRA_D},					//2A
{&CPU::SRA_E},					//2B
{&CPU::SRA_H},					//2C
{&CPU::SRA_L},					//2D
{&CPU::SRA_HL},				//2E
{&CPU::SRA_A},					//2F
{&CPU::SWAP_B},				//30
{&CPU::SWAP_C},				//31
{&CPU::SWAP_D},				//32
{&CPU::SWAP_E},				//33
{&CPU::SWAP_H},				//34
{&CPU::SWAP_L},				//35
{&CPU::SWAP_HL},			//36
{&CPU::SWAP_A},				//37
{&CPU::SRL_B},					//38
{&CPU::SRL_C},					//39
{&CPU::SRL_D},					//3A
{&CPU::SRL_E},					//3B
{&CPU::SRL_H},					//3C
{&CPU::SRL_L},					//3D
{&CPU::SRL_HL},				//3E
{&CPU::SRL_A},					//3F
{&CPU::BIT_0_B},				//40
{&CPU::BIT_0_C},				//41
{&CPU::BIT_0_D},				//42
{&CPU::BIT_0_E},				//43
{&CPU::BIT_0_H},				//44
{&CPU::BIT_0_L},				//45
{&CPU::BIT_0_HL},			//46
{&CPU::BIT_0_A},				//47
{&CPU::BIT_1_B},				//48
{&CPU::BIT_1_C},				//49
{&CPU::BIT_1_D},				//4A
{&CPU::BIT_1_E},				//4B
{&CPU::BIT_1_H},				//4C
{&CPU::BIT_1_L},				//4D
{&CPU::BIT_1_HL},			//4E
{&CPU::BIT_1_A},				//4F
{&CPU::BIT_2_B},				//50
{&CPU::BIT_2_C},				//51
{&CPU::BIT_2_D},				//52
{&CPU::BIT_2_E},				//53
{&CPU::BIT_2_H},				//54
{&CPU::BIT_2_L},				//55
{&CPU::BIT_2_HL},			//56
{&CPU::BIT_2_A},				//57
{&CPU::BIT_3_B},				//58
{&CPU::BIT_3_C},				//59
{&CPU::BIT_3_D},				//5A
{&CPU::BIT_3_E},				//5B
{&CPU::BIT_3_H},				//5C
{&CPU::BIT_3_L},				//5D
{&CPU::BIT_3_HL},			//5E
{&CPU::BIT_3_A},				//5F
{&CPU::BIT_4_B},				//60
{&CPU::BIT_4_C},				//61
{&CPU::BIT_4_D},				//62
{&CPU::BIT_4_E},				//63
{&CPU::BIT_4_H},				//64
{&CPU::BIT_4_L},				//65
{&CPU::BIT_4_HL},			//66
{&CPU::BIT_4_A},				//67
{&CPU::BIT_5_B},				//68
{&CPU::BIT_5_C},				//69
{&CPU::BIT_5_D},				//6A
{&CPU::BIT_5_E},				//6B
{&CPU::BIT_5_H},				//6C
{&CPU::BIT_5_L},				//6D
{&CPU::BIT_5_HL},			//6E
{&CPU::BIT_5_A},				//6F
{&CPU::BIT_6_B},				//70
{&CPU::BIT_6_C},				//71
{&CPU::BIT_6_D},				//72
{&CPU::BIT_6_E},				//73
{&CPU::BIT_6_H},				//74
{&CPU::BIT_6_L},				//75
{&CPU::BIT_6_HL},			//76
{&CPU::BIT_6_A},				//77
{&CPU::BIT_7_B},				//78
{&CPU::BIT_7_C},				//79
{&CPU::BIT_7_D},				//7A
{&CPU::BIT_7_E},				//7B
{&CPU::BIT_7_H},				//7C
{&CPU::BIT_7_L},				//7D
{&CPU::BIT_7_HL},			//7E
{&CPU::BIT_7_A},				//7F
{&CPU::RES_0_B},				//80
{&CPU::RES_0_C},				//81
{&CPU::RES_0_D},				//82
{&CPU::RES_0_E},				//83
{&CPU::RES_0_H},				//84
{&CPU::RES_0_L},				//85
{&CPU::RES_0_HL},			//86
{&CPU::RES_0_A},				//87
{&CPU::RES_1_B},				//88
{&CPU::RES_1_C},				//89
{&CPU::RES_1_D},				//8A
{&CPU::RES_1_E},				//8B
{&CPU::RES_1_H},				//8C
{&CPU::RES_1_L},				//8D
{&CPU::RES_1_HL},			//8E
{&CPU::RES_1_A},				//8F
{&CPU::RES_2_B},				//90
{&CPU::RES_2_C},				//91
{&CPU::RES_2_D},				//92
{&CPU::RES_2_E},				//93
{&CPU::RES_2_H},				//94
{&CPU::RES_2_L},				//95
{&CPU::RES_2_HL},			//96
{&CPU::RES_2_A},				//97
{&CPU::RES_3_B},				//98
{&CPU::RES_3_C},				//99
{&CPU::RES_3_D},				//9A
{&CPU::RES_3_E},				//9B
{&CPU::RES_3_H},				//9C
{&CPU::RES_3_L},				//9D
{&CPU::RES_3_HL},			//9E
{&CPU::RES_3_A},				//9F
{&CPU::RES_4_B},				//A0
{&CPU::RES_4_C},				//A1
{&CPU::RES_4_D},				//A2
{&CPU::RES_4_E},				//A3
{&CPU::RES_4_H},				//A4
{&CPU::RES_4_L},				//A5
{&CPU::RES_4_HL},			//A6
{&CPU::RES_4_A},				//A7
{&CPU::RES_5_B},				//A8
{&CPU::RES_5_C},				//A9
{&CPU::RES_5_D},				//AA
{&CPU::RES_5_E},				//AB
{&CPU::RES_5_H},				//AC
{&CPU::RES_5_L},				//AD
{&CPU::RES_5_HL},			//AE
{&CPU::RES_5_A},				//AF
{&CPU::RES_6_B},				//B0
{&CPU::RES_6_C},				//B1
{&CPU::RES_6_D},				//B2
{&CPU::RES_6_E},				//B3
{&CPU::RES_6_H},				//B4
{&CPU::RES_6_L},				//B5
{&CPU::RES_6_HL},			//B6
{&CPU::RES_6_A},				//B7
{&CPU::RES_7_B},				//B8
{&CPU::RES_7_C},				//B9
{&CPU::RES_7_D},				//BA
{&CPU::RES_7_E},				//BB
{&CPU::RES_7_H},				//BC
{&CPU::RES_7_L},				//BD
{&CPU::RES_7_HL},			//BE
{&CPU::RES_7_A},				//BF
{&CPU::SET_0_B},				//C0
{&CPU::SET_0_C},				//C1
{&CPU::SET_0_D},				//C2
{&CPU::SET_0_E},				//C3
{&CPU::SET_0_H},				//C4
{&CPU::SET_0_L},				//C5
{&CPU::SET_0_HL},			//C6
{&CPU::SET_0_A},				//C7
{&CPU::SET_1_B},				//C8
{&CPU::SET_1_C},				//C9
{&CPU::SET_1_D},				//CA
{&CPU::SET_1_E},				//CB
{&CPU::SET_1_H},				//CC
{&CPU::SET_1_L},				//CD
{&CPU::SET_1_HL},			//CE
{&CPU::SET_1_A},				//CF
{&CPU::SET_2_B},				//D0
{&CPU::SET_2_C},				//D1
{&CPU::SET_2_D},				//D2
{&CPU::SET_2_E},				//D3
{&CPU::SET_2_H},				//D4
{&CPU::SET_2_L},				//D5
{&CPU::SET_2_HL},			//D6
{&CPU::SET_2_A},				//D7
{&CPU::SET_3_B},				//D8
{&CPU::SET_3_C},				//D9
{&CPU::SET_3_D},				//DA
{&CPU::SET_3_E},				//DB
{&CPU::SET_3_H},				//DC
{&CPU::SET_3_L},				//DD
{&CPU::SET_3_HL},			//DE
{&CPU::SET_3_A},				//DF
{&CPU::SET_4_B},				//E0
{&CPU::SET_4_C},				//E1
{&CPU::SET_4_D},				//E2
{&CPU::SET_4_E},				//E3
{&CPU::SET_4_H},				//E4
{&CPU::SET_4_L},				//E5
{&CPU::SET_4_HL},			//E6
{&CPU::SET_4_A},				//E7
{&CPU::SET_5_B},				//E8
{&CPU::SET_5_C},				//E9
{&CPU::SET_5_D},				//EA
{&CPU::SET_5_E},				//EB
{&CPU::SET_5_H},				//EC
{&CPU::SET_5_L},				//ED
{&CPU::SET_5_HL},			//EE
{&CPU::SET_5_A},				//EF
{&CPU::SET_6_B},				//F0
{&CPU::SET_6_C},				//F1
{&CPU::SET_6_D},				//F2
{&CPU::SET_6_E},				//F3
{&CPU::SET_6_H},				//F4
{&CPU::SET_6_L},				//F5
{&CPU::SET_6_HL},			//F6
{&CPU::SET_6_A},				//F7
{&CPU::SET_7_B},				//F8
{&CPU::SET_7_C},				//F9
{&CPU::SET_7_D},				//FA
{&CPU::SET_7_E},				//FB
{&CPU::SET_7_H},				//FC
{&CPU::SET_7_L},				//FD
{&CPU::SET_7_HL},			//FE
{&CPU::SET_7_A},				//FF
};



void CPU::RLC(uint8_t& r) {
	(r & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r << 1) | FLAG_CHECK(FLAG_CARRY);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::RLC_HL() {
	uint8_t value = bus->Read(HL);
	RLC(value);
	bus->Write(HL, value);

	cycles += 2;
}

void CPU::RLC_A() { RRC(A); }
void CPU::RLC_B() { RLC(B); }
void CPU::RLC_C() { RLC(C); }
void CPU::RLC_D() { RLC(D); }
void CPU::RLC_E() { RLC(E); }
void CPU::RLC_H() { RLC(H); }
void CPU::RLC_L() { RLC(L); }

void CPU::RRC(uint8_t& r) {
	(r & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r >> 1) | (FLAG_CHECK(FLAG_CARRY) << 7);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::RRC_HL() {
	uint8_t value = bus->Read(HL);
	RRC(value);
	bus->Write(HL, value);

	cycles += 2;
}

void CPU::RRC_A() { RRC(A); }
void CPU::RRC_B() { RRC(B); }
void CPU::RRC_C() { RRC(C); }
void CPU::RRC_D() { RRC(D); }
void CPU::RRC_E() { RRC(E); }
void CPU::RRC_H() { RRC(H); }
void CPU::RRC_L() { RRC(L); }

void CPU::RL(uint8_t& r) {
	uint8_t carry = FLAG_CHECK(FLAG_CARRY);
	(r & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r << 1) | carry;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::RL_HL() {
	uint8_t value = bus->Read(HL);
	RL(value);
	bus->Write(HL, value);

	cycles += 2;
}

void CPU::RL_A() { RL(A); }
void CPU::RL_B() { RL(B); }
void CPU::RL_C() { RL(C); }
void CPU::RL_D() { RL(D); }
void CPU::RL_E() { RL(E); }
void CPU::RL_H() { RL(H); }
void CPU::RL_L() { RL(L); }

void CPU::RR(uint8_t& r) {
	uint8_t carry = FLAG_CHECK(FLAG_CARRY);
	(r & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r >> 1) | (carry << 7);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::RR_HL() {
	uint8_t value = bus->Read(HL);
	RR(value);
	bus->Write(HL, value);

	cycles += 2;
}

void CPU::RR_A() { RR(A); }
void CPU::RR_B() { RR(B); }
void CPU::RR_C() { RR(C); }
void CPU::RR_D() { RR(D); }
void CPU::RR_E() { RR(E); }
void CPU::RR_H() { RR(H); }
void CPU::RR_L() { RR(L); }

void CPU::BIT(uint8_t b, uint8_t r) {
	(r & (1 << b)) ? FLAG_CLEAR(FLAG_ZERO) : FLAG_SET(FLAG_ZERO);

	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::BIT_HL(uint8_t b) {
	uint8_t value = bus->Read(HL);
	BIT(b, value);
	bus->Write(HL, value);

	cycles += 3;
}

void CPU::BIT_0_B() { BIT(0, B); }
void CPU::BIT_0_C() { BIT(0, C); }
void CPU::BIT_0_D() { BIT(0, D); }
void CPU::BIT_0_E() { BIT(0, E); }
void CPU::BIT_0_H() { BIT(0, H); }
void CPU::BIT_0_L() { BIT(0, L); }
void CPU::BIT_0_HL() { BIT_HL(0); }
void CPU::BIT_0_A() { BIT(0, A); }
void CPU::BIT_1_B() { BIT(1, B); }
void CPU::BIT_1_C() { BIT(1, C); }
void CPU::BIT_1_D() { BIT(1, D); }
void CPU::BIT_1_E() { BIT(1, E); }
void CPU::BIT_1_H() { BIT(1, H); }
void CPU::BIT_1_L() { BIT(1, L); }
void CPU::BIT_1_HL() { BIT_HL(1); }
void CPU::BIT_1_A() { BIT(1, A); }
void CPU::BIT_2_B() { BIT(2, B); }
void CPU::BIT_2_C() { BIT(2, C); }
void CPU::BIT_2_D() { BIT(2, D); }
void CPU::BIT_2_E() { BIT(2, E); }
void CPU::BIT_2_H() { BIT(2, H); }
void CPU::BIT_2_L() { BIT(2, L); }
void CPU::BIT_2_HL() { BIT_HL(2); }
void CPU::BIT_2_A() { BIT(2, A); }
void CPU::BIT_3_B() { BIT(3, B); }
void CPU::BIT_3_C() { BIT(3, C); }
void CPU::BIT_3_D() { BIT(3, D); }
void CPU::BIT_3_E() { BIT(3, E); }
void CPU::BIT_3_H() { BIT(3, H); }
void CPU::BIT_3_L() { BIT(3, L); }
void CPU::BIT_3_HL() { BIT_HL(3); }
void CPU::BIT_3_A() { BIT(3, A); }
void CPU::BIT_4_B() { BIT(4, B); }
void CPU::BIT_4_C() { BIT(4, C); }
void CPU::BIT_4_D() { BIT(4, D); }
void CPU::BIT_4_E() { BIT(4, E); }
void CPU::BIT_4_H() { BIT(4, H); }
void CPU::BIT_4_L() { BIT(4, L); }
void CPU::BIT_4_HL() { BIT_HL(4); }
void CPU::BIT_4_A() { BIT(4, A); }
void CPU::BIT_5_B() { BIT(5, B); }
void CPU::BIT_5_C() { BIT(5, C); }
void CPU::BIT_5_D() { BIT(5, D); }
void CPU::BIT_5_E() { BIT(5, E); }
void CPU::BIT_5_H() { BIT(5, H); }
void CPU::BIT_5_L() { BIT(5, L); }
void CPU::BIT_5_HL() { BIT_HL(5); }
void CPU::BIT_5_A() { BIT(5, A); }
void CPU::BIT_6_B() { BIT(6, B); }
void CPU::BIT_6_C() { BIT(6, C); }
void CPU::BIT_6_D() { BIT(6, D); }
void CPU::BIT_6_E() { BIT(6, E); }
void CPU::BIT_6_H() { BIT(6, H); }
void CPU::BIT_6_L() { BIT(6, L); }
void CPU::BIT_6_HL() { BIT_HL(6); }
void CPU::BIT_6_A() { BIT(6, A); }
void CPU::BIT_7_B() { BIT(7, B); }
void CPU::BIT_7_C() { BIT(7, C); }
void CPU::BIT_7_D() { BIT(7, D); }
void CPU::BIT_7_E() { BIT(7, E); }
void CPU::BIT_7_H() { BIT(7, H); }
void CPU::BIT_7_L() { BIT(7, L); }
void CPU::BIT_7_HL() { BIT_HL(7); }
void CPU::BIT_7_A() { BIT(7, A); }

void CPU::SET(uint8_t b, uint8_t& r) {
	r |= (1 << b);

	cycles += 2;
}

void CPU::SET_HL(uint8_t b) {
	uint8_t value = bus->Read(HL);
	SET(b, value);
	bus->Write(HL, value);

	cycles += 2;
}

void CPU::SET_0_A() { SET(0, A); }
void CPU::SET_0_B() { SET(0, B); }
void CPU::SET_0_C() { SET(0, C); }
void CPU::SET_0_D() { SET(0, D); }
void CPU::SET_0_E() { SET(0, E); }
void CPU::SET_0_H() { SET(0, H); }
void CPU::SET_0_L() { SET(0, L); }
void CPU::SET_0_HL() { SET_HL(0); }
void CPU::SET_1_A() { SET(1, A); }
void CPU::SET_1_B() { SET(1, B); }
void CPU::SET_1_C() { SET(1, C); }
void CPU::SET_1_D() { SET(1, D); }
void CPU::SET_1_E() { SET(1, E); }
void CPU::SET_1_H() { SET(1, H); }
void CPU::SET_1_L() { SET(1, L); }
void CPU::SET_1_HL() { SET_HL(1); }
void CPU::SET_2_A() { SET(2, A); }
void CPU::SET_2_B() { SET(2, B); }
void CPU::SET_2_C() { SET(2, C); }
void CPU::SET_2_D() { SET(2, D); }
void CPU::SET_2_E() { SET(2, E); }
void CPU::SET_2_H() { SET(2, H); }
void CPU::SET_2_L() { SET(2, L); }
void CPU::SET_2_HL() { SET_HL(2); }
void CPU::SET_3_A() { SET(3, A); }
void CPU::SET_3_B() { SET(3, B); }
void CPU::SET_3_C() { SET(3, C); }
void CPU::SET_3_D() { SET(3, D); }
void CPU::SET_3_E() { SET(3, E); }
void CPU::SET_3_H() { SET(3, H); }
void CPU::SET_3_L() { SET(3, L); }
void CPU::SET_3_HL() { SET_HL(3); }
void CPU::SET_4_A() { SET(4, A); }
void CPU::SET_4_B() { SET(4, B); }
void CPU::SET_4_C() { SET(4, C); }
void CPU::SET_4_D() { SET(4, D); }
void CPU::SET_4_E() { SET(4, E); }
void CPU::SET_4_H() { SET(4, H); }
void CPU::SET_4_L() { SET(4, L); }
void CPU::SET_4_HL() { SET_HL(4); }
void CPU::SET_5_A() { SET(5, A); }
void CPU::SET_5_B() { SET(5, B); }
void CPU::SET_5_C() { SET(5, C); }
void CPU::SET_5_D() { SET(5, D); }
void CPU::SET_5_E() { SET(5, E); }
void CPU::SET_5_H() { SET(5, H); }
void CPU::SET_5_L() { SET(5, L); }
void CPU::SET_5_HL() { SET_HL(5); }
void CPU::SET_6_A() { SET(6, A); }
void CPU::SET_6_B() { SET(6, B); }
void CPU::SET_6_C() { SET(6, C); }
void CPU::SET_6_D() { SET(6, D); }
void CPU::SET_6_E() { SET(6, E); }
void CPU::SET_6_H() { SET(6, H); }
void CPU::SET_6_L() { SET(6, L); }
void CPU::SET_6_HL() { SET_HL(6); }
void CPU::SET_7_A() { SET(7, A); }
void CPU::SET_7_B() { SET(7, B); }
void CPU::SET_7_C() { SET(7, C); }
void CPU::SET_7_D() { SET(7, D); }
void CPU::SET_7_E() { SET(7, E); }
void CPU::SET_7_H() { SET(7, H); }
void CPU::SET_7_L() { SET(7, L); }
void CPU::SET_7_HL() { SET_HL(7); }

void CPU::RES(uint8_t b, uint8_t& r) {
	r &= ~(1 << b);

	cycles += 2;
}

void CPU::RES_HL(uint8_t b) {
	uint8_t value = bus->Read(HL);
	RES(b, value);
	bus->Write(HL, value);

	cycles += 2;
}

void CPU::RES_0_A() { RES(0, A); }
void CPU::RES_0_B() { RES(0, B); }
void CPU::RES_0_C() { RES(0, C); }
void CPU::RES_0_D() { RES(0, D); }
void CPU::RES_0_E() { RES(0, E); }
void CPU::RES_0_H() { RES(0, H); }
void CPU::RES_0_L() { RES(0, L); }
void CPU::RES_0_HL() { RES_HL(0); }
void CPU::RES_1_A() { RES(1, A); }
void CPU::RES_1_B() { RES(1, B); }
void CPU::RES_1_C() { RES(1, C); }
void CPU::RES_1_D() { RES(1, D); }
void CPU::RES_1_E() { RES(1, E); }
void CPU::RES_1_H() { RES(1, H); }
void CPU::RES_1_L() { RES(1, L); }
void CPU::RES_1_HL() { RES_HL(1); }
void CPU::RES_2_A() { RES(2, A); }
void CPU::RES_2_B() { RES(2, B); }
void CPU::RES_2_C() { RES(2, C); }
void CPU::RES_2_D() { RES(2, D); }
void CPU::RES_2_E() { RES(2, E); }
void CPU::RES_2_H() { RES(2, H); }
void CPU::RES_2_L() { RES(2, L); }
void CPU::RES_2_HL() { RES_HL(2); }
void CPU::RES_3_A() { RES(3, A); }
void CPU::RES_3_B() { RES(3, B); }
void CPU::RES_3_C() { RES(3, C); }
void CPU::RES_3_D() { RES(3, D); }
void CPU::RES_3_E() { RES(3, E); }
void CPU::RES_3_H() { RES(3, H); }
void CPU::RES_3_L() { RES(3, L); }
void CPU::RES_3_HL() { RES_HL(3); }
void CPU::RES_4_A() { RES(4, A); }
void CPU::RES_4_B() { RES(4, B); }
void CPU::RES_4_C() { RES(4, C); }
void CPU::RES_4_D() { RES(4, D); }
void CPU::RES_4_E() { RES(4, E); }
void CPU::RES_4_H() { RES(4, H); }
void CPU::RES_4_L() { RES(4, L); }
void CPU::RES_4_HL() { RES_HL(4); }
void CPU::RES_5_A() { RES(5, A); }
void CPU::RES_5_B() { RES(5, B); }
void CPU::RES_5_C() { RES(5, C); }
void CPU::RES_5_D() { RES(5, D); }
void CPU::RES_5_E() { RES(5, E); }
void CPU::RES_5_H() { RES(5, H); }
void CPU::RES_5_L() { RES(5, L); }
void CPU::RES_5_HL() { RES_HL(5); }
void CPU::RES_6_A() { RES(6, A); }
void CPU::RES_6_B() { RES(6, B); }
void CPU::RES_6_C() { RES(6, C); }
void CPU::RES_6_D() { RES(6, D); }
void CPU::RES_6_E() { RES(6, E); }
void CPU::RES_6_H() { RES(6, H); }
void CPU::RES_6_L() { RES(6, L); }
void CPU::RES_6_HL() { RES_HL(6); }
void CPU::RES_7_A() { RES(7, A); }
void CPU::RES_7_B() { RES(7, B); }
void CPU::RES_7_C() { RES(7, C); }
void CPU::RES_7_D() { RES(7, D); }
void CPU::RES_7_E() { RES(7, E); }
void CPU::RES_7_H() { RES(7, H); }
void CPU::RES_7_L() { RES(7, L); }
void CPU::RES_7_HL() { RES_HL(7); }

void CPU::SLA(uint8_t& r) {
	(r & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r << 1);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::SLA_A() { SLA(A); }
void CPU::SLA_B() { SLA(B); }
void CPU::SLA_C() { SLA(C); }
void CPU::SLA_D() { SLA(D); }
void CPU::SLA_E() { SLA(E); }
void CPU::SLA_H() { SLA(H); }
void CPU::SLA_L() { SLA(L); }

void CPU::SLA_HL() {
	uint8_t value = bus->Read(HL);
	SLA(value);
	bus->Write(HL, value);

	cycles += 2;
}

void CPU::SRA(uint8_t& r) {
	uint8_t bit7 = r & (1 << 7);
	(r & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r >> 1)& bit7;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::SRA_A() { SRA(A); }
void CPU::SRA_B() { SRA(B); }
void CPU::SRA_C() { SRA(C); }
void CPU::SRA_D() { SRA(D); }
void CPU::SRA_E() { SRA(E); }
void CPU::SRA_H() { SRA(H); }
void CPU::SRA_L() { SRA(L); }

void CPU::SRA_HL() {
	uint8_t value = bus->Read(HL);
	SRA(value);
	bus->Write(HL, value);

	cycles += 2;
}

void CPU::SRL(uint8_t& r) {
	(r & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r >> 1);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::SRL_A() { SRL(A); }
void CPU::SRL_B() { SRL(B); }
void CPU::SRL_C() { SRL(C); }
void CPU::SRL_D() { SRL(D); }
void CPU::SRL_E() { SRL(E); }
void CPU::SRL_H() { SRL(H); }
void CPU::SRL_L() { SRL(L); }

void CPU::SRL_HL() {
	uint8_t value = bus->Read(HL);
	SRL(value);
	bus->Write(HL, value);

	cycles += 2;
}


void CPU::SWAP(uint8_t& r) {
	r = (r << 4) | (r >> 4);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_CARRY | FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::SWAP_A() { SWAP(A); }
void CPU::SWAP_B() { SWAP(B); }
void CPU::SWAP_C() { SWAP(C); }
void CPU::SWAP_D() { SWAP(D); }
void CPU::SWAP_E() { SWAP(E); }
void CPU::SWAP_H() { SWAP(H); }
void CPU::SWAP_L() { SWAP(L); }

void CPU::SWAP_HL() { 
	uint8_t value = bus->Read(HL);
	SWAP(value);
	bus->Write(HL, value);
	
	cycles += 2;
}