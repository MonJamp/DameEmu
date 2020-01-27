#include "DameEmu.h"

DameEmu::Instruction DameEmu::cb_instructions[256] = {
	{"RLC B", 1, &UNKNOWN_CB},			//00
	{"RLC C", 1, &UNKNOWN_CB},			//01
	{"RLC D", 1, &UNKNOWN_CB},			//02
	{"RLC E", 1, &UNKNOWN_CB},			//03
	{"RLC H", 1, &UNKNOWN_CB},			//04
	{"RLC L", 1, &UNKNOWN_CB},			//05
	{"RLC (HL)", 1, &UNKNOWN_CB},		//06
	{"RLC A", 1, &UNKNOWN_CB},			//07
	{"RRC B", 1, &UNKNOWN_CB},			//08
	{"RRC C", 1, &UNKNOWN_CB},			//09
	{"RRC D", 1, &UNKNOWN_CB},			//0A
	{"RRC E", 1, &UNKNOWN_CB},			//0B
	{"RRC H", 1, &UNKNOWN_CB},			//0C
	{"RRC L", 1, &UNKNOWN_CB},			//0D
	{"RRC (HL)", 1, &UNKNOWN_CB},		//0E
	{"RRC A", 1, &UNKNOWN_CB},			//0F
	{"RL B", 1, &UNKNOWN_CB},			//10
	{"RL C", 1, &UNKNOWN_CB},			//11
	{"RL D", 1, &UNKNOWN_CB},			//12
	{"RL E", 1, &UNKNOWN_CB},			//13
	{"RL H", 1, &UNKNOWN_CB},			//14
	{"RL L", 1, &UNKNOWN_CB},			//15
	{"RL (HL)", 1, &UNKNOWN_CB},		//16
	{"RL A", 1, &UNKNOWN_CB},			//17
	{"RR B", 1, &UNKNOWN_CB},			//18
	{"RR C", 1, &UNKNOWN_CB},			//19
	{"RR D", 1, &UNKNOWN_CB},			//1A
	{"RR E", 1, &UNKNOWN_CB},			//1B
	{"RR H", 1, &UNKNOWN_CB},			//1C
	{"RR L", 1, &UNKNOWN_CB},			//1D
	{"RR (HL)", 1, &UNKNOWN_CB},		//1E
	{"RR A", 1, &UNKNOWN_CB},			//1F
	{"SLA B", 1, &UNKNOWN_CB},			//20
	{"SLA C", 1, &UNKNOWN_CB},			//21
	{"SLA D", 1, &UNKNOWN_CB},			//22
	{"SLA E", 1, &UNKNOWN_CB},			//23
	{"SLA H", 1, &UNKNOWN_CB},			//24
	{"SLA L", 1, &UNKNOWN_CB},			//25
	{"SLA (HL)", 1, &UNKNOWN_CB},		//26
	{"SLA A", 1, &UNKNOWN_CB},			//27
	{"SRA B", 1, &UNKNOWN_CB},			//28
	{"SRA C", 1, &UNKNOWN_CB},			//29
	{"SRA D", 1, &UNKNOWN_CB},			//2A
	{"SRA E", 1, &UNKNOWN_CB},			//2B
	{"SRA H", 1, &UNKNOWN_CB},			//2C
	{"SRA L", 1, &UNKNOWN_CB},			//2D
	{"SRA (HL)", 1, &UNKNOWN_CB},		//2E
	{"SRA A", 1, &UNKNOWN_CB},			//2F
	{"SWAP B", 1, &UNKNOWN_CB},			//30
	{"SWAP C", 1, &UNKNOWN_CB},			//31
	{"SWAP D", 1, &UNKNOWN_CB},			//32
	{"SWAP E", 1, &UNKNOWN_CB},			//33
	{"SWAP H", 1, &UNKNOWN_CB},			//34
	{"SWAP L", 1, &UNKNOWN_CB},			//35
	{"SWAP (HL)", 1, &UNKNOWN_CB},		//36
	{"SWAP A", 1, &UNKNOWN_CB},			//37
	{"SRL B", 1, &UNKNOWN_CB},			//38
	{"SRL C", 1, &UNKNOWN_CB},			//39
	{"SRL D", 1, &UNKNOWN_CB},			//3A
	{"SRL E", 1, &UNKNOWN_CB},			//3B
	{"SRL H", 1, &UNKNOWN_CB},			//3C
	{"SRL L", 1, &UNKNOWN_CB},			//3D
	{"SRL (HL)", 1, &UNKNOWN_CB},		//3E
	{"SRL A", 1, &UNKNOWN_CB},			//3F
	{"BIT 0, B", 1, &UNKNOWN_CB},		//40
	{"BIT 0, C", 1, &UNKNOWN_CB},		//41
	{"BIT 0, D", 1, &UNKNOWN_CB},		//42
	{"BIT 0, E", 1, &UNKNOWN_CB},		//43
	{"BIT 0, H", 1, &UNKNOWN_CB},		//44
	{"BIT 0, L", 1, &UNKNOWN_CB},		//45
	{"BIT 0, (HL)", 1, &UNKNOWN_CB},	//46
	{"BIT 0, A", 1, &UNKNOWN_CB},		//47
	{"BIT 1, B", 1, &UNKNOWN_CB},		//48
	{"BIT 1, C", 1, &UNKNOWN_CB},		//49
	{"BIT 1, D", 1, &UNKNOWN_CB},		//4A
	{"BIT 1, E", 1, &UNKNOWN_CB},		//4B
	{"BIT 1, H", 1, &UNKNOWN_CB},		//4C
	{"BIT 1, L", 1, &UNKNOWN_CB},		//4D
	{"BIT 1, (HL)", 1, &UNKNOWN_CB},	//4E
	{"BIT 1, A", 1, &UNKNOWN_CB},		//4F
	{"BIT 2, B", 1, &UNKNOWN_CB},		//50
	{"BIT 2, C", 1, &UNKNOWN_CB},		//51
	{"BIT 2, D", 1, &UNKNOWN_CB},		//52
	{"BIT 2, E", 1, &UNKNOWN_CB},		//53
	{"BIT 2, H", 1, &UNKNOWN_CB},		//54
	{"BIT 2, L", 1, &UNKNOWN_CB},		//55
	{"BIT 2, (HL)", 1, &UNKNOWN_CB},	//56
	{"BIT 2, A", 1, &UNKNOWN_CB},		//57
	{"BIT 3, B", 1, &UNKNOWN_CB},		//58
	{"BIT 3, C", 1, &UNKNOWN_CB},		//59
	{"BIT 3, D", 1, &UNKNOWN_CB},		//5A
	{"BIT 3, E", 1, &UNKNOWN_CB},		//5B
	{"BIT 3, H", 1, &UNKNOWN_CB},		//5C
	{"BIT 3, L", 1, &UNKNOWN_CB},		//5D
	{"BIT 3, (HL)", 1, &UNKNOWN_CB},	//5E
	{"BIT 3, A", 1, &UNKNOWN_CB},		//5F
	{"BIT 4, B", 1, &UNKNOWN_CB},		//60
	{"BIT 4, C", 1, &UNKNOWN_CB},		//61
	{"BIT 4, D", 1, &UNKNOWN_CB},		//62
	{"BIT 4, E", 1, &UNKNOWN_CB},		//63
	{"BIT 4, H", 1, &UNKNOWN_CB},		//64
	{"BIT 4, L", 1, &UNKNOWN_CB},		//65
	{"BIT 4, (HL)", 1, &UNKNOWN_CB},	//66
	{"BIT 4, A", 1, &UNKNOWN_CB},		//67
	{"BIT 5, B", 1, &UNKNOWN_CB},		//68
	{"BIT 5, C", 1, &UNKNOWN_CB},		//69
	{"BIT 5, D", 1, &UNKNOWN_CB},		//6A
	{"BIT 5, E", 1, &UNKNOWN_CB},		//6B
	{"BIT 5, H", 1, &UNKNOWN_CB},		//6C
	{"BIT 5, L", 1, &UNKNOWN_CB},		//6D
	{"BIT 5, (HL)", 1, &UNKNOWN_CB},	//6E
	{"BIT 5, A", 1, &UNKNOWN_CB},		//6F
	{"BIT 6, B", 1, &UNKNOWN_CB},		//70
	{"BIT 6, C", 1, &UNKNOWN_CB},		//71
	{"BIT 6, D", 1, &UNKNOWN_CB},		//72
	{"BIT 6, E", 1, &UNKNOWN_CB},		//73
	{"BIT 6, H", 1, &UNKNOWN_CB},		//74
	{"BIT 6, L", 1, &UNKNOWN_CB},		//75
	{"BIT 6, (HL)", 1, &UNKNOWN_CB},	//76
	{"BIT 6, A", 1, &UNKNOWN_CB},		//77
	{"BIT 7, B", 1, &UNKNOWN_CB},		//78
	{"BIT 7, C", 1, &UNKNOWN_CB},		//79
	{"BIT 7, D", 1, &UNKNOWN_CB},		//7A
	{"BIT 7, E", 1, &UNKNOWN_CB},		//7B
	{"BIT 7, H", 1, &UNKNOWN_CB},		//7C
	{"BIT 7, L", 1, &UNKNOWN_CB},		//7D
	{"BIT 7, (HL)", 1, &UNKNOWN_CB},	//7E
	{"BIT 7, A", 1, &UNKNOWN_CB},		//7F
	{"RES 0, B", 1, &UNKNOWN_CB},		//80
	{"RES 0, C", 1, &UNKNOWN_CB},		//81
	{"RES 0, D", 1, &UNKNOWN_CB},		//82
	{"RES 0, E", 1, &UNKNOWN_CB},		//83
	{"RES 0, H", 1, &UNKNOWN_CB},		//84
	{"RES 0, L", 1, &UNKNOWN_CB},		//85
	{"RES 0, (HL)", 1, &UNKNOWN_CB},	//86
	{"RES 0, A", 1, &UNKNOWN_CB},		//87
	{"RES 1, B", 1, &UNKNOWN_CB},		//88
	{"RES 1, C", 1, &UNKNOWN_CB},		//89
	{"RES 1, D", 1, &UNKNOWN_CB},		//8A
	{"RES 1, E", 1, &UNKNOWN_CB},		//8B
	{"RES 1, H", 1, &UNKNOWN_CB},		//8C
	{"RES 1, L", 1, &UNKNOWN_CB},		//8D
	{"RES 1, (HL)", 1, &UNKNOWN_CB},	//8E
	{"RES 1, A", 1, &UNKNOWN_CB},		//8F
	{"RES 2, B", 1, &UNKNOWN_CB},		//90
	{"RES 2, C", 1, &UNKNOWN_CB},		//91
	{"RES 2, D", 1, &UNKNOWN_CB},		//92
	{"RES 2, E", 1, &UNKNOWN_CB},		//93
	{"RES 2, H", 1, &UNKNOWN_CB},		//94
	{"RES 2, L", 1, &UNKNOWN_CB},		//95
	{"RES 2, (HL)", 1, &UNKNOWN_CB},	//96
	{"RES 2, A", 1, &UNKNOWN_CB},		//97
	{"RES 3, B", 1, &UNKNOWN_CB},		//98
	{"RES 3, C", 1, &UNKNOWN_CB},		//99
	{"RES 3, D", 1, &UNKNOWN_CB},		//9A
	{"RES 3, E", 1, &UNKNOWN_CB},		//9B
	{"RES 3, H", 1, &UNKNOWN_CB},		//9C
	{"RES 3, L", 1, &UNKNOWN_CB},		//9D
	{"RES 3, (HL)", 1, &UNKNOWN_CB},	//9E
	{"RES 3, A", 1, &UNKNOWN_CB},		//9F
	{"RES 4, B", 1, &UNKNOWN_CB},		//A0
	{"RES 4, C", 1, &UNKNOWN_CB},		//A1
	{"RES 4, D", 1, &UNKNOWN_CB},		//A2
	{"RES 4, E", 1, &UNKNOWN_CB},		//A3
	{"RES 4, H", 1, &UNKNOWN_CB},		//A4
	{"RES 4, L", 1, &UNKNOWN_CB},		//A5
	{"RES 4, (HL)", 1, &UNKNOWN_CB},	//A6
	{"RES 4, A", 1, &UNKNOWN_CB},		//A7
	{"RES 5, B", 1, &UNKNOWN_CB},		//A8
	{"RES 5, C", 1, &UNKNOWN_CB},		//A9
	{"RES 5, D", 1, &UNKNOWN_CB},		//AA
	{"RES 5, E", 1, &UNKNOWN_CB},		//AB
	{"RES 5, H", 1, &UNKNOWN_CB},		//AC
	{"RES 5, L", 1, &UNKNOWN_CB},		//AD
	{"RES 5, (HL)", 1, &UNKNOWN_CB},	//AE
	{"RES 5, A", 1, &UNKNOWN_CB},		//AF
	{"RES 6, B", 1, &UNKNOWN_CB},		//B0
	{"RES 6, C", 1, &UNKNOWN_CB},		//B1
	{"RES 6, D", 1, &UNKNOWN_CB},		//B2
	{"RES 6, E", 1, &UNKNOWN_CB},		//B3
	{"RES 6, H", 1, &UNKNOWN_CB},		//B4
	{"RES 6, L", 1, &UNKNOWN_CB},		//B5
	{"RES 6, (HL)", 1, &UNKNOWN_CB},	//B6
	{"RES 6, A", 1, &UNKNOWN_CB},		//B7
	{"RES 7, B", 1, &UNKNOWN_CB},		//B8
	{"RES 7, C", 1, &UNKNOWN_CB},		//B9
	{"RES 7, D", 1, &UNKNOWN_CB},		//BA
	{"RES 7, E", 1, &UNKNOWN_CB},		//BB
	{"RES 7, H", 1, &UNKNOWN_CB},		//BC
	{"RES 7, L", 1, &UNKNOWN_CB},		//BD
	{"RES 7, (HL)", 1, &UNKNOWN_CB},	//BE
	{"RES 7, A", 1, &UNKNOWN_CB},		//BF
	{"SET 0, B", 1, &UNKNOWN_CB},		//C0
	{"SET 0, C", 1, &UNKNOWN_CB},		//C1
	{"SET 0, D", 1, &UNKNOWN_CB},		//C2
	{"SET 0, E", 1, &UNKNOWN_CB},		//C3
	{"SET 0, H", 1, &UNKNOWN_CB},		//C4
	{"SET 0, L", 1, &UNKNOWN_CB},		//C5
	{"SET 0, (HL)", 1, &UNKNOWN_CB},	//C6
	{"SET 0, A", 1, &UNKNOWN_CB},		//C7
	{"SET 1, B", 1, &UNKNOWN_CB},		//C8
	{"SET 1, C", 1, &UNKNOWN_CB},		//C9
	{"SET 1, D", 1, &UNKNOWN_CB},		//CA
	{"SET 1, E", 1, &UNKNOWN_CB},		//CB
	{"SET 1, H", 1, &UNKNOWN_CB},		//CC
	{"SET 1, L", 1, &UNKNOWN_CB},		//CD
	{"SET 1, (HL)", 1, &UNKNOWN_CB},	//CE
	{"SET 1, A", 1, &UNKNOWN_CB},		//CF
	{"SET 2, B", 1, &UNKNOWN_CB},		//D0
	{"SET 2, C", 1, &UNKNOWN_CB},		//D1
	{"SET 2, D", 1, &UNKNOWN_CB},		//D2
	{"SET 2, E", 1, &UNKNOWN_CB},		//D3
	{"SET 2, H", 1, &UNKNOWN_CB},		//D4
	{"SET 2, L", 1, &UNKNOWN_CB},		//D5
	{"SET 2, (HL)", 1, &UNKNOWN_CB},	//D6
	{"SET 2, A", 1, &UNKNOWN_CB},		//D7
	{"SET 3, B", 1, &UNKNOWN_CB},		//D8
	{"SET 3, C", 1, &UNKNOWN_CB},		//D9
	{"SET 3, D", 1, &UNKNOWN_CB},		//DA
	{"SET 3, E", 1, &UNKNOWN_CB},		//DB
	{"SET 3, H", 1, &UNKNOWN_CB},		//DC
	{"SET 3, L", 1, &UNKNOWN_CB},		//DD
	{"SET 3, (HL)", 1, &UNKNOWN_CB},	//DE
	{"SET 3, A", 1, &UNKNOWN_CB},		//DF
	{"SET 4, B", 1, &UNKNOWN_CB},		//E0
	{"SET 4, C", 1, &UNKNOWN_CB},		//E1
	{"SET 4, D", 1, &UNKNOWN_CB},		//E2
	{"SET 4, E", 1, &UNKNOWN_CB},		//E3
	{"SET 4, H", 1, &UNKNOWN_CB},		//E4
	{"SET 4, L", 1, &UNKNOWN_CB},		//E5
	{"SET 4, (HL)", 1, &UNKNOWN_CB},	//E6
	{"SET 4, A", 1, &UNKNOWN_CB},		//E7
	{"SET 5, B", 1, &UNKNOWN_CB},		//E8
	{"SET 5, C", 1, &UNKNOWN_CB},		//E9
	{"SET 5, D", 1, &UNKNOWN_CB},		//EA
	{"SET 5, E", 1, &UNKNOWN_CB},		//EB
	{"SET 5, H", 1, &UNKNOWN_CB},		//EC
	{"SET 5, L", 1, &UNKNOWN_CB},		//ED
	{"SET 5, (HL)", 1, &UNKNOWN_CB},	//EE
	{"SET 5, A", 1, &UNKNOWN_CB},		//EF
	{"SET 6, B", 1, &UNKNOWN_CB},		//F0
	{"SET 6, C", 1, &UNKNOWN_CB},		//F1
	{"SET 6, D", 1, &UNKNOWN_CB},		//F2
	{"SET 6, E", 1, &UNKNOWN_CB},		//F3
	{"SET 6, H", 1, &UNKNOWN_CB},		//F4
	{"SET 6, L", 1, &UNKNOWN_CB},		//F5
	{"SET 6, (HL)", 1, &UNKNOWN_CB},	//F6
	{"SET 6, A", 1, &UNKNOWN_CB},		//F7
	{"SET 7, B", 1, &UNKNOWN_CB},		//F8
	{"SET 7, C", 1, &UNKNOWN_CB},		//F9
	{"SET 7, D", 1, &UNKNOWN_CB},		//FA
	{"SET 7, E", 1, &UNKNOWN_CB},		//FB
	{"SET 7, H", 1, &UNKNOWN_CB},		//FC
	{"SET 7, L", 1, &UNKNOWN_CB},		//FD
	{"SET 7, (HL)", 1, &UNKNOWN_CB},	//FE
	{"SET 7, A", 1, &UNKNOWN_CB},		//FF
};