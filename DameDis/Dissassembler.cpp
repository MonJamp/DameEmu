#include "Dissassembler.h"
#include <sstream>
#include <fstream>
#include <iostream>


#define nn (uint16_t)(memory[PC + 2] << 8 | memory[PC + 1])
#define n (uint16_t)(memory[PC + 1])


Instruction Dissassembler::instructions[256] = {
	{"NOP;", 1},				//00
	{"LD BC, nn; nn=", 3},		//01
	{"LD (BC), A;", 1},			//02
	{"INC BC;", 1},				//03
	{"INC B;", 1},				//04
	{"DEC B;", 1},				//05
	{"LD B, n; n=", 2},			//06
	{"RLCA;", 1},				//07
	{"LD (nn), SP; nn=", 3},	//08
	{"ADD HL, BC;", 1},			//09
	{"LD A, (BC);", 1},			//0A
	{"DEC BC;", 1},				//0B
	{"INC C;", 1},				//0C
	{"DEC C;", 1},				//0D
	{"LD C, n; n=", 2},			//0E
	{"RRCA;", 1},				//0F
	{"STOP;", 1},				//10
	{"LD DE, nn; nn=", 3},		//11
	{"LD (DE), A;", 1},			//12
	{"INC DE;", 1},				//13
	{"INC D;", 1},				//14
	{"DEC D;", 1},				//15
	{"LD D, n; n=", 2},			//16
	{"RLA;", 1},				//17
	{"JR n; n=", 2},			//18
	{"ADD HL, DE;", 1},			//19
	{"LD A, (DE);", 1},			//1A
	{"DEC DE;", 1},				//1B
	{"INC E;", 1},				//1C
	{"DEC E;", 1},				//1D
	{"LD E, n; n=", 2},			//1E
	{"RRA;", 1},				//1F
	{"JR NZ, n; n=", 2},		//20
	{"LD HL, nn; nn=", 3},		//21
	{"LD (HL+), A;", 1},		//22
	{"INC HL;", 1},				//23
	{"INC H;", 1},				//24
	{"DEC H;", 1},				//25
	{"LD H, n; n=", 2},			//26
	{"DAA;", 1},				//27
	{"JR Z, n; n=", 2},			//28
	{"ADD HL, HL;", 1},			//29
	{"LD A, (HL+);", 1},		//2A
	{"DEC HL;", 1},				//2B
	{"INC L;", 1},				//2C
	{"DEC L;", 1},				//2D
	{"LD L, n; n=", 2},			//2E
	{"CPL;", 1},				//2F
	{"JR NC, n; n=", 2},		//30
	{"LD SP, nn; nn=", 3},		//31
	{"LD (HL-), A;", 1},		//32
	{"INC SP;", 1},				//33
	{"INC (HL);", 1},			//34
	{"DEC (HL);", 1},			//35
	{"LD (HL), n; n=", 2},		//36
	{"SCF;", 1},				//37
	{"JR C, n; n=", 2},			//38
	{"ADD HL, SP;", 1},			//39
	{"LD A, (HL-);", 1},		//3A
	{"DEC SP;", 1},				//3B
	{"INC A;", 1},				//3C
	{"DEC A;", 1},				//3D
	{"LD A, n; n=", 2},			//3E
	{"CCF;", 1},				//3F
	{"LD B, B;", 1},			//40
	{"LD B, C;", 1},			//41
	{"LD B, D;", 1},			//42
	{"LD B, E;", 1},			//43
	{"LD B, H;", 1},			//44
	{"LD B, L;", 1},			//45
	{"LD B, (HL);", 1},			//46
	{"LD B, A;", 1},			//47
	{"LD C, B;", 1},			//48
	{"LD C, C;", 1},			//49
	{"LD C, D;", 1},			//4A
	{"LD C, E;", 1},			//4B
	{"LD C, H;", 1},			//4C
	{"LD C, L;", 1},			//4D
	{"LD C, (HL);", 1},			//4E
	{"LD C, A;", 1},			//4F
	{"LD D, B;", 1},			//50
	{"LD D, C;", 1},			//51
	{"LD D, D;", 1},			//52
	{"LD D, E;", 1},			//53
	{"LD D, H;", 1},			//54
	{"LD D, L;", 1},			//55
	{"LD D, (HL);", 1},			//56
	{"LD D, A;", 1},			//57
	{"LD E, B;", 1},			//58
	{"LD E, C;", 1},			//59
	{"LD E, D;", 1},			//5A
	{"LD E, E;", 1},			//5B
	{"LD E, H;", 1},			//5C
	{"LD E, L;", 1},			//5D
	{"LD E, (HL);", 1},			//5E
	{"LD E, A;", 1},			//5F
	{"LD H, B;", 1},			//60
	{"LD H, C;", 1},			//61
	{"LD H, D;", 1},			//62
	{"LD H, E;", 1},			//63
	{"LD H, H;", 1},			//64
	{"LD H, L;", 1},			//65
	{"LD H, (HL);", 1},			//66
	{"LD H, A;", 1},			//67
	{"LD L, B;", 1},			//68
	{"LD L, C;", 1},			//69
	{"LD L, D;", 1},			//6A
	{"LD L, E;", 1},			//6B
	{"LD L, H;", 1},			//6C
	{"LD L, L;", 1},			//6D
	{"LD L, (HL);", 1},			//6E
	{"LD L, A;", 1},			//6F
	{"LD (HL), B;", 1},			//70
	{"LD (HL), C;", 1},			//71
	{"LD (HL), D;", 1},			//72
	{"LD (HL), E;", 1},			//73
	{"LD (HL), H;", 1},			//74
	{"LD (HL), L;", 1},			//75
	{"HALT;", 1},				//76
	{"LD (HL), A;", 1},			//77
	{"LD A, B;", 1},			//78
	{"LD A, C;", 1},			//79
	{"LD A, D;", 1},			//7A
	{"LD A, E;", 1},			//7B
	{"LD A, H;", 1},			//7C
	{"LD A, L;", 1},			//7D
	{"LD A, (HL);", 1},			//7E
	{"LD A, A;", 1},			//7F
	{"ADD B;", 1},				//80
	{"ADD C;", 1},				//81
	{"ADD D;", 1},				//82
	{"ADD E;", 1},				//83
	{"ADD H;", 1},				//84
	{"ADD L;", 1},				//85
	{"ADD (HL);", 1},			//86
	{"ADD A;", 1},				//87
	{"ADC B;", 1},				//88
	{"ADC C;", 1},				//89
	{"ADC D;", 1},				//8A
	{"ADC E;", 1},				//8B
	{"ADC H;", 1},				//8C
	{"ADC L;", 1},				//8D
	{"ADC (HL);", 1},			//8E
	{"ADC A;", 1},				//8F
	{"SUB B;", 1},				//90
	{"SUB C;", 1},				//91
	{"SUB D;", 1},				//92
	{"SUB E;", 1},				//93
	{"SUB H;", 1},				//94
	{"SUB L;", 1},				//95
	{"SUB (HL);", 1},			//96
	{"SUB A;", 1},				//97
	{"SBC B;", 1},				//98
	{"SBC C;", 1},				//99
	{"SBC D;", 1},				//9A
	{"SBC E;", 1},				//9B
	{"SBC H;", 1},				//9C
	{"SBC L;", 1},				//9D
	{"SBC (HL);", 1},			//9E
	{"SBC A;", 1},				//9F
	{"AND B;", 1},				//A0
	{"AND C;", 1},				//A1
	{"AND D;", 1},				//A2
	{"AND E;", 1,},				//A3
	{"AND H;", 1,},				//A4
	{"AND L;", 1,},				//A5
	{"AND (HL);", 1L},			//A6
	{"AND A;", 1},				//A7
	{"XOR B;", 1},				//A8
	{"XOR C;", 1},				//A9
	{"XOR D;", 1},				//AA
	{"XOR E;", 1},				//AB
	{"XOR H;", 1},				//AC
	{"XOR L;", 1},				//AD
	{"XOR (HL);", 1},			//AE
	{"XOR A;", 1},				//AF
	{"OR B;", 1},				//B0
	{"OR C;", 1},				//B1
	{"OR D;", 1},				//B2
	{"OR E;", 1},				//B3
	{"OR H;", 1},				//B4
	{"OR L;", 1},				//B5
	{"OR (HL);", 1},			//B6
	{"OR A;", 1},				//B7
	{"CP B;", 1},				//B8
	{"CP C;", 1},				//B9
	{"CP D;", 1},				//BA
	{"CP E;", 1},				//BB
	{"CP H;", 1},				//BC
	{"CP L;", 1},				//BD
	{"CP (HL);", 1},			//BE
	{"CP A;", 1},				//BF
	{"RET NZ;", 1},				//C0
	{"POP BC;", 1},				//C1
	{"JP NZ, nn; nn=", 3},		//C2
	{"JP nn; nn=", 3},			//C3
	{"CALL NZ, nn; nn=", 3},	//C4
	{"PUSH BC;", 1},			//C5
	{"ADD n; n=", 2},			//C6
	{"RST 0x00;", 1},			//C7
	{"RET Z;", 1},				//C8
	{"RET;", 1},				//C9
	{"JP Z, nn; nn=", 3},		//CA
	{"CB n; n=", 2},			//CB
	{"CALL Z, nn; nn=", 3},		//CC
	{"CALL nn; nn=", 3},		//CD
	{"ADC n; n=", 2},			//CE
	{"RST 0x08;", 1},			//CF
	{"RET NC;", 1},				//D0
	{"POP DE;", 1},				//D1
	{"JP NC, nn; nn=", 3},		//D2
	{"Undefined OP;", 1},		//D3
	{"CALL NC, nn; nn=", 3},	//D4
	{"PUSH DE;", 1},			//D5
	{"SUB n; n=", 2},			//D6
	{"RST 0x10;", 1},			//D7
	{"RET C;", 1},				//D8
	{"RETI;", 1},				//D9
	{"JP C, nn; nn=", 3},		//DA
	{"Undefined OP;", 1},		//DB
	{"CALL C, nn; nn=", 3},		//DC
	{"Undefined OP;", 1},		//DD
	{"SBC n; n=", 2},			//DE
	{"RST 0x18;", 1},			//DF
	{"LD (FF00+n), A; n=", 2},	//E0
	{"POP HL;", 1},				//E1
	{"LD (FF00+C), A;", 1},		//E2
	{"Undefined OP;", 1},		//E3
	{"Undefined OP;", 1},		//E4
	{"PUSH HL;", 1},			//E5
	{"AND n; n=", 2},			//E6
	{"RST 0x20;", 1},			//E7
	{"ADD SP, n; n=", 2},		//E8
	{"JP HL;", 1},				//E9
	{"LD (nn), A; nn=", 3},		//EA
	{"Undefined OP;", 1},		//EB
	{"Undefined OP;", 1},		//EC
	{"Undefined OP;", 1},		//ED
	{"XOR n; n=", 2},			//EE
	{"RST 0x28;", 1},			//EF
	{"LD A, (FF00+n); n=", 2},	//F0
	{"POP AF;", 1},				//F1
	{"LD A, (FF00+C);", 1},		//F2
	{"DI;", 1},					//F3
	{"Undefined OP;", 1},		//F4
	{"PUSH AF;", 1},			//F5
	{"OR n; n=", 2},			//F6
	{"RST 0x30;", 1},			//F7
	{"LD HL, SP+n; n=", 2},		//F8
	{"LD SP, HL;", 1},			//F9
	{"LD A, (nn); nn=", 3},		//FA
	{"EI;", 1},					//FB
	{"Undefined OP;", 1},		//FC
	{"Undefined OP;", 1},		//FD
	{"CP n; n=", 2},			//FE
	{"RST 0x38;", 1},			//FF
};

CB_Instruction Dissassembler::cb_instructions[256] = {
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

Dissassembler::Dissassembler(std::string ROM_DIR) {
	LoadROM(ROM_DIR);
	PC = 0;
}

void Dissassembler::LoadROM(std::string ROM_DIR) {
	std::ifstream ROM;
	ROM.open(ROM_DIR, std::ios::binary);
	
	if (ROM.fail()) {
		printf("Error! ROM could not be read!\n");
	}

	size_t fileSize;

	ROM.seekg(0, std::ios::end);
	fileSize = ROM.tellg();
	ROM.seekg(0, std::ios::beg);

	memory.resize(fileSize);
	ROM.read((char*)(&memory[0]), fileSize);

	ROM.close();
}

void Dissassembler::PrintInstructions() {
	std::cout << GetFullDissassembly();
}

std::string Dissassembler::GetFullDissassembly() {
	std::stringstream ss;

	while (PC < memory.size()) {
		ss << std::hex << std::showbase << (int)PC << ": ";
		ss << GetNextInstruction();
	}

	return ss.str();
}

std::string Dissassembler::GetNextInstruction() {
	uint8_t opcode = memory[PC];
	Instruction ins = instructions[opcode];
	std::stringstream ss;

	if (opcode != 0xCB) {
		switch (ins.length) {
		case 1:
			ss << ins.mnemonic;
			break;
		case 2:
			ss << ins.mnemonic << std::hex << std::showbase << n;
			break;
		case 3:
			ss << ins.mnemonic << std::hex << std::showbase << nn;
			break;
		default:
			ss << "Error! Invalid OP!";
			break;
		}
	}
	else {
		CB_Instruction cb_ins = cb_instructions[n];
		ss << cb_ins.mnemonic;
	}

	PC += ins.length;

	ss << std::endl;

	return ss.str();
}