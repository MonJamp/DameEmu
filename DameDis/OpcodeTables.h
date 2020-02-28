#pragma once
#include <cstdint>
#include <string>

struct Opcode {
	std::string mnemonic;
	uint8_t operand_bytes;
};

struct CB_Opcode : public Opcode {
	CB_Opcode(std::string mnemonic) : Opcode{ mnemonic, 0 } {};
	std::string mnemonic;
};

static Opcode opcodeTable[256] = {
	{"nop", 0},					//00
	{"ld BC, nn", 2},			//01
	{"ld (BC), A", 0},			//02
	{"inc BC", 0},				//03
	{"inc B", 0},				//04
	{"dec B", 0},				//05
	{"ld B, n", 1},			//06
	{"rlca", 0},				//07
	{"ld (nn), SP", 2},	//08
	{"add HL, BC", 0},			//09
	{"ld A, (BC)", 0},			//0A
	{"dec BC", 0},				//0B
	{"inc C", 0},				//0C
	{"dec C", 0},				//0D
	{"ld C, n", 1},			//0E
	{"rrca", 0},				//0F
	{"stop", 0},				//10
	{"ld DE, nn", 2},		//11
	{"ld (DE), A", 0},			//12
	{"inc DE", 0},				//13
	{"inc D", 0},				//14
	{"dec D", 0},				//15
	{"ld D, n", 1},			//16
	{"rla", 0},				//17
	{"jr n", 1},			//18
	{"add HL, DE", 0},			//19
	{"ld A, (DE)", 0},			//1A
	{"dec DE", 0},				//1B
	{"inc E", 0},				//1C
	{"dec E", 0},				//1D
	{"ld E, n", 1},			//1E
	{"rra", 0},				//1F
	{"jr nz, n", 1},		//20
	{"ld HL, nn", 2},		//21
	{"ld (HL+), A", 0},		//22
	{"inc HL", 0},				//23
	{"inc H", 0},				//24
	{"dec H", 0},				//25
	{"ld H, n", 1},			//26
	{"daa", 0},				//27
	{"jr Z, n", 1},			//28
	{"add HL, HL", 0},			//29
	{"ld A, (HL+)", 0},		//2A
	{"dec HL", 0},				//2B
	{"inc L", 0},				//2C
	{"dec L", 0},				//2D
	{"ld L, n", 1},			//2E
	{"cpl", 0},				//2F
	{"jr NC, n", 1},		//30
	{"ld SP, nn", 2},		//31
	{"ld (HL-), A", 0},		//32
	{"inc SP", 0},				//33
	{"inc (HL)", 0},			//34
	{"dec (HL)", 0},			//35
	{"ld (HL), n", 1},		//36
	{"SCF", 0},				//37
	{"jr C, n", 1},			//38
	{"add HL, SP", 0},			//39
	{"ld A, (HL-)", 0},		//3A
	{"dec SP", 0},				//3B
	{"inc A", 0},				//3C
	{"dec A", 0},				//3D
	{"ld A, n", 1},			//3E
	{"ccf", 0},				//3F
	{"ld B, B", 0},			//40
	{"ld B, C", 0},			//41
	{"ld B, D", 0},			//42
	{"ld B, E", 0},			//43
	{"ld B, H", 0},			//44
	{"ld B, L", 0},			//45
	{"ld B, (HL)", 0},			//46
	{"ld B, A", 0},			//47
	{"ld C, B", 0},			//48
	{"ld C, C", 0},			//49
	{"ld C, D", 0},			//4A
	{"ld C, E", 0},			//4B
	{"ld C, H", 0},			//4C
	{"ld C, L", 0},			//4D
	{"ld C, (HL)", 0},			//4E
	{"ld C, A", 0},			//4F
	{"ld D, B", 0},			//50
	{"ld D, C", 0},			//51
	{"ld D, D", 0},			//52
	{"ld D, E", 0},			//53
	{"ld D, H", 0},			//54
	{"ld D, L", 0},			//55
	{"ld D, (HL)", 0},			//56
	{"ld D, A", 0},			//57
	{"ld E, B", 0},			//58
	{"ld E, C", 0},			//59
	{"ld E, D", 0},			//5A
	{"ld E, E", 0},			//5B
	{"ld E, H", 0},			//5C
	{"ld E, L", 0},			//5D
	{"ld E, (HL)", 0},			//5E
	{"ld E, A", 0},			//5F
	{"ld H, B", 0},			//60
	{"ld H, C", 0},			//61
	{"ld H, D", 0},			//62
	{"ld H, E", 0},			//63
	{"ld H, H", 0},			//64
	{"ld H, L", 0},			//65
	{"ld H, (HL)", 0},			//66
	{"ld H, A", 0},			//67
	{"ld L, B", 0},			//68
	{"ld L, C", 0},			//69
	{"ld L, D", 0},			//6A
	{"ld L, E", 0},			//6B
	{"ld L, H", 0},			//6C
	{"ld L, L", 0},			//6D
	{"ld L, (HL)", 0},			//6E
	{"ld L, A", 0},			//6F
	{"ld (HL), B", 0},			//70
	{"ld (HL), C", 0},			//71
	{"ld (HL), D", 0},			//72
	{"ld (HL), E", 0},			//73
	{"ld (HL), H", 0},			//74
	{"ld (HL), L", 0},			//75
	{"halt", 0},				//76
	{"ld (HL), A", 0},			//77
	{"ld A, B", 0},			//78
	{"ld A, C", 0},			//79
	{"ld A, D", 0},			//7A
	{"ld A, E", 0},			//7B
	{"ld A, H", 0},			//7C
	{"ld A, L", 0},			//7D
	{"ld A, (HL)", 0},			//7E
	{"ld A, A", 0},			//7F
	{"add B", 0},				//80
	{"add C", 0},				//81
	{"add D", 0},				//82
	{"add E", 0},				//83
	{"add H", 0},				//84
	{"add L", 0},				//85
	{"add (HL)", 0},			//86
	{"add A", 0},				//87
	{"adc B", 0},				//88
	{"adc C", 0},				//89
	{"adc D", 0},				//8A
	{"adc E", 0},				//8B
	{"adc H", 0},				//8C
	{"adc L", 0},				//8D
	{"adc (HL)", 0},			//8E
	{"adc A", 0},				//8F
	{"sub B", 0},				//90
	{"sub C", 0},				//91
	{"sub D", 0},				//92
	{"sub E", 0},				//93
	{"sub H", 0},				//94
	{"sub L", 0},				//95
	{"sub (HL)", 0},			//96
	{"sub A", 0},				//97
	{"sbc B", 0},				//98
	{"sbc C", 0},				//99
	{"sbc D", 0},				//9A
	{"sbc E", 0},				//9B
	{"sbc H", 0},				//9C
	{"sbc L", 0},				//9D
	{"sbc (HL)", 0},			//9E
	{"sbc A", 0},				//9F
	{"and B", 0},				//A0
	{"and C", 0},				//A1
	{"and D", 0},				//A2
	{"and E", 0,},				//A3
	{"and H", 0,},				//A4
	{"and L", 0,},				//A5
	{"and (HL)", 0L},			//A6
	{"and A", 0},				//A7
	{"xor B", 0},				//A8
	{"xor C", 0},				//A9
	{"xor D", 0},				//AA
	{"xor E", 0},				//AB
	{"xor H", 0},				//AC
	{"xor L", 0},				//AD
	{"xor (HL)", 0},			//AE
	{"xor A", 0},				//AF
	{"or B", 0},				//B0
	{"or C", 0},				//B1
	{"or D", 0},				//B2
	{"or E", 0},				//B3
	{"or H", 0},				//B4
	{"or L", 0},				//B5
	{"or (HL)", 0},			//B6
	{"or A", 0},				//B7
	{"cp B", 0},				//B8
	{"cp C", 0},				//B9
	{"cp D", 0},				//BA
	{"cp E", 0},				//BB
	{"cp H", 0},				//BC
	{"cp L", 0},				//BD
	{"cp (HL)", 0},			//BE
	{"cp A", 0},				//BF
	{"ret nz", 0},				//C0
	{"pop BC", 0},				//C1
	{"jp nz, nn", 2},		//C2
	{"jp nn", 2},			//C3
	{"call nz, nn", 2},	//C4
	{"push BC", 0},			//C5
	{"add n", 1},			//C6
	{"rst 0x00", 0},			//C7
	{"ret Z", 0},				//C8
	{"ret", 0},				//C9
	{"jp Z, nn", 2},		//CA
	{"CB n", 1},			//CB
	{"call Z, nn", 2},		//CC
	{"call nn", 2},		//CD
	{"adc n", 1},			//CE
	{"rst 0x08", 0},			//CF
	{"ret NC", 0},				//D0
	{"pop DE", 0},				//D1
	{"jp NC, nn", 2},		//D2
	{"Undefined OP", 0},		//D3
	{"call NC, nn", 2},	//D4
	{"push DE", 0},			//D5
	{"sub n", 1},			//D6
	{"rst 0x00", 0},			//D7
	{"ret C", 0},				//D8
	{"reti", 0},				//D9
	{"jp C, nn", 2},		//DA
	{"Undefined OP", 0},		//DB
	{"call C, nn", 2},		//DC
	{"Undefined OP", 0},		//DD
	{"sbc n", 1},			//DE
	{"rst 0x08", 0},			//DF
	{"ld (FF00+n), A", 1},	//E0
	{"pop HL", 0},				//E1
	{"ld (FF00+C), A", 0},		//E2
	{"Undefined OP", 0},		//E3
	{"Undefined OP", 0},		//E4
	{"push HL", 0},			//E5
	{"and n", 1},			//E6
	{"rst 0x10", 0},			//E7
	{"add SP, n", 1},		//E8
	{"jp HL", 0},				//E9
	{"ld (nn), A", 2},		//EA
	{"Undefined OP", 0},		//EB
	{"Undefined OP", 0},		//EC
	{"Undefined OP", 0},		//ED
	{"xor n", 1},			//EE
	{"rst 0x18", 0},			//EF
	{"ld A, (FF00+n)", 1},	//F0
	{"pop AF", 0},				//F1
	{"ld A, (FF00+C)", 0},		//F2
	{"di", 0},					//F3
	{"Undefined OP", 0},		//F4
	{"push AF", 0},			//F5
	{"or n", 1},			//F6
	{"rst 0x20", 0},			//F7
	{"ld HL, SP+n", 1},		//F8
	{"ld SP, HL", 0},			//F9
	{"ld A, (nn)", 2},		//FA
	{"ei", 0},					//FB
	{"Undefined OP", 0},		//FC
	{"Undefined OP", 0},		//FD
	{"cp n", 1},			//FE
	{"rst 0x28", 0},			//FF
};

static CB_Opcode cb_opcodeTable[256] = {
	{"rlc B"},			//00
	{"rlc C"},			//01
	{"rlc D"},			//02
	{"rlc E"},			//03
	{"rlc H"},			//04
	{"rlc L"},			//05
	{"rlc (HL)"},		//06
	{"rlc A"},			//07
	{"rrc B"},			//08
	{"rrc C"},			//09
	{"rrc D"},			//0A
	{"rrc E"},			//0B
	{"rrc H"},			//0C
	{"rrc L"},			//0D
	{"rrc (HL)"},		//0E
	{"rrc A"},			//0F
	{"rl B"},			//10
	{"rl C"},			//11
	{"rl D"},			//12
	{"rl E"},			//13
	{"rl H"},			//14
	{"rl L"},			//15
	{"rl (HL)"},		//16
	{"rl A"},			//17
	{"rr B"},			//18
	{"rr C"},			//19
	{"rr D"},			//1A
	{"rr E"},			//1B
	{"rr H"},			//1C
	{"rr L"},			//1D
	{"rr (HL)"},		//1E
	{"rr A"},			//1F
	{"sla B"},			//20
	{"sla C"},			//21
	{"sla D"},			//22
	{"sla E"},			//23
	{"sla H"},			//24
	{"sla L"},			//25
	{"sla (HL)"},		//26
	{"sla A"},			//27
	{"sra B"},			//28
	{"sra C"},			//29
	{"sra D"},			//2A
	{"sra E"},			//2B
	{"sra H"},			//2C
	{"sra L"},			//2D
	{"sra (HL)"},		//2E
	{"sra A"},			//2F
	{"swap B"},		//30
	{"swap C"},		//31
	{"swap D"},		//32
	{"swap E"},		//33
	{"swap H"},		//34
	{"swap L"},		//35
	{"swap (HL)"},		//36
	{"swap A"},		//37
	{"srl B"},			//38
	{"srl C"},			//39
	{"srl D"},			//3A
	{"srl E"},			//3B
	{"srl H"},			//3C
	{"srl L"},			//3D
	{"srl (HL)"},		//3E
	{"srl A"},			//3F
	{"bit 0, B"},		//40
	{"bit 0, C"},		//41
	{"bit 0, D"},		//42
	{"bit 0, E"},		//43
	{"bit 0, H"},		//44
	{"bit 0, L"},		//45
	{"bit 0, (HL)"},	//46
	{"bit 0, A"},		//47
	{"bit 1, B"},		//48
	{"bit 1, C"},		//49
	{"bit 1, D"},		//4A
	{"bit 1, E"},		//4B
	{"bit 1, H"},		//4C
	{"bit 1, L"},		//4D
	{"bit 1, (HL)"},	//4E
	{"bit 1, A"},		//4F
	{"bit 2, B"},		//50
	{"bit 2, C"},		//51
	{"bit 2, D"},		//52
	{"bit 2, E"},		//53
	{"bit 2, H"},		//54
	{"bit 2, L"},		//55
	{"bit 2, (HL)"},	//56
	{"bit 2, A"},		//57
	{"bit 3, B"},		//58
	{"bit 3, C"},		//59
	{"bit 3, D"},		//5A
	{"bit 3, E"},		//5B
	{"bit 3, H"},		//5C
	{"bit 3, L"},		//5D
	{"bit 3, (HL)"},	//5E
	{"bit 3, A"},		//5F
	{"bit 4, B"},		//60
	{"bit 4, C"},		//61
	{"bit 4, D"},		//62
	{"bit 4, E"},		//63
	{"bit 4, H"},		//64
	{"bit 4, L"},		//65
	{"bit 4, (HL)"},	//66
	{"bit 4, A"},		//67
	{"bit 5, B"},		//68
	{"bit 5, C"},		//69
	{"bit 5, D"},		//6A
	{"bit 5, E"},		//6B
	{"bit 5, H"},		//6C
	{"bit 5, L"},		//6D
	{"bit 5, (HL)"},	//6E
	{"bit 5, A"},		//6F
	{"bit 6, B"},		//70
	{"bit 6, C"},		//71
	{"bit 6, D"},		//72
	{"bit 6, E"},		//73
	{"bit 6, H"},		//74
	{"bit 6, L"},		//75
	{"bit 6, (HL)"},	//76
	{"bit 6, A"},		//77
	{"bit 7, B"},		//78
	{"bit 7, C"},		//79
	{"bit 7, D"},		//7A
	{"bit 7, E"},		//7B
	{"bit 7, H"},		//7C
	{"bit 7, L"},		//7D
	{"bit 7, (HL)"},	//7E
	{"bit 7, A"},		//7F
	{"res 0, B"},		//80
	{"res 0, C"},		//81
	{"res 0, D"},		//82
	{"res 0, E"},		//83
	{"res 0, H"},		//84
	{"res 0, L"},		//85
	{"res 0, (HL)"},	//86
	{"res 0, A"},		//87
	{"res 1, B"},		//88
	{"res 1, C"},		//89
	{"res 1, D"},		//8A
	{"res 1, E"},		//8B
	{"res 1, H"},		//8C
	{"res 1, L"},		//8D
	{"res 1, (HL)"},	//8E
	{"res 1, A"},		//8F
	{"res 2, B"},		//90
	{"res 2, C"},		//91
	{"res 2, D"},		//92
	{"res 2, E"},		//93
	{"res 2, H"},		//94
	{"res 2, L"},		//95
	{"res 2, (HL)"},	//96
	{"res 2, A"},		//97
	{"res 3, B"},		//98
	{"res 3, C"},		//99
	{"res 3, D"},		//9A
	{"res 3, E"},		//9B
	{"res 3, H"},		//9C
	{"res 3, L"},		//9D
	{"res 3, (HL)"},	//9E
	{"res 3, A"},		//9F
	{"res 4, B"},		//A0
	{"res 4, C"},		//A1
	{"res 4, D"},		//A2
	{"res 4, E"},		//A3
	{"res 4, H"},		//A4
	{"res 4, L"},		//A5
	{"res 4, (HL)"},	//A6
	{"res 4, A"},		//A7
	{"res 5, B"},		//A8
	{"res 5, C"},		//A9
	{"res 5, D"},		//AA
	{"res 5, E"},		//AB
	{"res 5, H"},		//AC
	{"res 5, L"},		//AD
	{"res 5, (HL)"},	//AE
	{"res 5, A"},		//AF
	{"res 6, B"},		//B0
	{"res 6, C"},		//B1
	{"res 6, D"},		//B2
	{"res 6, E"},		//B3
	{"res 6, H"},		//B4
	{"res 6, L"},		//B5
	{"res 6, (HL)"},	//B6
	{"res 6, A"},		//B7
	{"res 7, B"},		//B8
	{"res 7, C"},		//B9
	{"res 7, D"},		//BA
	{"res 7, E"},		//BB
	{"res 7, H"},		//BC
	{"res 7, L"},		//BD
	{"res 7, (HL)"},	//BE
	{"res 7, A"},		//BF
	{"set 0, B"},		//C0
	{"set 0, C"},		//C1
	{"set 0, D"},		//C2
	{"set 0, E"},		//C3
	{"set 0, H"},		//C4
	{"set 0, L"},		//C5
	{"set 0, (HL)"},	//C6
	{"set 0, A"},		//C7
	{"set 1, B"},		//C8
	{"set 1, C"},		//C9
	{"set 1, D"},		//CA
	{"set 1, E"},		//CB
	{"set 1, H"},		//CC
	{"set 1, L"},		//CD
	{"set 1, (HL)"},	//CE
	{"set 1, A"},		//CF
	{"set 2, B"},		//D0
	{"set 2, C"},		//D1
	{"set 2, D"},		//D2
	{"set 2, E"},		//D3
	{"set 2, H"},		//D4
	{"set 2, L"},		//D5
	{"set 2, (HL)"},	//D6
	{"set 2, A"},		//D7
	{"set 3, B"},		//D8
	{"set 3, C"},		//D9
	{"set 3, D"},		//DA
	{"set 3, E"},		//DB
	{"set 3, H"},		//DC
	{"set 3, L"},		//DD
	{"set 3, (HL)"},	//DE
	{"set 3, A"},		//DF
	{"set 4, B"},		//E0
	{"set 4, C"},		//E1
	{"set 4, D"},		//E2
	{"set 4, E"},		//E3
	{"set 4, H"},		//E4
	{"set 4, L"},		//E5
	{"set 4, (HL)"},	//E6
	{"set 4, A"},		//E7
	{"set 5, B"},		//E8
	{"set 5, C"},		//E9
	{"set 5, D"},		//EA
	{"set 5, E"},		//EB
	{"set 5, H"},		//EC
	{"set 5, L"},		//ED
	{"set 5, (HL)"},	//EE
	{"set 5, A"},		//EF
	{"set 6, B"},		//F0
	{"set 6, C"},		//F1
	{"set 6, D"},		//F2
	{"set 6, E"},		//F3
	{"set 6, H"},		//F4
	{"set 6, L"},		//F5
	{"set 6, (HL)"},	//F6
	{"set 6, A"},		//F7
	{"set 7, B"},		//F8
	{"set 7, C"},		//F9
	{"set 7, D"},		//FA
	{"set 7, E"},		//FB
	{"set 7, H"},		//FC
	{"set 7, L"},		//FD
	{"set 7, (HL)"},	//FE
	{"set 7, A"},		//FF
};