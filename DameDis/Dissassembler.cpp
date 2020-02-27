#include "Dissassembler.h"
#include <sstream>
#include <fstream>
#include <iostream>


Opcode Dissassembler::opcodeTable[256] = {
	{"NOP;", 0},				//00
	{"LD BC, nn; nn=", 2},		//01
	{"LD (BC), A;", 0},			//02
	{"INC BC;", 0},				//03
	{"INC B;", 0},				//04
	{"DEC B;", 0},				//05
	{"LD B, n; n=", 1},			//06
	{"RLCA;", 0},				//07
	{"LD (nn), SP; nn=", 2},	//08
	{"ADD HL, BC;", 0},			//09
	{"LD A, (BC);", 0},			//0A
	{"DEC BC;", 0},				//0B
	{"INC C;", 0},				//0C
	{"DEC C;", 0},				//0D
	{"LD C, n; n=", 1},			//0E
	{"RRCA;", 0},				//0F
	{"STOP;", 0},				//10
	{"LD DE, nn; nn=", 2},		//11
	{"LD (DE), A;", 0},			//12
	{"INC DE;", 0},				//13
	{"INC D;", 0},				//14
	{"DEC D;", 0},				//15
	{"LD D, n; n=", 1},			//16
	{"RLA;", 0},				//17
	{"JR n; n=", 1},			//18
	{"ADD HL, DE;", 0},			//19
	{"LD A, (DE);", 0},			//1A
	{"DEC DE;", 0},				//1B
	{"INC E;", 0},				//1C
	{"DEC E;", 0},				//1D
	{"LD E, n; n=", 1},			//1E
	{"RRA;", 0},				//1F
	{"JR NZ, n; n=", 1},		//20
	{"LD HL, nn; nn=", 2},		//21
	{"LD (HL+), A;", 0},		//22
	{"INC HL;", 0},				//23
	{"INC H;", 0},				//24
	{"DEC H;", 0},				//25
	{"LD H, n; n=", 1},			//26
	{"DAA;", 0},				//27
	{"JR Z, n; n=", 1},			//28
	{"ADD HL, HL;", 0},			//29
	{"LD A, (HL+);", 0},		//2A
	{"DEC HL;", 0},				//2B
	{"INC L;", 0},				//2C
	{"DEC L;", 0},				//2D
	{"LD L, n; n=", 1},			//2E
	{"CPL;", 0},				//2F
	{"JR NC, n; n=", 1},		//30
	{"LD SP, nn; nn=", 2},		//31
	{"LD (HL-), A;", 0},		//32
	{"INC SP;", 0},				//33
	{"INC (HL);", 0},			//34
	{"DEC (HL);", 0},			//35
	{"LD (HL), n; n=", 1},		//36
	{"SCF;", 0},				//37
	{"JR C, n; n=", 1},			//38
	{"ADD HL, SP;", 0},			//39
	{"LD A, (HL-);", 0},		//3A
	{"DEC SP;", 0},				//3B
	{"INC A;", 0},				//3C
	{"DEC A;", 0},				//3D
	{"LD A, n; n=", 1},			//3E
	{"CCF;", 0},				//3F
	{"LD B, B;", 0},			//40
	{"LD B, C;", 0},			//41
	{"LD B, D;", 0},			//42
	{"LD B, E;", 0},			//43
	{"LD B, H;", 0},			//44
	{"LD B, L;", 0},			//45
	{"LD B, (HL);", 0},			//46
	{"LD B, A;", 0},			//47
	{"LD C, B;", 0},			//48
	{"LD C, C;", 0},			//49
	{"LD C, D;", 0},			//4A
	{"LD C, E;", 0},			//4B
	{"LD C, H;", 0},			//4C
	{"LD C, L;", 0},			//4D
	{"LD C, (HL);", 0},			//4E
	{"LD C, A;", 0},			//4F
	{"LD D, B;", 0},			//50
	{"LD D, C;", 0},			//51
	{"LD D, D;", 0},			//52
	{"LD D, E;", 0},			//53
	{"LD D, H;", 0},			//54
	{"LD D, L;", 0},			//55
	{"LD D, (HL);", 0},			//56
	{"LD D, A;", 0},			//57
	{"LD E, B;", 0},			//58
	{"LD E, C;", 0},			//59
	{"LD E, D;", 0},			//5A
	{"LD E, E;", 0},			//5B
	{"LD E, H;", 0},			//5C
	{"LD E, L;", 0},			//5D
	{"LD E, (HL);", 0},			//5E
	{"LD E, A;", 0},			//5F
	{"LD H, B;", 0},			//60
	{"LD H, C;", 0},			//61
	{"LD H, D;", 0},			//62
	{"LD H, E;", 0},			//63
	{"LD H, H;", 0},			//64
	{"LD H, L;", 0},			//65
	{"LD H, (HL);", 0},			//66
	{"LD H, A;", 0},			//67
	{"LD L, B;", 0},			//68
	{"LD L, C;", 0},			//69
	{"LD L, D;", 0},			//6A
	{"LD L, E;", 0},			//6B
	{"LD L, H;", 0},			//6C
	{"LD L, L;", 0},			//6D
	{"LD L, (HL);", 0},			//6E
	{"LD L, A;", 0},			//6F
	{"LD (HL), B;", 0},			//70
	{"LD (HL), C;", 0},			//71
	{"LD (HL), D;", 0},			//72
	{"LD (HL), E;", 0},			//73
	{"LD (HL), H;", 0},			//74
	{"LD (HL), L;", 0},			//75
	{"HALT;", 0},				//76
	{"LD (HL), A;", 0},			//77
	{"LD A, B;", 0},			//78
	{"LD A, C;", 0},			//79
	{"LD A, D;", 0},			//7A
	{"LD A, E;", 0},			//7B
	{"LD A, H;", 0},			//7C
	{"LD A, L;", 0},			//7D
	{"LD A, (HL);", 0},			//7E
	{"LD A, A;", 0},			//7F
	{"ADD B;", 0},				//80
	{"ADD C;", 0},				//81
	{"ADD D;", 0},				//82
	{"ADD E;", 0},				//83
	{"ADD H;", 0},				//84
	{"ADD L;", 0},				//85
	{"ADD (HL);", 0},			//86
	{"ADD A;", 0},				//87
	{"ADC B;", 0},				//88
	{"ADC C;", 0},				//89
	{"ADC D;", 0},				//8A
	{"ADC E;", 0},				//8B
	{"ADC H;", 0},				//8C
	{"ADC L;", 0},				//8D
	{"ADC (HL);", 0},			//8E
	{"ADC A;", 0},				//8F
	{"SUB B;", 0},				//90
	{"SUB C;", 0},				//91
	{"SUB D;", 0},				//92
	{"SUB E;", 0},				//93
	{"SUB H;", 0},				//94
	{"SUB L;", 0},				//95
	{"SUB (HL);", 0},			//96
	{"SUB A;", 0},				//97
	{"SBC B;", 0},				//98
	{"SBC C;", 0},				//99
	{"SBC D;", 0},				//9A
	{"SBC E;", 0},				//9B
	{"SBC H;", 0},				//9C
	{"SBC L;", 0},				//9D
	{"SBC (HL);", 0},			//9E
	{"SBC A;", 0},				//9F
	{"AND B;", 0},				//A0
	{"AND C;", 0},				//A1
	{"AND D;", 0},				//A2
	{"AND E;", 0,},				//A3
	{"AND H;", 0,},				//A4
	{"AND L;", 0,},				//A5
	{"AND (HL);", 0L},			//A6
	{"AND A;", 0},				//A7
	{"XOR B;", 0},				//A8
	{"XOR C;", 0},				//A9
	{"XOR D;", 0},				//AA
	{"XOR E;", 0},				//AB
	{"XOR H;", 0},				//AC
	{"XOR L;", 0},				//AD
	{"XOR (HL);", 0},			//AE
	{"XOR A;", 0},				//AF
	{"OR B;", 0},				//B0
	{"OR C;", 0},				//B1
	{"OR D;", 0},				//B2
	{"OR E;", 0},				//B3
	{"OR H;", 0},				//B4
	{"OR L;", 0},				//B5
	{"OR (HL);", 0},			//B6
	{"OR A;", 0},				//B7
	{"CP B;", 0},				//B8
	{"CP C;", 0},				//B9
	{"CP D;", 0},				//BA
	{"CP E;", 0},				//BB
	{"CP H;", 0},				//BC
	{"CP L;", 0},				//BD
	{"CP (HL);", 0},			//BE
	{"CP A;", 0},				//BF
	{"RET NZ;", 0},				//C0
	{"POP BC;", 0},				//C1
	{"JP NZ, nn; nn=", 2},		//C2
	{"JP nn; nn=", 2},			//C3
	{"CALL NZ, nn; nn=", 2},	//C4
	{"PUSH BC;", 0},			//C5
	{"ADD n; n=", 1},			//C6
	{"RST 0x00;", 0},			//C7
	{"RET Z;", 0},				//C8
	{"RET;", 0},				//C9
	{"JP Z, nn; nn=", 2},		//CA
	{"CB n; n=", 1},			//CB
	{"CALL Z, nn; nn=", 2},		//CC
	{"CALL nn; nn=", 2},		//CD
	{"ADC n; n=", 1},			//CE
	{"RST 0x08;", 0},			//CF
	{"RET NC;", 0},				//D0
	{"POP DE;", 0},				//D1
	{"JP NC, nn; nn=", 2},		//D2
	{"Undefined OP;", 0},		//D3
	{"CALL NC, nn; nn=", 2},	//D4
	{"PUSH DE;", 0},			//D5
	{"SUB n; n=", 1},			//D6
	{"RST 0x00;", 0},			//D7
	{"RET C;", 0},				//D8
	{"RETI;", 0},				//D9
	{"JP C, nn; nn=", 2},		//DA
	{"Undefined OP;", 0},		//DB
	{"CALL C, nn; nn=", 2},		//DC
	{"Undefined OP;", 0},		//DD
	{"SBC n; n=", 1},			//DE
	{"RST 0x08;", 0},			//DF
	{"LD (FF00+n), A; n=", 1},	//E0
	{"POP HL;", 0},				//E1
	{"LD (FF00+C), A;", 0},		//E2
	{"Undefined OP;", 0},		//E3
	{"Undefined OP;", 0},		//E4
	{"PUSH HL;", 0},			//E5
	{"AND n; n=", 1},			//E6
	{"RST 0x10;", 0},			//E7
	{"ADD SP, n; n=", 1},		//E8
	{"JP HL;", 0},				//E9
	{"LD (nn), A; nn=", 2},		//EA
	{"Undefined OP;", 0},		//EB
	{"Undefined OP;", 0},		//EC
	{"Undefined OP;", 0},		//ED
	{"XOR n; n=", 1},			//EE
	{"RST 0x18;", 0},			//EF
	{"LD A, (FF00+n); n=", 1},	//F0
	{"POP AF;", 0},				//F1
	{"LD A, (FF00+C);", 0},		//F2
	{"DI;", 0},					//F3
	{"Undefined OP;", 0},		//F4
	{"PUSH AF;", 0},			//F5
	{"OR n; n=", 1},			//F6
	{"RST 0x20;", 0},			//F7
	{"LD HL, SP+n; n=", 1},		//F8
	{"LD SP, HL;", 0},			//F9
	{"LD A, (nn); nn=", 2},		//FA
	{"EI;", 0},					//FB
	{"Undefined OP;", 0},		//FC
	{"Undefined OP;", 0},		//FD
	{"CP n; n=", 1},			//FE
	{"RST 0x28;", 0},			//FF
};

CB_Opcode Dissassembler::cb_opcodeTable[256] = {
	{"RLC B;"},			//00
	{"RLC C;"},			//01
	{"RLC D;"},			//02
	{"RLC E;"},			//03
	{"RLC H;"},			//04
	{"RLC L;"},			//05
	{"RLC (HL);"},		//06
	{"RLC A;"},			//07
	{"RRC B;"},			//08
	{"RRC C;"},			//09
	{"RRC D;"},			//0A
	{"RRC E;"},			//0B
	{"RRC H;"},			//0C
	{"RRC L;"},			//0D
	{"RRC (HL);"},		//0E
	{"RRC A;"},			//0F
	{"RL B;"},			//10
	{"RL C;"},			//11
	{"RL D;"},			//12
	{"RL E;"},			//13
	{"RL H;"},			//14
	{"RL L;"},			//15
	{"RL (HL);"},		//16
	{"RL A;"},			//17
	{"RR B;"},			//18
	{"RR C;"},			//19
	{"RR D;"},			//1A
	{"RR E;"},			//1B
	{"RR H;"},			//1C
	{"RR L;"},			//1D
	{"RR (HL);"},		//1E
	{"RR A;"},			//1F
	{"SLA B;"},			//20
	{"SLA C;"},			//21
	{"SLA D;"},			//22
	{"SLA E;"},			//23
	{"SLA H;"},			//24
	{"SLA L;"},			//25
	{"SLA (HL);"},		//26
	{"SLA A;"},			//27
	{"SRA B;"},			//28
	{"SRA C;"},			//29
	{"SRA D;"},			//2A
	{"SRA E;"},			//2B
	{"SRA H;"},			//2C
	{"SRA L;"},			//2D
	{"SRA (HL);"},		//2E
	{"SRA A;"},			//2F
	{"SWAP B;"},		//30
	{"SWAP C;"},		//31
	{"SWAP D;"},		//32
	{"SWAP E;"},		//33
	{"SWAP H;"},		//34
	{"SWAP L;"},		//35
	{"SWAP (HL);"},		//36
	{"SWAP A;"},		//37
	{"SRL B;"},			//38
	{"SRL C;"},			//39
	{"SRL D;"},			//3A
	{"SRL E;"},			//3B
	{"SRL H;"},			//3C
	{"SRL L;"},			//3D
	{"SRL (HL);"},		//3E
	{"SRL A;"},			//3F
	{"BIT 0, B;"},		//40
	{"BIT 0, C;"},		//41
	{"BIT 0, D;"},		//42
	{"BIT 0, E;"},		//43
	{"BIT 0, H;"},		//44
	{"BIT 0, L;"},		//45
	{"BIT 0, (HL);"},	//46
	{"BIT 0, A;"},		//47
	{"BIT 1, B;"},		//48
	{"BIT 1, C;"},		//49
	{"BIT 1, D;"},		//4A
	{"BIT 1, E;"},		//4B
	{"BIT 1, H;"},		//4C
	{"BIT 1, L;"},		//4D
	{"BIT 1, (HL);"},	//4E
	{"BIT 1, A;"},		//4F
	{"BIT 2, B;"},		//50
	{"BIT 2, C;"},		//51
	{"BIT 2, D;"},		//52
	{"BIT 2, E;"},		//53
	{"BIT 2, H;"},		//54
	{"BIT 2, L;"},		//55
	{"BIT 2, (HL);"},	//56
	{"BIT 2, A;"},		//57
	{"BIT 3, B;"},		//58
	{"BIT 3, C;"},		//59
	{"BIT 3, D;"},		//5A
	{"BIT 3, E;"},		//5B
	{"BIT 3, H;"},		//5C
	{"BIT 3, L;"},		//5D
	{"BIT 3, (HL);"},	//5E
	{"BIT 3, A;"},		//5F
	{"BIT 4, B;"},		//60
	{"BIT 4, C;"},		//61
	{"BIT 4, D;"},		//62
	{"BIT 4, E;"},		//63
	{"BIT 4, H;"},		//64
	{"BIT 4, L;"},		//65
	{"BIT 4, (HL);"},	//66
	{"BIT 4, A;"},		//67
	{"BIT 5, B;"},		//68
	{"BIT 5, C;"},		//69
	{"BIT 5, D;"},		//6A
	{"BIT 5, E;"},		//6B
	{"BIT 5, H;"},		//6C
	{"BIT 5, L;"},		//6D
	{"BIT 5, (HL);"},	//6E
	{"BIT 5, A;"},		//6F
	{"BIT 6, B;"},		//70
	{"BIT 6, C;"},		//71
	{"BIT 6, D;"},		//72
	{"BIT 6, E;"},		//73
	{"BIT 6, H;"},		//74
	{"BIT 6, L;"},		//75
	{"BIT 6, (HL);"},	//76
	{"BIT 6, A;"},		//77
	{"BIT 7, B;"},		//78
	{"BIT 7, C;"},		//79
	{"BIT 7, D;"},		//7A
	{"BIT 7, E;"},		//7B
	{"BIT 7, H;"},		//7C
	{"BIT 7, L;"},		//7D
	{"BIT 7, (HL);"},	//7E
	{"BIT 7, A;"},		//7F
	{"RES 0, B;"},		//80
	{"RES 0, C;"},		//81
	{"RES 0, D;"},		//82
	{"RES 0, E;"},		//83
	{"RES 0, H;"},		//84
	{"RES 0, L;"},		//85
	{"RES 0, (HL);"},	//86
	{"RES 0, A;"},		//87
	{"RES 1, B;"},		//88
	{"RES 1, C;"},		//89
	{"RES 1, D;"},		//8A
	{"RES 1, E;"},		//8B
	{"RES 1, H;"},		//8C
	{"RES 1, L;"},		//8D
	{"RES 1, (HL);"},	//8E
	{"RES 1, A;"},		//8F
	{"RES 2, B;"},		//90
	{"RES 2, C;"},		//91
	{"RES 2, D;"},		//92
	{"RES 2, E;"},		//93
	{"RES 2, H;"},		//94
	{"RES 2, L;"},		//95
	{"RES 2, (HL);"},	//96
	{"RES 2, A;"},		//97
	{"RES 3, B;"},		//98
	{"RES 3, C;"},		//99
	{"RES 3, D;"},		//9A
	{"RES 3, E;"},		//9B
	{"RES 3, H;"},		//9C
	{"RES 3, L;"},		//9D
	{"RES 3, (HL);"},	//9E
	{"RES 3, A;"},		//9F
	{"RES 4, B;"},		//A0
	{"RES 4, C;"},		//A1
	{"RES 4, D;"},		//A2
	{"RES 4, E;"},		//A3
	{"RES 4, H;"},		//A4
	{"RES 4, L;"},		//A5
	{"RES 4, (HL);"},	//A6
	{"RES 4, A;"},		//A7
	{"RES 5, B;"},		//A8
	{"RES 5, C;"},		//A9
	{"RES 5, D;"},		//AA
	{"RES 5, E;"},		//AB
	{"RES 5, H;"},		//AC
	{"RES 5, L;"},		//AD
	{"RES 5, (HL);"},	//AE
	{"RES 5, A;"},		//AF
	{"RES 6, B;"},		//B0
	{"RES 6, C;"},		//B1
	{"RES 6, D;"},		//B2
	{"RES 6, E;"},		//B3
	{"RES 6, H;"},		//B4
	{"RES 6, L;"},		//B5
	{"RES 6, (HL);"},	//B6
	{"RES 6, A;"},		//B7
	{"RES 7, B;"},		//B8
	{"RES 7, C;"},		//B9
	{"RES 7, D;"},		//BA
	{"RES 7, E;"},		//BB
	{"RES 7, H;"},		//BC
	{"RES 7, L;"},		//BD
	{"RES 7, (HL);"},	//BE
	{"RES 7, A;"},		//BF
	{"SET 0, B;"},		//C0
	{"SET 0, C;"},		//C1
	{"SET 0, D;"},		//C2
	{"SET 0, E;"},		//C3
	{"SET 0, H;"},		//C4
	{"SET 0, L;"},		//C5
	{"SET 0, (HL);"},	//C6
	{"SET 0, A;"},		//C7
	{"SET 1, B;"},		//C8
	{"SET 1, C;"},		//C9
	{"SET 1, D;"},		//CA
	{"SET 1, E;"},		//CB
	{"SET 1, H;"},		//CC
	{"SET 1, L;"},		//CD
	{"SET 1, (HL);"},	//CE
	{"SET 1, A;"},		//CF
	{"SET 2, B;"},		//D0
	{"SET 2, C;"},		//D1
	{"SET 2, D;"},		//D2
	{"SET 2, E;"},		//D3
	{"SET 2, H;"},		//D4
	{"SET 2, L;"},		//D5
	{"SET 2, (HL);"},	//D6
	{"SET 2, A;"},		//D7
	{"SET 3, B;"},		//D8
	{"SET 3, C;"},		//D9
	{"SET 3, D;"},		//DA
	{"SET 3, E;"},		//DB
	{"SET 3, H;"},		//DC
	{"SET 3, L;"},		//DD
	{"SET 3, (HL);"},	//DE
	{"SET 3, A;"},		//DF
	{"SET 4, B;"},		//E0
	{"SET 4, C;"},		//E1
	{"SET 4, D;"},		//E2
	{"SET 4, E;"},		//E3
	{"SET 4, H;"},		//E4
	{"SET 4, L;"},		//E5
	{"SET 4, (HL);"},	//E6
	{"SET 4, A;"},		//E7
	{"SET 5, B;"},		//E8
	{"SET 5, C;"},		//E9
	{"SET 5, D;"},		//EA
	{"SET 5, E;"},		//EB
	{"SET 5, H;"},		//EC
	{"SET 5, L;"},		//ED
	{"SET 5, (HL);"},	//EE
	{"SET 5, A;"},		//EF
	{"SET 6, B;"},		//F0
	{"SET 6, C;"},		//F1
	{"SET 6, D;"},		//F2
	{"SET 6, E;"},		//F3
	{"SET 6, H;"},		//F4
	{"SET 6, L;"},		//F5
	{"SET 6, (HL);"},	//F6
	{"SET 6, A;"},		//F7
	{"SET 7, B;"},		//F8
	{"SET 7, C;"},		//F9
	{"SET 7, D;"},		//FA
	{"SET 7, E;"},		//FB
	{"SET 7, H;"},		//FC
	{"SET 7, L;"},		//FD
	{"SET 7, (HL);"},	//FE
	{"SET 7, A;"},		//FF
};

Dissassembler::Dissassembler()
{
	PC = 0;
	cart = new Cartridge();
}

Dissassembler::~Dissassembler()
{
	delete cart;
}

void Dissassembler::LoadCartridge(const char* filename)
{
	cart->open(filename);
}

void Dissassembler::Disassemble()
{
	while (PC < ROM_MAX_SIZE)
	{
		StoreNextInstruction();
	}
}

inline uint8_t Dissassembler::fetch()
{
	return cart->read(PC++);
}

void Dissassembler::StoreNextInstruction() {
	uint16_t address = PC;
	uint8_t opcode = fetch();
	uint8_t operand_bytes = opcodeTable[opcode].operand_bytes;
	uint32_t operand = NO_OPERAND;

	if (operand_bytes == 1)
	{
		operand = fetch();
	}
	else if (operand_bytes == 2)
	{
		operand = fetch() | (fetch() << 8);
	}

	Instruction ins;
	ins.address = address;
	ins.mnemonic = opcodeTable[opcode].mnemonic;
	ins.operand = operand;

	instructions.push_back(ins);
}