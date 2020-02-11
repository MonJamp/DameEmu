#include "DameEmu.h"

DameEmu::Instruction DameEmu::cb_instructions[256] = {
	{"RLC B", 1, &DameEmu::RLC_B},					//00
	{"RLC C", 1, &DameEmu::RLC_C},					//01
	{"RLC D", 1, &DameEmu::RLC_D},					//02
	{"RLC E", 1, &DameEmu::RLC_E},					//03
	{"RLC H", 1, &DameEmu::RLC_H},					//04
	{"RLC L", 1, &DameEmu::RLC_L},					//05
	{"RLC (HL)", 1, &DameEmu::RLC_HL},				//06
	{"RLC A", 1, &DameEmu::RLC_A},					//07
	{"RRC B", 1, &DameEmu::RRC_B},					//08
	{"RRC C", 1, &DameEmu::RRC_C},					//09
	{"RRC D", 1, &DameEmu::RRC_D},					//0A
	{"RRC E", 1, &DameEmu::RRC_E},					//0B
	{"RRC H", 1, &DameEmu::RRC_H},					//0C
	{"RRC L", 1, &DameEmu::RRC_L},					//0D
	{"RRC (HL)", 1, &DameEmu::RRC_HL},				//0E
	{"RRC A", 1, &DameEmu::RRC_A},					//0F
	{"RL B", 1, &DameEmu::RL_B},					//10
	{"RL C", 1, &DameEmu::RL_C},					//11
	{"RL D", 1, &DameEmu::RL_D},					//12
	{"RL E", 1, &DameEmu::RL_E},					//13
	{"RL H", 1, &DameEmu::RL_H},					//14
	{"RL L", 1, &DameEmu::RL_L},					//15
	{"RL (HL)", 1, &DameEmu::RL_HL},				//16
	{"RL A", 1, &DameEmu::RL_A},					//17
	{"RR B", 1, &DameEmu::RR_B},					//18
	{"RR C", 1, &DameEmu::RR_C},					//19
	{"RR D", 1, &DameEmu::RR_D},					//1A
	{"RR E", 1, &DameEmu::RR_E},					//1B
	{"RR H", 1, &DameEmu::RR_H},					//1C
	{"RR L", 1, &DameEmu::RR_L},					//1D
	{"RR (HL)", 1, &DameEmu::RR_HL},				//1E
	{"RR A", 1, &DameEmu::RR_A},					//1F
	{"SLA B", 1, &DameEmu::UNIMPLEMENTED},			//20
	{"SLA C", 1, &DameEmu::UNIMPLEMENTED},			//21
	{"SLA D", 1, &DameEmu::UNIMPLEMENTED},			//22
	{"SLA E", 1, &DameEmu::UNIMPLEMENTED},			//23
	{"SLA H", 1, &DameEmu::UNIMPLEMENTED},			//24
	{"SLA L", 1, &DameEmu::UNIMPLEMENTED},			//25
	{"SLA (HL)", 1, &DameEmu::UNIMPLEMENTED},		//26
	{"SLA A", 1, &DameEmu::UNIMPLEMENTED},			//27
	{"SRA B", 1, &DameEmu::UNIMPLEMENTED},			//28
	{"SRA C", 1, &DameEmu::UNIMPLEMENTED},			//29
	{"SRA D", 1, &DameEmu::UNIMPLEMENTED},			//2A
	{"SRA E", 1, &DameEmu::UNIMPLEMENTED},			//2B
	{"SRA H", 1, &DameEmu::UNIMPLEMENTED},			//2C
	{"SRA L", 1, &DameEmu::UNIMPLEMENTED},			//2D
	{"SRA (HL)", 1, &DameEmu::UNIMPLEMENTED},		//2E
	{"SRA A", 1, &DameEmu::UNIMPLEMENTED},			//2F
	{"SWAP B", 1, &DameEmu::UNIMPLEMENTED},			//30
	{"SWAP C", 1, &DameEmu::UNIMPLEMENTED},			//31
	{"SWAP D", 1, &DameEmu::UNIMPLEMENTED},			//32
	{"SWAP E", 1, &DameEmu::UNIMPLEMENTED},			//33
	{"SWAP H", 1, &DameEmu::UNIMPLEMENTED},			//34
	{"SWAP L", 1, &DameEmu::UNIMPLEMENTED},			//35
	{"SWAP (HL)", 1, &DameEmu::UNIMPLEMENTED},		//36
	{"SWAP A", 1, &DameEmu::UNIMPLEMENTED},			//37
	{"SRL B", 1, &DameEmu::UNIMPLEMENTED},			//38
	{"SRL C", 1, &DameEmu::UNIMPLEMENTED},			//39
	{"SRL D", 1, &DameEmu::UNIMPLEMENTED},			//3A
	{"SRL E", 1, &DameEmu::UNIMPLEMENTED},			//3B
	{"SRL H", 1, &DameEmu::UNIMPLEMENTED},			//3C
	{"SRL L", 1, &DameEmu::UNIMPLEMENTED},			//3D
	{"SRL (HL)", 1, &DameEmu::UNIMPLEMENTED},		//3E
	{"SRL A", 1, &DameEmu::UNIMPLEMENTED},			//3F
	{"BIT 0, B", 1, &DameEmu::BIT_0_B},				//40
	{"BIT 0, C", 1, &DameEmu::BIT_0_C},				//41
	{"BIT 0, D", 1, &DameEmu::BIT_0_D},				//42
	{"BIT 0, E", 1, &DameEmu::BIT_0_E},				//43
	{"BIT 0, H", 1, &DameEmu::BIT_0_H},				//44
	{"BIT 0, L", 1, &DameEmu::BIT_0_L},				//45
	{"BIT 0, (HL)", 1, &DameEmu::BIT_0_HL},			//46
	{"BIT 0, A", 1, &DameEmu::BIT_0_A},				//47
	{"BIT 1, B", 1, &DameEmu::BIT_1_B},				//48
	{"BIT 1, C", 1, &DameEmu::BIT_1_C},				//49
	{"BIT 1, D", 1, &DameEmu::BIT_1_D},				//4A
	{"BIT 1, E", 1, &DameEmu::BIT_1_E},				//4B
	{"BIT 1, H", 1, &DameEmu::BIT_1_H},				//4C
	{"BIT 1, L", 1, &DameEmu::BIT_1_L},				//4D
	{"BIT 1, (HL)", 1, &DameEmu::BIT_1_HL},			//4E
	{"BIT 1, A", 1, &DameEmu::BIT_1_A},				//4F
	{"BIT 2, B", 1, &DameEmu::BIT_2_B},				//50
	{"BIT 2, C", 1, &DameEmu::BIT_2_C},				//51
	{"BIT 2, D", 1, &DameEmu::BIT_2_D},				//52
	{"BIT 2, E", 1, &DameEmu::BIT_2_E},				//53
	{"BIT 2, H", 1, &DameEmu::BIT_2_H},				//54
	{"BIT 2, L", 1, &DameEmu::BIT_2_L},				//55
	{"BIT 2, (HL)", 1, &DameEmu::BIT_2_HL},			//56
	{"BIT 2, A", 1, &DameEmu::BIT_2_A},				//57
	{"BIT 3, B", 1, &DameEmu::BIT_3_B},				//58
	{"BIT 3, C", 1, &DameEmu::BIT_3_C},				//59
	{"BIT 3, D", 1, &DameEmu::BIT_3_D},				//5A
	{"BIT 3, E", 1, &DameEmu::BIT_3_E},				//5B
	{"BIT 3, H", 1, &DameEmu::BIT_3_H},				//5C
	{"BIT 3, L", 1, &DameEmu::BIT_3_L},				//5D
	{"BIT 3, (HL)", 1, &DameEmu::BIT_3_HL},			//5E
	{"BIT 3, A", 1, &DameEmu::BIT_3_A},				//5F
	{"BIT 4, B", 1, &DameEmu::BIT_4_B},				//60
	{"BIT 4, C", 1, &DameEmu::BIT_4_C},				//61
	{"BIT 4, D", 1, &DameEmu::BIT_4_D},				//62
	{"BIT 4, E", 1, &DameEmu::BIT_4_E},				//63
	{"BIT 4, H", 1, &DameEmu::BIT_4_H},				//64
	{"BIT 4, L", 1, &DameEmu::BIT_4_L},				//65
	{"BIT 4, (HL)", 1, &DameEmu::BIT_4_HL},			//66
	{"BIT 4, A", 1, &DameEmu::BIT_4_A},				//67
	{"BIT 5, B", 1, &DameEmu::BIT_5_B},				//68
	{"BIT 5, C", 1, &DameEmu::BIT_5_C},				//69
	{"BIT 5, D", 1, &DameEmu::BIT_5_D},				//6A
	{"BIT 5, E", 1, &DameEmu::BIT_5_E},				//6B
	{"BIT 5, H", 1, &DameEmu::BIT_5_H},				//6C
	{"BIT 5, L", 1, &DameEmu::BIT_5_L},				//6D
	{"BIT 5, (HL)", 1, &DameEmu::BIT_5_HL},			//6E
	{"BIT 5, A", 1, &DameEmu::BIT_5_A},				//6F
	{"BIT 6, B", 1, &DameEmu::BIT_6_B},				//70
	{"BIT 6, C", 1, &DameEmu::BIT_6_C},				//71
	{"BIT 6, D", 1, &DameEmu::BIT_6_D},				//72
	{"BIT 6, E", 1, &DameEmu::BIT_6_E},				//73
	{"BIT 6, H", 1, &DameEmu::BIT_6_H},				//74
	{"BIT 6, L", 1, &DameEmu::BIT_6_L},				//75
	{"BIT 6, (HL)", 1, &DameEmu::BIT_6_HL},			//76
	{"BIT 6, A", 1, &DameEmu::BIT_6_A},				//77
	{"BIT 7, B", 1, &DameEmu::BIT_7_B},				//78
	{"BIT 7, C", 1, &DameEmu::BIT_7_C},				//79
	{"BIT 7, D", 1, &DameEmu::BIT_7_D},				//7A
	{"BIT 7, E", 1, &DameEmu::BIT_7_E},				//7B
	{"BIT 7, H", 1, &DameEmu::BIT_7_H},				//7C
	{"BIT 7, L", 1, &DameEmu::BIT_7_L},				//7D
	{"BIT 7, (HL)", 1, &DameEmu::BIT_7_HL},			//7E
	{"BIT 7, A", 1, &DameEmu::BIT_7_A},				//7F
	{"RES 0, B", 1, &DameEmu::RES_0_B},				//80
	{"RES 0, C", 1, &DameEmu::RES_0_C},				//81
	{"RES 0, D", 1, &DameEmu::RES_0_D},				//82
	{"RES 0, E", 1, &DameEmu::RES_0_E},				//83
	{"RES 0, H", 1, &DameEmu::RES_0_H},				//84
	{"RES 0, L", 1, &DameEmu::RES_0_L},				//85
	{"RES 0, (HL)", 1, &DameEmu::RES_0_HL},			//86
	{"RES 0, A", 1, &DameEmu::RES_0_A},				//87
	{"RES 1, B", 1, &DameEmu::RES_1_B},				//88
	{"RES 1, C", 1, &DameEmu::RES_1_C},				//89
	{"RES 1, D", 1, &DameEmu::RES_1_D},				//8A
	{"RES 1, E", 1, &DameEmu::RES_1_E},				//8B
	{"RES 1, H", 1, &DameEmu::RES_1_H},				//8C
	{"RES 1, L", 1, &DameEmu::RES_1_L},				//8D
	{"RES 1, (HL)", 1, &DameEmu::RES_1_HL},			//8E
	{"RES 1, A", 1, &DameEmu::RES_1_A},				//8F
	{"RES 2, B", 1, &DameEmu::RES_2_B},				//90
	{"RES 2, C", 1, &DameEmu::RES_2_C},				//91
	{"RES 2, D", 1, &DameEmu::RES_2_D},				//92
	{"RES 2, E", 1, &DameEmu::RES_2_E},				//93
	{"RES 2, H", 1, &DameEmu::RES_2_H},				//94
	{"RES 2, L", 1, &DameEmu::RES_2_L},				//95
	{"RES 2, (HL)", 1, &DameEmu::RES_2_HL},			//96
	{"RES 2, A", 1, &DameEmu::RES_2_A},				//97
	{"RES 3, B", 1, &DameEmu::RES_3_B},				//98
	{"RES 3, C", 1, &DameEmu::RES_3_C},				//99
	{"RES 3, D", 1, &DameEmu::RES_3_D},				//9A
	{"RES 3, E", 1, &DameEmu::RES_3_E},				//9B
	{"RES 3, H", 1, &DameEmu::RES_3_H},				//9C
	{"RES 3, L", 1, &DameEmu::RES_3_L},				//9D
	{"RES 3, (HL)", 1, &DameEmu::RES_3_HL},			//9E
	{"RES 3, A", 1, &DameEmu::RES_3_A},				//9F
	{"RES 4, B", 1, &DameEmu::RES_4_B},				//A0
	{"RES 4, C", 1, &DameEmu::RES_4_C},				//A1
	{"RES 4, D", 1, &DameEmu::RES_4_D},				//A2
	{"RES 4, E", 1, &DameEmu::RES_4_E},				//A3
	{"RES 4, H", 1, &DameEmu::RES_4_H},				//A4
	{"RES 4, L", 1, &DameEmu::RES_4_L},				//A5
	{"RES 4, (HL)", 1, &DameEmu::RES_4_HL},			//A6
	{"RES 4, A", 1, &DameEmu::RES_4_A},				//A7
	{"RES 5, B", 1, &DameEmu::RES_5_B},				//A8
	{"RES 5, C", 1, &DameEmu::RES_5_C},				//A9
	{"RES 5, D", 1, &DameEmu::RES_5_D},				//AA
	{"RES 5, E", 1, &DameEmu::RES_5_E},				//AB
	{"RES 5, H", 1, &DameEmu::RES_5_H},				//AC
	{"RES 5, L", 1, &DameEmu::RES_5_L},				//AD
	{"RES 5, (HL)", 1, &DameEmu::RES_5_HL},			//AE
	{"RES 5, A", 1, &DameEmu::RES_5_A},				//AF
	{"RES 6, B", 1, &DameEmu::RES_6_B},				//B0
	{"RES 6, C", 1, &DameEmu::RES_6_C},				//B1
	{"RES 6, D", 1, &DameEmu::RES_6_D},				//B2
	{"RES 6, E", 1, &DameEmu::RES_6_E},				//B3
	{"RES 6, H", 1, &DameEmu::RES_6_H},				//B4
	{"RES 6, L", 1, &DameEmu::RES_6_L},				//B5
	{"RES 6, (HL)", 1, &DameEmu::RES_6_HL},			//B6
	{"RES 6, A", 1, &DameEmu::RES_6_A},				//B7
	{"RES 7, B", 1, &DameEmu::RES_7_B},				//B8
	{"RES 7, C", 1, &DameEmu::RES_7_C},				//B9
	{"RES 7, D", 1, &DameEmu::RES_7_D},				//BA
	{"RES 7, E", 1, &DameEmu::RES_7_E},				//BB
	{"RES 7, H", 1, &DameEmu::RES_7_H},				//BC
	{"RES 7, L", 1, &DameEmu::RES_7_L},				//BD
	{"RES 7, (HL)", 1, &DameEmu::RES_7_HL},			//BE
	{"RES 7, A", 1, &DameEmu::RES_7_A},				//BF
	{"SET 0, B", 1, &DameEmu::SET_0_B},				//C0
	{"SET 0, C", 1, &DameEmu::SET_0_C},				//C1
	{"SET 0, D", 1, &DameEmu::SET_0_D},				//C2
	{"SET 0, E", 1, &DameEmu::SET_0_E},				//C3
	{"SET 0, H", 1, &DameEmu::SET_0_H},				//C4
	{"SET 0, L", 1, &DameEmu::SET_0_L},				//C5
	{"SET 0, (HL)", 1, &DameEmu::SET_0_HL},			//C6
	{"SET 0, A", 1, &DameEmu::SET_0_A},				//C7
	{"SET 1, B", 1, &DameEmu::SET_1_B},				//C8
	{"SET 1, C", 1, &DameEmu::SET_1_C},				//C9
	{"SET 1, D", 1, &DameEmu::SET_1_D},				//CA
	{"SET 1, E", 1, &DameEmu::SET_1_E},				//CB
	{"SET 1, H", 1, &DameEmu::SET_1_H},				//CC
	{"SET 1, L", 1, &DameEmu::SET_1_L},				//CD
	{"SET 1, (HL)", 1, &DameEmu::SET_1_HL},			//CE
	{"SET 1, A", 1, &DameEmu::SET_1_A},				//CF
	{"SET 2, B", 1, &DameEmu::SET_2_B},				//D0
	{"SET 2, C", 1, &DameEmu::SET_2_C},				//D1
	{"SET 2, D", 1, &DameEmu::SET_2_D},				//D2
	{"SET 2, E", 1, &DameEmu::SET_2_E},				//D3
	{"SET 2, H", 1, &DameEmu::SET_2_H},				//D4
	{"SET 2, L", 1, &DameEmu::SET_2_L},				//D5
	{"SET 2, (HL)", 1, &DameEmu::SET_2_HL},			//D6
	{"SET 2, A", 1, &DameEmu::SET_2_A},				//D7
	{"SET 3, B", 1, &DameEmu::SET_3_B},				//D8
	{"SET 3, C", 1, &DameEmu::SET_3_C},				//D9
	{"SET 3, D", 1, &DameEmu::SET_3_D},				//DA
	{"SET 3, E", 1, &DameEmu::SET_3_E},				//DB
	{"SET 3, H", 1, &DameEmu::SET_3_H},				//DC
	{"SET 3, L", 1, &DameEmu::SET_3_L},				//DD
	{"SET 3, (HL)", 1, &DameEmu::SET_3_HL},			//DE
	{"SET 3, A", 1, &DameEmu::SET_3_A},				//DF
	{"SET 4, B", 1, &DameEmu::SET_4_B},				//E0
	{"SET 4, C", 1, &DameEmu::SET_4_C},				//E1
	{"SET 4, D", 1, &DameEmu::SET_4_D},				//E2
	{"SET 4, E", 1, &DameEmu::SET_4_E},				//E3
	{"SET 4, H", 1, &DameEmu::SET_4_H},				//E4
	{"SET 4, L", 1, &DameEmu::SET_4_L},				//E5
	{"SET 4, (HL)", 1, &DameEmu::SET_4_HL},			//E6
	{"SET 4, A", 1, &DameEmu::SET_4_A},				//E7
	{"SET 5, B", 1, &DameEmu::SET_5_B},				//E8
	{"SET 5, C", 1, &DameEmu::SET_5_C},				//E9
	{"SET 5, D", 1, &DameEmu::SET_5_D},				//EA
	{"SET 5, E", 1, &DameEmu::SET_5_E},				//EB
	{"SET 5, H", 1, &DameEmu::SET_5_H},				//EC
	{"SET 5, L", 1, &DameEmu::SET_5_L},				//ED
	{"SET 5, (HL)", 1, &DameEmu::SET_5_HL},			//EE
	{"SET 5, A", 1, &DameEmu::SET_5_A},				//EF
	{"SET 6, B", 1, &DameEmu::SET_6_B},				//F0
	{"SET 6, C", 1, &DameEmu::SET_6_C},				//F1
	{"SET 6, D", 1, &DameEmu::SET_6_D},				//F2
	{"SET 6, E", 1, &DameEmu::SET_6_E},				//F3
	{"SET 6, H", 1, &DameEmu::SET_6_H},				//F4
	{"SET 6, L", 1, &DameEmu::SET_6_L},				//F5
	{"SET 6, (HL)", 1, &DameEmu::SET_6_HL},			//F6
	{"SET 6, A", 1, &DameEmu::SET_6_A},				//F7
	{"SET 7, B", 1, &DameEmu::SET_7_B},				//F8
	{"SET 7, C", 1, &DameEmu::SET_7_C},				//F9
	{"SET 7, D", 1, &DameEmu::SET_7_D},				//FA
	{"SET 7, E", 1, &DameEmu::SET_7_E},				//FB
	{"SET 7, H", 1, &DameEmu::SET_7_H},				//FC
	{"SET 7, L", 1, &DameEmu::SET_7_L},				//FD
	{"SET 7, (HL)", 1, &DameEmu::SET_7_HL},			//FE
	{"SET 7, A", 1, &DameEmu::SET_7_A},				//FF
};

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

void DameEmu::RLC(uint8_t& r) {
	(r & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r << 1) | FLAG_CHECK(FLAG_CARRY);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void DameEmu::RRC(uint8_t& r) {
	(r & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r >> 1) | (FLAG_CHECK(FLAG_CARRY) << 7);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void DameEmu::RL(uint8_t& r) {
	uint8_t carry = FLAG_CHECK(FLAG_CARRY);
	(r & (1 << 7)) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r << 1) | carry;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void DameEmu::RR(uint8_t& r) {
	uint8_t carry = FLAG_CHECK(FLAG_CARRY);
	(r & 1) ? FLAG_SET(FLAG_CARRY) : FLAG_CLEAR(FLAG_CARRY);
	r = (r >> 1) | (carry << 7);
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);

	FLAG_CLEAR(FLAG_HALFCARRY | FLAG_NEGATIVE);

	cycles += 2;
}

void DameEmu::SET(uint8_t b, uint8_t& r) {
	r |= (1 << b);

	cycles += 2;
}

void DameEmu::RES(uint8_t b, uint8_t& r) {
	F &= ~(1 << b);

	cycles += 2;
}

void DameEmu::RLC_B() { RLC(B); }
void DameEmu::RLC_C() { RLC(C); }
void DameEmu::RLC_D() { RLC(D); }
void DameEmu::RLC_E() { RLC(E); }
void DameEmu::RLC_H() { RLC(H); }
void DameEmu::RLC_L() { RLC(L); }
void DameEmu::RLC_HL() { RLC(memory[HL]); cycles += 2; }
void DameEmu::RLC_A() { RRC(A); }
void DameEmu::RRC_B() { RRC(B); }
void DameEmu::RRC_C() { RRC(C); }
void DameEmu::RRC_D() { RRC(D); }
void DameEmu::RRC_E() { RRC(E); }
void DameEmu::RRC_H() { RRC(H); }
void DameEmu::RRC_L() { RRC(L); }
void DameEmu::RRC_HL() { RRC(memory[HL]); cycles += 2; }
void DameEmu::RRC_A() { RRC(A); }
void DameEmu::RL_B() { RL(B); }
void DameEmu::RL_C() { RL(C); }
void DameEmu::RL_D() { RL(D); }
void DameEmu::RL_E() { RL(E); }
void DameEmu::RL_H() { RL(H); }
void DameEmu::RL_L() { RL(L); }
void DameEmu::RL_HL() { RL(memory[HL]); cycles += 2; }
void DameEmu::RL_A() { RL(A); }
void DameEmu::RR_B() { RR(B); }
void DameEmu::RR_C() { RR(C); }
void DameEmu::RR_D() { RR(D); }
void DameEmu::RR_E() { RR(E); }
void DameEmu::RR_H() { RR(H); }
void DameEmu::RR_L() { RR(L); }
void DameEmu::RR_HL() { RR(memory[HL]); cycles += 2; }
void DameEmu::RR_A() { RR(A); }

void DameEmu::BIT_0_B() { BIT(0, B); }
void DameEmu::BIT_0_C() { BIT(0, C); }
void DameEmu::BIT_0_D() { BIT(0, D); }
void DameEmu::BIT_0_E() { BIT(0, E); }
void DameEmu::BIT_0_H() { BIT(0, H); }
void DameEmu::BIT_0_L() { BIT(0, L); }
void DameEmu::BIT_0_HL() { BIT_HL(0); }
void DameEmu::BIT_0_A() { BIT(0, A); }
void DameEmu::BIT_1_B() { BIT(1, B); }
void DameEmu::BIT_1_C() { BIT(1, C); }
void DameEmu::BIT_1_D() { BIT(1, D); }
void DameEmu::BIT_1_E() { BIT(1, E); }
void DameEmu::BIT_1_H() { BIT(1, H); }
void DameEmu::BIT_1_L() { BIT(1, L); }
void DameEmu::BIT_1_HL() { BIT_HL(1); }
void DameEmu::BIT_1_A() { BIT(1, A); }
void DameEmu::BIT_2_B() { BIT(2, B); }
void DameEmu::BIT_2_C() { BIT(2, C); }
void DameEmu::BIT_2_D() { BIT(2, D); }
void DameEmu::BIT_2_E() { BIT(2, E); }
void DameEmu::BIT_2_H() { BIT(2, H); }
void DameEmu::BIT_2_L() { BIT(2, L); }
void DameEmu::BIT_2_HL() { BIT_HL(2); }
void DameEmu::BIT_2_A() { BIT(2, A); }
void DameEmu::BIT_3_B() { BIT(3, B); }
void DameEmu::BIT_3_C() { BIT(3, C); }
void DameEmu::BIT_3_D() { BIT(3, D); }
void DameEmu::BIT_3_E() { BIT(3, E); }
void DameEmu::BIT_3_H() { BIT(3, H); }
void DameEmu::BIT_3_L() { BIT(3, L); }
void DameEmu::BIT_3_HL() { BIT_HL(3); }
void DameEmu::BIT_3_A() { BIT(3, A); }
void DameEmu::BIT_4_B() { BIT(4, B); }
void DameEmu::BIT_4_C() { BIT(4, C); }
void DameEmu::BIT_4_D() { BIT(4, D); }
void DameEmu::BIT_4_E() { BIT(4, E); }
void DameEmu::BIT_4_H() { BIT(4, H); }
void DameEmu::BIT_4_L() { BIT(4, L); }
void DameEmu::BIT_4_HL() { BIT_HL(4); }
void DameEmu::BIT_4_A() { BIT(4, A); }
void DameEmu::BIT_5_B() { BIT(5, B); }
void DameEmu::BIT_5_C() { BIT(5, C); }
void DameEmu::BIT_5_D() { BIT(5, D); }
void DameEmu::BIT_5_E() { BIT(5, E); }
void DameEmu::BIT_5_H() { BIT(5, H); }
void DameEmu::BIT_5_L() { BIT(5, L); }
void DameEmu::BIT_5_HL() { BIT_HL(5); }
void DameEmu::BIT_5_A() { BIT(5, A); }
void DameEmu::BIT_6_B() { BIT(6, B); }
void DameEmu::BIT_6_C() { BIT(6, C); }
void DameEmu::BIT_6_D() { BIT(6, D); }
void DameEmu::BIT_6_E() { BIT(6, E); }
void DameEmu::BIT_6_H() { BIT(6, H); }
void DameEmu::BIT_6_L() { BIT(6, L); }
void DameEmu::BIT_6_HL() { BIT_HL(6); }
void DameEmu::BIT_6_A() { BIT(6, A); }
void DameEmu::BIT_7_B() { BIT(7, B); }
void DameEmu::BIT_7_C() { BIT(7, C); }
void DameEmu::BIT_7_D() { BIT(7, D); }
void DameEmu::BIT_7_E() { BIT(7, E); }
void DameEmu::BIT_7_H() { BIT(7, H); }
void DameEmu::BIT_7_L() { BIT(7, L); }
void DameEmu::BIT_7_HL() { BIT_HL(7); }
void DameEmu::BIT_7_A() { BIT(7, A); }

void DameEmu::SET_0_A() { SET(0, A); }
void DameEmu::SET_0_B() { SET(0, B); }
void DameEmu::SET_0_C() { SET(0, C); }
void DameEmu::SET_0_D() { SET(0, D); }
void DameEmu::SET_0_E() { SET(0, E); }
void DameEmu::SET_0_H() { SET(0, H); }
void DameEmu::SET_0_L() { SET(0, L); }
void DameEmu::SET_0_HL() { SET(0, memory[HL]); cycles += 2; }
void DameEmu::SET_1_A() { SET(1, A); }
void DameEmu::SET_1_B() { SET(1, B); }
void DameEmu::SET_1_C() { SET(1, C); }
void DameEmu::SET_1_D() { SET(1, D); }
void DameEmu::SET_1_E() { SET(1, E); }
void DameEmu::SET_1_H() { SET(1, H); }
void DameEmu::SET_1_L() { SET(1, L); }
void DameEmu::SET_1_HL() { SET(1, memory[HL]); cycles += 2; }
void DameEmu::SET_2_A() { SET(2, A); }
void DameEmu::SET_2_B() { SET(2, B); }
void DameEmu::SET_2_C() { SET(2, C); }
void DameEmu::SET_2_D() { SET(2, D); }
void DameEmu::SET_2_E() { SET(2, E); }
void DameEmu::SET_2_H() { SET(2, H); }
void DameEmu::SET_2_L() { SET(2, L); }
void DameEmu::SET_2_HL() { SET(2, memory[HL]); cycles += 2; }
void DameEmu::SET_3_A() { SET(3, A); }
void DameEmu::SET_3_B() { SET(3, B); }
void DameEmu::SET_3_C() { SET(3, C); }
void DameEmu::SET_3_D() { SET(3, D); }
void DameEmu::SET_3_E() { SET(3, E); }
void DameEmu::SET_3_H() { SET(3, H); }
void DameEmu::SET_3_L() { SET(3, L); }
void DameEmu::SET_3_HL() { SET(3, memory[HL]); cycles += 2; }
void DameEmu::SET_4_A() { SET(4, A); }
void DameEmu::SET_4_B() { SET(4, B); }
void DameEmu::SET_4_C() { SET(4, C); }
void DameEmu::SET_4_D() { SET(4, D); }
void DameEmu::SET_4_E() { SET(4, E); }
void DameEmu::SET_4_H() { SET(4, H); }
void DameEmu::SET_4_L() { SET(4, L); }
void DameEmu::SET_4_HL() { SET(4, memory[HL]); cycles += 2; }
void DameEmu::SET_5_A() { SET(5, A); }
void DameEmu::SET_5_B() { SET(5, B); }
void DameEmu::SET_5_C() { SET(5, C); }
void DameEmu::SET_5_D() { SET(5, D); }
void DameEmu::SET_5_E() { SET(5, E); }
void DameEmu::SET_5_H() { SET(5, H); }
void DameEmu::SET_5_L() { SET(5, L); }
void DameEmu::SET_5_HL() { SET(5, memory[HL]); cycles += 2; }
void DameEmu::SET_6_A() { SET(6, A); }
void DameEmu::SET_6_B() { SET(6, B); }
void DameEmu::SET_6_C() { SET(6, C); }
void DameEmu::SET_6_D() { SET(6, D); }
void DameEmu::SET_6_E() { SET(6, E); }
void DameEmu::SET_6_H() { SET(6, H); }
void DameEmu::SET_6_L() { SET(6, L); }
void DameEmu::SET_6_HL() { SET(6, memory[HL]); cycles += 2; }
void DameEmu::SET_7_A() { SET(7, A); }
void DameEmu::SET_7_B() { SET(7, B); }
void DameEmu::SET_7_C() { SET(7, C); }
void DameEmu::SET_7_D() { SET(7, D); }
void DameEmu::SET_7_E() { SET(7, E); }
void DameEmu::SET_7_H() { SET(7, H); }
void DameEmu::SET_7_L() { SET(7, L); }
void DameEmu::SET_7_HL() { SET(7, memory[HL]); cycles += 2; }

void DameEmu::RES_0_A() { RES(0, A); }
void DameEmu::RES_0_B() { RES(0, B); }
void DameEmu::RES_0_C() { RES(0, C); }
void DameEmu::RES_0_D() { RES(0, D); }
void DameEmu::RES_0_E() { RES(0, E); }
void DameEmu::RES_0_H() { RES(0, H); }
void DameEmu::RES_0_L() { RES(0, L); }
void DameEmu::RES_0_HL() { RES(0, memory[HL]); cycles += 2; }
void DameEmu::RES_1_A() { RES(1, A); }
void DameEmu::RES_1_B() { RES(1, B); }
void DameEmu::RES_1_C() { RES(1, C); }
void DameEmu::RES_1_D() { RES(1, D); }
void DameEmu::RES_1_E() { RES(1, E); }
void DameEmu::RES_1_H() { RES(1, H); }
void DameEmu::RES_1_L() { RES(1, L); }
void DameEmu::RES_1_HL() { RES(1, memory[HL]); cycles += 2; }
void DameEmu::RES_2_A() { RES(2, A); }
void DameEmu::RES_2_B() { RES(2, B); }
void DameEmu::RES_2_C() { RES(2, C); }
void DameEmu::RES_2_D() { RES(2, D); }
void DameEmu::RES_2_E() { RES(2, E); }
void DameEmu::RES_2_H() { RES(2, H); }
void DameEmu::RES_2_L() { RES(2, L); }
void DameEmu::RES_2_HL() { RES(2, memory[HL]); cycles += 2; }
void DameEmu::RES_3_A() { RES(3, A); }
void DameEmu::RES_3_B() { RES(3, B); }
void DameEmu::RES_3_C() { RES(3, C); }
void DameEmu::RES_3_D() { RES(3, D); }
void DameEmu::RES_3_E() { RES(3, E); }
void DameEmu::RES_3_H() { RES(3, H); }
void DameEmu::RES_3_L() { RES(3, L); }
void DameEmu::RES_3_HL() { RES(3, memory[HL]); cycles += 2; }
void DameEmu::RES_4_A() { RES(4, A); }
void DameEmu::RES_4_B() { RES(4, B); }
void DameEmu::RES_4_C() { RES(4, C); }
void DameEmu::RES_4_D() { RES(4, D); }
void DameEmu::RES_4_E() { RES(4, E); }
void DameEmu::RES_4_H() { RES(4, H); }
void DameEmu::RES_4_L() { RES(4, L); }
void DameEmu::RES_4_HL() { RES(4, memory[HL]); cycles += 2; }
void DameEmu::RES_5_A() { RES(5, A); }
void DameEmu::RES_5_B() { RES(5, B); }
void DameEmu::RES_5_C() { RES(5, C); }
void DameEmu::RES_5_D() { RES(5, D); }
void DameEmu::RES_5_E() { RES(5, E); }
void DameEmu::RES_5_H() { RES(5, H); }
void DameEmu::RES_5_L() { RES(5, L); }
void DameEmu::RES_5_HL() { RES(5, memory[HL]); cycles += 2; }
void DameEmu::RES_6_A() { RES(6, A); }
void DameEmu::RES_6_B() { RES(6, B); }
void DameEmu::RES_6_C() { RES(6, C); }
void DameEmu::RES_6_D() { RES(6, D); }
void DameEmu::RES_6_E() { RES(6, E); }
void DameEmu::RES_6_H() { RES(6, H); }
void DameEmu::RES_6_L() { RES(6, L); }
void DameEmu::RES_6_HL() { RES(6, memory[HL]); cycles += 2; }
void DameEmu::RES_7_A() { RES(7, A); }
void DameEmu::RES_7_B() { RES(7, B); }
void DameEmu::RES_7_C() { RES(7, C); }
void DameEmu::RES_7_D() { RES(7, D); }
void DameEmu::RES_7_E() { RES(7, E); }
void DameEmu::RES_7_H() { RES(7, H); }
void DameEmu::RES_7_L() { RES(7, L); }
void DameEmu::RES_7_HL() { RES(7, memory[HL]); cycles += 2; }