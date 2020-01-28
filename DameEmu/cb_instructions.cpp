#include "DameEmu.h"

DameEmu::Instruction DameEmu::cb_instructions[256] = {
	{"RLC B", 1, &UNIMPLEMENTED},			//00
	{"RLC C", 1, &UNIMPLEMENTED},			//01
	{"RLC D", 1, &UNIMPLEMENTED},			//02
	{"RLC E", 1, &UNIMPLEMENTED},			//03
	{"RLC H", 1, &UNIMPLEMENTED},			//04
	{"RLC L", 1, &UNIMPLEMENTED},			//05
	{"RLC (HL)", 1, &UNIMPLEMENTED},		//06
	{"RLC A", 1, &UNIMPLEMENTED},			//07
	{"RRC B", 1, &UNIMPLEMENTED},			//08
	{"RRC C", 1, &UNIMPLEMENTED},			//09
	{"RRC D", 1, &UNIMPLEMENTED},			//0A
	{"RRC E", 1, &UNIMPLEMENTED},			//0B
	{"RRC H", 1, &UNIMPLEMENTED},			//0C
	{"RRC L", 1, &UNIMPLEMENTED},			//0D
	{"RRC (HL)", 1, &UNIMPLEMENTED},		//0E
	{"RRC A", 1, &UNIMPLEMENTED},			//0F
	{"RL B", 1, &UNIMPLEMENTED},			//10
	{"RL C", 1, &UNIMPLEMENTED},			//11
	{"RL D", 1, &UNIMPLEMENTED},			//12
	{"RL E", 1, &UNIMPLEMENTED},			//13
	{"RL H", 1, &UNIMPLEMENTED},			//14
	{"RL L", 1, &UNIMPLEMENTED},			//15
	{"RL (HL)", 1, &UNIMPLEMENTED},			//16
	{"RL A", 1, &UNIMPLEMENTED},			//17
	{"RR B", 1, &UNIMPLEMENTED},			//18
	{"RR C", 1, &UNIMPLEMENTED},			//19
	{"RR D", 1, &UNIMPLEMENTED},			//1A
	{"RR E", 1, &UNIMPLEMENTED},			//1B
	{"RR H", 1, &UNIMPLEMENTED},			//1C
	{"RR L", 1, &UNIMPLEMENTED},			//1D
	{"RR (HL)", 1, &UNIMPLEMENTED},			//1E
	{"RR A", 1, &UNIMPLEMENTED},			//1F
	{"SLA B", 1, &UNIMPLEMENTED},			//20
	{"SLA C", 1, &UNIMPLEMENTED},			//21
	{"SLA D", 1, &UNIMPLEMENTED},			//22
	{"SLA E", 1, &UNIMPLEMENTED},			//23
	{"SLA H", 1, &UNIMPLEMENTED},			//24
	{"SLA L", 1, &UNIMPLEMENTED},			//25
	{"SLA (HL)", 1, &UNIMPLEMENTED},		//26
	{"SLA A", 1, &UNIMPLEMENTED},			//27
	{"SRA B", 1, &UNIMPLEMENTED},			//28
	{"SRA C", 1, &UNIMPLEMENTED},			//29
	{"SRA D", 1, &UNIMPLEMENTED},			//2A
	{"SRA E", 1, &UNIMPLEMENTED},			//2B
	{"SRA H", 1, &UNIMPLEMENTED},			//2C
	{"SRA L", 1, &UNIMPLEMENTED},			//2D
	{"SRA (HL)", 1, &UNIMPLEMENTED},		//2E
	{"SRA A", 1, &UNIMPLEMENTED},			//2F
	{"SWAP B", 1, &UNIMPLEMENTED},			//30
	{"SWAP C", 1, &UNIMPLEMENTED},			//31
	{"SWAP D", 1, &UNIMPLEMENTED},			//32
	{"SWAP E", 1, &UNIMPLEMENTED},			//33
	{"SWAP H", 1, &UNIMPLEMENTED},			//34
	{"SWAP L", 1, &UNIMPLEMENTED},			//35
	{"SWAP (HL)", 1, &UNIMPLEMENTED},		//36
	{"SWAP A", 1, &UNIMPLEMENTED},			//37
	{"SRL B", 1, &UNIMPLEMENTED},			//38
	{"SRL C", 1, &UNIMPLEMENTED},			//39
	{"SRL D", 1, &UNIMPLEMENTED},			//3A
	{"SRL E", 1, &UNIMPLEMENTED},			//3B
	{"SRL H", 1, &UNIMPLEMENTED},			//3C
	{"SRL L", 1, &UNIMPLEMENTED},			//3D
	{"SRL (HL)", 1, &UNIMPLEMENTED},		//3E
	{"SRL A", 1, &UNIMPLEMENTED},			//3F
	{"BIT 0, B", 1, &BIT_0_B},				//40
	{"BIT 0, C", 1, &BIT_0_C},				//41
	{"BIT 0, D", 1, &BIT_0_D},				//42
	{"BIT 0, E", 1, &BIT_0_E},				//43
	{"BIT 0, H", 1, &BIT_0_H},				//44
	{"BIT 0, L", 1, &BIT_0_L},				//45
	{"BIT 0, (HL)", 1, &BIT_0_HL},			//46
	{"BIT 0, A", 1, &BIT_0_A},				//47
	{"BIT 1, B", 1, &BIT_1_B},				//48
	{"BIT 1, C", 1, &BIT_1_C},				//49
	{"BIT 1, D", 1, &BIT_1_D},				//4A
	{"BIT 1, E", 1, &BIT_1_E},				//4B
	{"BIT 1, H", 1, &BIT_1_H},				//4C
	{"BIT 1, L", 1, &BIT_1_L},				//4D
	{"BIT 1, (HL)", 1, &BIT_1_HL},			//4E
	{"BIT 1, A", 1, &BIT_1_A},				//4F
	{"BIT 2, B", 1, &BIT_2_B},				//50
	{"BIT 2, C", 1, &BIT_2_C},				//51
	{"BIT 2, D", 1, &BIT_2_D},				//52
	{"BIT 2, E", 1, &BIT_2_E},				//53
	{"BIT 2, H", 1, &BIT_2_H},				//54
	{"BIT 2, L", 1, &BIT_2_L},				//55
	{"BIT 2, (HL)", 1, &BIT_2_HL},			//56
	{"BIT 2, A", 1, &BIT_2_A},				//57
	{"BIT 3, B", 1, &BIT_3_B},				//58
	{"BIT 3, C", 1, &BIT_3_C},				//59
	{"BIT 3, D", 1, &BIT_3_D},				//5A
	{"BIT 3, E", 1, &BIT_3_E},				//5B
	{"BIT 3, H", 1, &BIT_3_H},				//5C
	{"BIT 3, L", 1, &BIT_3_L},				//5D
	{"BIT 3, (HL)", 1, &BIT_3_HL},			//5E
	{"BIT 3, A", 1, &BIT_3_A},				//5F
	{"BIT 4, B", 1, &BIT_4_B},				//60
	{"BIT 4, C", 1, &BIT_4_C},				//61
	{"BIT 4, D", 1, &BIT_4_D},				//62
	{"BIT 4, E", 1, &BIT_4_E},				//63
	{"BIT 4, H", 1, &BIT_4_H},				//64
	{"BIT 4, L", 1, &BIT_4_L},				//65
	{"BIT 4, (HL)", 1, &BIT_4_HL},			//66
	{"BIT 4, A", 1, &BIT_4_A},				//67
	{"BIT 5, B", 1, &BIT_5_B},				//68
	{"BIT 5, C", 1, &BIT_5_C},				//69
	{"BIT 5, D", 1, &BIT_5_D},				//6A
	{"BIT 5, E", 1, &BIT_5_E},				//6B
	{"BIT 5, H", 1, &BIT_5_H},				//6C
	{"BIT 5, L", 1, &BIT_5_L},				//6D
	{"BIT 5, (HL)", 1, &BIT_5_HL},			//6E
	{"BIT 5, A", 1, &BIT_5_A},				//6F
	{"BIT 6, B", 1, &BIT_6_B},				//70
	{"BIT 6, C", 1, &BIT_6_C},				//71
	{"BIT 6, D", 1, &BIT_6_D},				//72
	{"BIT 6, E", 1, &BIT_6_E},				//73
	{"BIT 6, H", 1, &BIT_6_H},				//74
	{"BIT 6, L", 1, &BIT_6_L},				//75
	{"BIT 6, (HL)", 1, &BIT_6_HL},			//76
	{"BIT 6, A", 1, &BIT_6_A},				//77
	{"BIT 7, B", 1, &BIT_7_B},				//78
	{"BIT 7, C", 1, &BIT_7_C},				//79
	{"BIT 7, D", 1, &BIT_7_D},				//7A
	{"BIT 7, E", 1, &BIT_7_E},				//7B
	{"BIT 7, H", 1, &BIT_7_H},				//7C
	{"BIT 7, L", 1, &BIT_7_L},				//7D
	{"BIT 7, (HL)", 1, &BIT_7_HL},			//7E
	{"BIT 7, A", 1, &BIT_7_A},				//7F
	{"RES 0, B", 1, &UNIMPLEMENTED},		//80
	{"RES 0, C", 1, &UNIMPLEMENTED},		//81
	{"RES 0, D", 1, &UNIMPLEMENTED},		//82
	{"RES 0, E", 1, &UNIMPLEMENTED},		//83
	{"RES 0, H", 1, &UNIMPLEMENTED},		//84
	{"RES 0, L", 1, &UNIMPLEMENTED},		//85
	{"RES 0, (HL)", 1, &UNIMPLEMENTED},		//86
	{"RES 0, A", 1, &UNIMPLEMENTED},		//87
	{"RES 1, B", 1, &UNIMPLEMENTED},		//88
	{"RES 1, C", 1, &UNIMPLEMENTED},		//89
	{"RES 1, D", 1, &UNIMPLEMENTED},		//8A
	{"RES 1, E", 1, &UNIMPLEMENTED},		//8B
	{"RES 1, H", 1, &UNIMPLEMENTED},		//8C
	{"RES 1, L", 1, &UNIMPLEMENTED},		//8D
	{"RES 1, (HL)", 1, &UNIMPLEMENTED},		//8E
	{"RES 1, A", 1, &UNIMPLEMENTED},		//8F
	{"RES 2, B", 1, &UNIMPLEMENTED},		//90
	{"RES 2, C", 1, &UNIMPLEMENTED},		//91
	{"RES 2, D", 1, &UNIMPLEMENTED},		//92
	{"RES 2, E", 1, &UNIMPLEMENTED},		//93
	{"RES 2, H", 1, &UNIMPLEMENTED},		//94
	{"RES 2, L", 1, &UNIMPLEMENTED},		//95
	{"RES 2, (HL)", 1, &UNIMPLEMENTED},		//96
	{"RES 2, A", 1, &UNIMPLEMENTED},		//97
	{"RES 3, B", 1, &UNIMPLEMENTED},		//98
	{"RES 3, C", 1, &UNIMPLEMENTED},		//99
	{"RES 3, D", 1, &UNIMPLEMENTED},		//9A
	{"RES 3, E", 1, &UNIMPLEMENTED},		//9B
	{"RES 3, H", 1, &UNIMPLEMENTED},		//9C
	{"RES 3, L", 1, &UNIMPLEMENTED},		//9D
	{"RES 3, (HL)", 1, &UNIMPLEMENTED},		//9E
	{"RES 3, A", 1, &UNIMPLEMENTED},		//9F
	{"RES 4, B", 1, &UNIMPLEMENTED},		//A0
	{"RES 4, C", 1, &UNIMPLEMENTED},		//A1
	{"RES 4, D", 1, &UNIMPLEMENTED},		//A2
	{"RES 4, E", 1, &UNIMPLEMENTED},		//A3
	{"RES 4, H", 1, &UNIMPLEMENTED},		//A4
	{"RES 4, L", 1, &UNIMPLEMENTED},		//A5
	{"RES 4, (HL)", 1, &UNIMPLEMENTED},		//A6
	{"RES 4, A", 1, &UNIMPLEMENTED},		//A7
	{"RES 5, B", 1, &UNIMPLEMENTED},		//A8
	{"RES 5, C", 1, &UNIMPLEMENTED},		//A9
	{"RES 5, D", 1, &UNIMPLEMENTED},		//AA
	{"RES 5, E", 1, &UNIMPLEMENTED},		//AB
	{"RES 5, H", 1, &UNIMPLEMENTED},		//AC
	{"RES 5, L", 1, &UNIMPLEMENTED},		//AD
	{"RES 5, (HL)", 1, &UNIMPLEMENTED},		//AE
	{"RES 5, A", 1, &UNIMPLEMENTED},		//AF
	{"RES 6, B", 1, &UNIMPLEMENTED},		//B0
	{"RES 6, C", 1, &UNIMPLEMENTED},		//B1
	{"RES 6, D", 1, &UNIMPLEMENTED},		//B2
	{"RES 6, E", 1, &UNIMPLEMENTED},		//B3
	{"RES 6, H", 1, &UNIMPLEMENTED},		//B4
	{"RES 6, L", 1, &UNIMPLEMENTED},		//B5
	{"RES 6, (HL)", 1, &UNIMPLEMENTED},		//B6
	{"RES 6, A", 1, &UNIMPLEMENTED},		//B7
	{"RES 7, B", 1, &UNIMPLEMENTED},		//B8
	{"RES 7, C", 1, &UNIMPLEMENTED},		//B9
	{"RES 7, D", 1, &UNIMPLEMENTED},		//BA
	{"RES 7, E", 1, &UNIMPLEMENTED},		//BB
	{"RES 7, H", 1, &UNIMPLEMENTED},		//BC
	{"RES 7, L", 1, &UNIMPLEMENTED},		//BD
	{"RES 7, (HL)", 1, &UNIMPLEMENTED},		//BE
	{"RES 7, A", 1, &UNIMPLEMENTED},		//BF
	{"SET 0, B", 1, &UNIMPLEMENTED},		//C0
	{"SET 0, C", 1, &UNIMPLEMENTED},		//C1
	{"SET 0, D", 1, &UNIMPLEMENTED},		//C2
	{"SET 0, E", 1, &UNIMPLEMENTED},		//C3
	{"SET 0, H", 1, &UNIMPLEMENTED},		//C4
	{"SET 0, L", 1, &UNIMPLEMENTED},		//C5
	{"SET 0, (HL)", 1, &UNIMPLEMENTED},		//C6
	{"SET 0, A", 1, &UNIMPLEMENTED},		//C7
	{"SET 1, B", 1, &UNIMPLEMENTED},		//C8
	{"SET 1, C", 1, &UNIMPLEMENTED},		//C9
	{"SET 1, D", 1, &UNIMPLEMENTED},		//CA
	{"SET 1, E", 1, &UNIMPLEMENTED},		//CB
	{"SET 1, H", 1, &UNIMPLEMENTED},		//CC
	{"SET 1, L", 1, &UNIMPLEMENTED},		//CD
	{"SET 1, (HL)", 1, &UNIMPLEMENTED},		//CE
	{"SET 1, A", 1, &UNIMPLEMENTED},		//CF
	{"SET 2, B", 1, &UNIMPLEMENTED},		//D0
	{"SET 2, C", 1, &UNIMPLEMENTED},		//D1
	{"SET 2, D", 1, &UNIMPLEMENTED},		//D2
	{"SET 2, E", 1, &UNIMPLEMENTED},		//D3
	{"SET 2, H", 1, &UNIMPLEMENTED},		//D4
	{"SET 2, L", 1, &UNIMPLEMENTED},		//D5
	{"SET 2, (HL)", 1, &UNIMPLEMENTED},		//D6
	{"SET 2, A", 1, &UNIMPLEMENTED},		//D7
	{"SET 3, B", 1, &UNIMPLEMENTED},		//D8
	{"SET 3, C", 1, &UNIMPLEMENTED},		//D9
	{"SET 3, D", 1, &UNIMPLEMENTED},		//DA
	{"SET 3, E", 1, &UNIMPLEMENTED},		//DB
	{"SET 3, H", 1, &UNIMPLEMENTED},		//DC
	{"SET 3, L", 1, &UNIMPLEMENTED},		//DD
	{"SET 3, (HL)", 1, &UNIMPLEMENTED},		//DE
	{"SET 3, A", 1, &UNIMPLEMENTED},		//DF
	{"SET 4, B", 1, &UNIMPLEMENTED},		//E0
	{"SET 4, C", 1, &UNIMPLEMENTED},		//E1
	{"SET 4, D", 1, &UNIMPLEMENTED},		//E2
	{"SET 4, E", 1, &UNIMPLEMENTED},		//E3
	{"SET 4, H", 1, &UNIMPLEMENTED},		//E4
	{"SET 4, L", 1, &UNIMPLEMENTED},		//E5
	{"SET 4, (HL)", 1, &UNIMPLEMENTED},		//E6
	{"SET 4, A", 1, &UNIMPLEMENTED},		//E7
	{"SET 5, B", 1, &UNIMPLEMENTED},		//E8
	{"SET 5, C", 1, &UNIMPLEMENTED},		//E9
	{"SET 5, D", 1, &UNIMPLEMENTED},		//EA
	{"SET 5, E", 1, &UNIMPLEMENTED},		//EB
	{"SET 5, H", 1, &UNIMPLEMENTED},		//EC
	{"SET 5, L", 1, &UNIMPLEMENTED},		//ED
	{"SET 5, (HL)", 1, &UNIMPLEMENTED},		//EE
	{"SET 5, A", 1, &UNIMPLEMENTED},		//EF
	{"SET 6, B", 1, &UNIMPLEMENTED},		//F0
	{"SET 6, C", 1, &UNIMPLEMENTED},		//F1
	{"SET 6, D", 1, &UNIMPLEMENTED},		//F2
	{"SET 6, E", 1, &UNIMPLEMENTED},		//F3
	{"SET 6, H", 1, &UNIMPLEMENTED},		//F4
	{"SET 6, L", 1, &UNIMPLEMENTED},		//F5
	{"SET 6, (HL)", 1, &UNIMPLEMENTED},		//F6
	{"SET 6, A", 1, &UNIMPLEMENTED},		//F7
	{"SET 7, B", 1, &UNIMPLEMENTED},		//F8
	{"SET 7, C", 1, &UNIMPLEMENTED},		//F9
	{"SET 7, D", 1, &UNIMPLEMENTED},		//FA
	{"SET 7, E", 1, &UNIMPLEMENTED},		//FB
	{"SET 7, H", 1, &UNIMPLEMENTED},		//FC
	{"SET 7, L", 1, &UNIMPLEMENTED},		//FD
	{"SET 7, (HL)", 1, &UNIMPLEMENTED},		//FE
	{"SET 7, A", 1, &UNIMPLEMENTED},		//FF
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