#include "CPU.h"

CPU::Instruction CPU::cb_instructions[256] = {
	{"RLC B", 1, &CPU::RLC_B},					//00
	{"RLC C", 1, &CPU::RLC_C},					//01
	{"RLC D", 1, &CPU::RLC_D},					//02
	{"RLC E", 1, &CPU::RLC_E},					//03
	{"RLC H", 1, &CPU::RLC_H},					//04
	{"RLC L", 1, &CPU::RLC_L},					//05
	{"RLC (HL)", 1, &CPU::RLC_HL},				//06
	{"RLC A", 1, &CPU::RLC_A},					//07
	{"RRC B", 1, &CPU::RRC_B},					//08
	{"RRC C", 1, &CPU::RRC_C},					//09
	{"RRC D", 1, &CPU::RRC_D},					//0A
	{"RRC E", 1, &CPU::RRC_E},					//0B
	{"RRC H", 1, &CPU::RRC_H},					//0C
	{"RRC L", 1, &CPU::RRC_L},					//0D
	{"RRC (HL)", 1, &CPU::RRC_HL},				//0E
	{"RRC A", 1, &CPU::RRC_A},					//0F
	{"RL B", 1, &CPU::RL_B},					//10
	{"RL C", 1, &CPU::RL_C},					//11
	{"RL D", 1, &CPU::RL_D},					//12
	{"RL E", 1, &CPU::RL_E},					//13
	{"RL H", 1, &CPU::RL_H},					//14
	{"RL L", 1, &CPU::RL_L},					//15
	{"RL (HL)", 1, &CPU::RL_HL},				//16
	{"RL A", 1, &CPU::RL_A},					//17
	{"RR B", 1, &CPU::RR_B},					//18
	{"RR C", 1, &CPU::RR_C},					//19
	{"RR D", 1, &CPU::RR_D},					//1A
	{"RR E", 1, &CPU::RR_E},					//1B
	{"RR H", 1, &CPU::RR_H},					//1C
	{"RR L", 1, &CPU::RR_L},					//1D
	{"RR (HL)", 1, &CPU::RR_HL},				//1E
	{"RR A", 1, &CPU::RR_A},					//1F
	{"SLA B", 1, &CPU::SLA_B},					//20
	{"SLA C", 1, &CPU::SLA_C},					//21
	{"SLA D", 1, &CPU::SLA_D},					//22
	{"SLA E", 1, &CPU::SLA_E},					//23
	{"SLA H", 1, &CPU::SLA_H},					//24
	{"SLA L", 1, &CPU::SLA_L},					//25
	{"SLA (HL)", 1, &CPU::SLA_HL},				//26
	{"SLA A", 1, &CPU::SRA_A},					//27
	{"SRA B", 1, &CPU::SRA_B},					//28
	{"SRA C", 1, &CPU::SRA_C},					//29
	{"SRA D", 1, &CPU::SRA_D},					//2A
	{"SRA E", 1, &CPU::SRA_E},					//2B
	{"SRA H", 1, &CPU::SRA_H},					//2C
	{"SRA L", 1, &CPU::SRA_L},					//2D
	{"SRA (HL)", 1, &CPU::SRA_HL},				//2E
	{"SRA A", 1, &CPU::SRA_A},					//2F
	{"SWAP B", 1, &CPU::SWAP_B},				//30
	{"SWAP C", 1, &CPU::SWAP_C},				//31
	{"SWAP D", 1, &CPU::SWAP_D},				//32
	{"SWAP E", 1, &CPU::SWAP_E},				//33
	{"SWAP H", 1, &CPU::SWAP_H},				//34
	{"SWAP L", 1, &CPU::SWAP_L},				//35
	{"SWAP (HL)", 1, &CPU::SWAP_HL},			//36
	{"SWAP A", 1, &CPU::SWAP_A},				//37
	{"SRL B", 1, &CPU::SRL_B},					//38
	{"SRL C", 1, &CPU::SRL_C},					//39
	{"SRL D", 1, &CPU::SRL_D},					//3A
	{"SRL E", 1, &CPU::SRL_E},					//3B
	{"SRL H", 1, &CPU::SRL_H},					//3C
	{"SRL L", 1, &CPU::SRL_L},					//3D
	{"SRL (HL)", 1, &CPU::SRL_HL},				//3E
	{"SRL A", 1, &CPU::SRL_A},					//3F
	{"BIT 0, B", 1, &CPU::BIT_0_B},				//40
	{"BIT 0, C", 1, &CPU::BIT_0_C},				//41
	{"BIT 0, D", 1, &CPU::BIT_0_D},				//42
	{"BIT 0, E", 1, &CPU::BIT_0_E},				//43
	{"BIT 0, H", 1, &CPU::BIT_0_H},				//44
	{"BIT 0, L", 1, &CPU::BIT_0_L},				//45
	{"BIT 0, (HL)", 1, &CPU::BIT_0_HL},			//46
	{"BIT 0, A", 1, &CPU::BIT_0_A},				//47
	{"BIT 1, B", 1, &CPU::BIT_1_B},				//48
	{"BIT 1, C", 1, &CPU::BIT_1_C},				//49
	{"BIT 1, D", 1, &CPU::BIT_1_D},				//4A
	{"BIT 1, E", 1, &CPU::BIT_1_E},				//4B
	{"BIT 1, H", 1, &CPU::BIT_1_H},				//4C
	{"BIT 1, L", 1, &CPU::BIT_1_L},				//4D
	{"BIT 1, (HL)", 1, &CPU::BIT_1_HL},			//4E
	{"BIT 1, A", 1, &CPU::BIT_1_A},				//4F
	{"BIT 2, B", 1, &CPU::BIT_2_B},				//50
	{"BIT 2, C", 1, &CPU::BIT_2_C},				//51
	{"BIT 2, D", 1, &CPU::BIT_2_D},				//52
	{"BIT 2, E", 1, &CPU::BIT_2_E},				//53
	{"BIT 2, H", 1, &CPU::BIT_2_H},				//54
	{"BIT 2, L", 1, &CPU::BIT_2_L},				//55
	{"BIT 2, (HL)", 1, &CPU::BIT_2_HL},			//56
	{"BIT 2, A", 1, &CPU::BIT_2_A},				//57
	{"BIT 3, B", 1, &CPU::BIT_3_B},				//58
	{"BIT 3, C", 1, &CPU::BIT_3_C},				//59
	{"BIT 3, D", 1, &CPU::BIT_3_D},				//5A
	{"BIT 3, E", 1, &CPU::BIT_3_E},				//5B
	{"BIT 3, H", 1, &CPU::BIT_3_H},				//5C
	{"BIT 3, L", 1, &CPU::BIT_3_L},				//5D
	{"BIT 3, (HL)", 1, &CPU::BIT_3_HL},			//5E
	{"BIT 3, A", 1, &CPU::BIT_3_A},				//5F
	{"BIT 4, B", 1, &CPU::BIT_4_B},				//60
	{"BIT 4, C", 1, &CPU::BIT_4_C},				//61
	{"BIT 4, D", 1, &CPU::BIT_4_D},				//62
	{"BIT 4, E", 1, &CPU::BIT_4_E},				//63
	{"BIT 4, H", 1, &CPU::BIT_4_H},				//64
	{"BIT 4, L", 1, &CPU::BIT_4_L},				//65
	{"BIT 4, (HL)", 1, &CPU::BIT_4_HL},			//66
	{"BIT 4, A", 1, &CPU::BIT_4_A},				//67
	{"BIT 5, B", 1, &CPU::BIT_5_B},				//68
	{"BIT 5, C", 1, &CPU::BIT_5_C},				//69
	{"BIT 5, D", 1, &CPU::BIT_5_D},				//6A
	{"BIT 5, E", 1, &CPU::BIT_5_E},				//6B
	{"BIT 5, H", 1, &CPU::BIT_5_H},				//6C
	{"BIT 5, L", 1, &CPU::BIT_5_L},				//6D
	{"BIT 5, (HL)", 1, &CPU::BIT_5_HL},			//6E
	{"BIT 5, A", 1, &CPU::BIT_5_A},				//6F
	{"BIT 6, B", 1, &CPU::BIT_6_B},				//70
	{"BIT 6, C", 1, &CPU::BIT_6_C},				//71
	{"BIT 6, D", 1, &CPU::BIT_6_D},				//72
	{"BIT 6, E", 1, &CPU::BIT_6_E},				//73
	{"BIT 6, H", 1, &CPU::BIT_6_H},				//74
	{"BIT 6, L", 1, &CPU::BIT_6_L},				//75
	{"BIT 6, (HL)", 1, &CPU::BIT_6_HL},			//76
	{"BIT 6, A", 1, &CPU::BIT_6_A},				//77
	{"BIT 7, B", 1, &CPU::BIT_7_B},				//78
	{"BIT 7, C", 1, &CPU::BIT_7_C},				//79
	{"BIT 7, D", 1, &CPU::BIT_7_D},				//7A
	{"BIT 7, E", 1, &CPU::BIT_7_E},				//7B
	{"BIT 7, H", 1, &CPU::BIT_7_H},				//7C
	{"BIT 7, L", 1, &CPU::BIT_7_L},				//7D
	{"BIT 7, (HL)", 1, &CPU::BIT_7_HL},			//7E
	{"BIT 7, A", 1, &CPU::BIT_7_A},				//7F
	{"RES 0, B", 1, &CPU::RES_0_B},				//80
	{"RES 0, C", 1, &CPU::RES_0_C},				//81
	{"RES 0, D", 1, &CPU::RES_0_D},				//82
	{"RES 0, E", 1, &CPU::RES_0_E},				//83
	{"RES 0, H", 1, &CPU::RES_0_H},				//84
	{"RES 0, L", 1, &CPU::RES_0_L},				//85
	{"RES 0, (HL)", 1, &CPU::RES_0_HL},			//86
	{"RES 0, A", 1, &CPU::RES_0_A},				//87
	{"RES 1, B", 1, &CPU::RES_1_B},				//88
	{"RES 1, C", 1, &CPU::RES_1_C},				//89
	{"RES 1, D", 1, &CPU::RES_1_D},				//8A
	{"RES 1, E", 1, &CPU::RES_1_E},				//8B
	{"RES 1, H", 1, &CPU::RES_1_H},				//8C
	{"RES 1, L", 1, &CPU::RES_1_L},				//8D
	{"RES 1, (HL)", 1, &CPU::RES_1_HL},			//8E
	{"RES 1, A", 1, &CPU::RES_1_A},				//8F
	{"RES 2, B", 1, &CPU::RES_2_B},				//90
	{"RES 2, C", 1, &CPU::RES_2_C},				//91
	{"RES 2, D", 1, &CPU::RES_2_D},				//92
	{"RES 2, E", 1, &CPU::RES_2_E},				//93
	{"RES 2, H", 1, &CPU::RES_2_H},				//94
	{"RES 2, L", 1, &CPU::RES_2_L},				//95
	{"RES 2, (HL)", 1, &CPU::RES_2_HL},			//96
	{"RES 2, A", 1, &CPU::RES_2_A},				//97
	{"RES 3, B", 1, &CPU::RES_3_B},				//98
	{"RES 3, C", 1, &CPU::RES_3_C},				//99
	{"RES 3, D", 1, &CPU::RES_3_D},				//9A
	{"RES 3, E", 1, &CPU::RES_3_E},				//9B
	{"RES 3, H", 1, &CPU::RES_3_H},				//9C
	{"RES 3, L", 1, &CPU::RES_3_L},				//9D
	{"RES 3, (HL)", 1, &CPU::RES_3_HL},			//9E
	{"RES 3, A", 1, &CPU::RES_3_A},				//9F
	{"RES 4, B", 1, &CPU::RES_4_B},				//A0
	{"RES 4, C", 1, &CPU::RES_4_C},				//A1
	{"RES 4, D", 1, &CPU::RES_4_D},				//A2
	{"RES 4, E", 1, &CPU::RES_4_E},				//A3
	{"RES 4, H", 1, &CPU::RES_4_H},				//A4
	{"RES 4, L", 1, &CPU::RES_4_L},				//A5
	{"RES 4, (HL)", 1, &CPU::RES_4_HL},			//A6
	{"RES 4, A", 1, &CPU::RES_4_A},				//A7
	{"RES 5, B", 1, &CPU::RES_5_B},				//A8
	{"RES 5, C", 1, &CPU::RES_5_C},				//A9
	{"RES 5, D", 1, &CPU::RES_5_D},				//AA
	{"RES 5, E", 1, &CPU::RES_5_E},				//AB
	{"RES 5, H", 1, &CPU::RES_5_H},				//AC
	{"RES 5, L", 1, &CPU::RES_5_L},				//AD
	{"RES 5, (HL)", 1, &CPU::RES_5_HL},			//AE
	{"RES 5, A", 1, &CPU::RES_5_A},				//AF
	{"RES 6, B", 1, &CPU::RES_6_B},				//B0
	{"RES 6, C", 1, &CPU::RES_6_C},				//B1
	{"RES 6, D", 1, &CPU::RES_6_D},				//B2
	{"RES 6, E", 1, &CPU::RES_6_E},				//B3
	{"RES 6, H", 1, &CPU::RES_6_H},				//B4
	{"RES 6, L", 1, &CPU::RES_6_L},				//B5
	{"RES 6, (HL)", 1, &CPU::RES_6_HL},			//B6
	{"RES 6, A", 1, &CPU::RES_6_A},				//B7
	{"RES 7, B", 1, &CPU::RES_7_B},				//B8
	{"RES 7, C", 1, &CPU::RES_7_C},				//B9
	{"RES 7, D", 1, &CPU::RES_7_D},				//BA
	{"RES 7, E", 1, &CPU::RES_7_E},				//BB
	{"RES 7, H", 1, &CPU::RES_7_H},				//BC
	{"RES 7, L", 1, &CPU::RES_7_L},				//BD
	{"RES 7, (HL)", 1, &CPU::RES_7_HL},			//BE
	{"RES 7, A", 1, &CPU::RES_7_A},				//BF
	{"SET 0, B", 1, &CPU::SET_0_B},				//C0
	{"SET 0, C", 1, &CPU::SET_0_C},				//C1
	{"SET 0, D", 1, &CPU::SET_0_D},				//C2
	{"SET 0, E", 1, &CPU::SET_0_E},				//C3
	{"SET 0, H", 1, &CPU::SET_0_H},				//C4
	{"SET 0, L", 1, &CPU::SET_0_L},				//C5
	{"SET 0, (HL)", 1, &CPU::SET_0_HL},			//C6
	{"SET 0, A", 1, &CPU::SET_0_A},				//C7
	{"SET 1, B", 1, &CPU::SET_1_B},				//C8
	{"SET 1, C", 1, &CPU::SET_1_C},				//C9
	{"SET 1, D", 1, &CPU::SET_1_D},				//CA
	{"SET 1, E", 1, &CPU::SET_1_E},				//CB
	{"SET 1, H", 1, &CPU::SET_1_H},				//CC
	{"SET 1, L", 1, &CPU::SET_1_L},				//CD
	{"SET 1, (HL)", 1, &CPU::SET_1_HL},			//CE
	{"SET 1, A", 1, &CPU::SET_1_A},				//CF
	{"SET 2, B", 1, &CPU::SET_2_B},				//D0
	{"SET 2, C", 1, &CPU::SET_2_C},				//D1
	{"SET 2, D", 1, &CPU::SET_2_D},				//D2
	{"SET 2, E", 1, &CPU::SET_2_E},				//D3
	{"SET 2, H", 1, &CPU::SET_2_H},				//D4
	{"SET 2, L", 1, &CPU::SET_2_L},				//D5
	{"SET 2, (HL)", 1, &CPU::SET_2_HL},			//D6
	{"SET 2, A", 1, &CPU::SET_2_A},				//D7
	{"SET 3, B", 1, &CPU::SET_3_B},				//D8
	{"SET 3, C", 1, &CPU::SET_3_C},				//D9
	{"SET 3, D", 1, &CPU::SET_3_D},				//DA
	{"SET 3, E", 1, &CPU::SET_3_E},				//DB
	{"SET 3, H", 1, &CPU::SET_3_H},				//DC
	{"SET 3, L", 1, &CPU::SET_3_L},				//DD
	{"SET 3, (HL)", 1, &CPU::SET_3_HL},			//DE
	{"SET 3, A", 1, &CPU::SET_3_A},				//DF
	{"SET 4, B", 1, &CPU::SET_4_B},				//E0
	{"SET 4, C", 1, &CPU::SET_4_C},				//E1
	{"SET 4, D", 1, &CPU::SET_4_D},				//E2
	{"SET 4, E", 1, &CPU::SET_4_E},				//E3
	{"SET 4, H", 1, &CPU::SET_4_H},				//E4
	{"SET 4, L", 1, &CPU::SET_4_L},				//E5
	{"SET 4, (HL)", 1, &CPU::SET_4_HL},			//E6
	{"SET 4, A", 1, &CPU::SET_4_A},				//E7
	{"SET 5, B", 1, &CPU::SET_5_B},				//E8
	{"SET 5, C", 1, &CPU::SET_5_C},				//E9
	{"SET 5, D", 1, &CPU::SET_5_D},				//EA
	{"SET 5, E", 1, &CPU::SET_5_E},				//EB
	{"SET 5, H", 1, &CPU::SET_5_H},				//EC
	{"SET 5, L", 1, &CPU::SET_5_L},				//ED
	{"SET 5, (HL)", 1, &CPU::SET_5_HL},			//EE
	{"SET 5, A", 1, &CPU::SET_5_A},				//EF
	{"SET 6, B", 1, &CPU::SET_6_B},				//F0
	{"SET 6, C", 1, &CPU::SET_6_C},				//F1
	{"SET 6, D", 1, &CPU::SET_6_D},				//F2
	{"SET 6, E", 1, &CPU::SET_6_E},				//F3
	{"SET 6, H", 1, &CPU::SET_6_H},				//F4
	{"SET 6, L", 1, &CPU::SET_6_L},				//F5
	{"SET 6, (HL)", 1, &CPU::SET_6_HL},			//F6
	{"SET 6, A", 1, &CPU::SET_6_A},				//F7
	{"SET 7, B", 1, &CPU::SET_7_B},				//F8
	{"SET 7, C", 1, &CPU::SET_7_C},				//F9
	{"SET 7, D", 1, &CPU::SET_7_D},				//FA
	{"SET 7, E", 1, &CPU::SET_7_E},				//FB
	{"SET 7, H", 1, &CPU::SET_7_H},				//FC
	{"SET 7, L", 1, &CPU::SET_7_L},				//FD
	{"SET 7, (HL)", 1, &CPU::SET_7_HL},			//FE
	{"SET 7, A", 1, &CPU::SET_7_A},				//FF
};



void CPU::RLC(uint8_t& r) {
	(r & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r << 1) | FLAG_CHECK(FLAG_CARRY);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::RLC_HL() {
	uint8_t value = mmu.Read(HL);
	RLC(value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	RRC(value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	RL(value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	RR(value);
	mmu.Write(HL, value);

	cycles += 2;
}

void CPU::RR_A() { RR(A); }
void CPU::RR_B() { RR(B); }
void CPU::RR_C() { RR(C); }
void CPU::RR_D() { RR(D); }
void CPU::RR_E() { RR(E); }
void CPU::RR_H() { RR(H); }
void CPU::RR_L() { RR(L); }

void CPU::BIT(uint8_t b, uint8_t& r) {
	(r & (1 << b)) ? FLAG_CLEAR(FLAG_ZERO) : FLAG_SET(FLAG_ZERO);

	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 2;
}

void CPU::BIT_HL(uint8_t b) {
	uint8_t value = mmu.Read(HL);
	BIT(b, value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	SET(b, value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	RES(b, value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	SLA(value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	SRA(value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	SRL(value);
	mmu.Write(HL, value);

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
	uint8_t value = mmu.Read(HL);
	SWAP(value);
	mmu.Write(HL, value);
	
	cycles += 2;
}