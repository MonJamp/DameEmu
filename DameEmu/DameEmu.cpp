#include "DameEmu.h"
#include <fstream>

#ifdef _DEBUG
#include <cstdio>
#define debug_msg(...) printf(__VA_ARGS__)
#else
#define debug_msg(x)
#endif

#define nn (uint16_t)(memory[PC + 2] << 8 | memory[PC + 1])
#define n (uint8_t)(memory[PC + 1])
#define e (int8_t)(memory[PC + 1])

DameEmu::DameEmu(const char* ROM_DIR) {
	BootUp(ROM_DIR);

	status = EmuStatus::OK;
}

DameEmu::~DameEmu() {
	if (app != NULL) {
		app->close();
	}

	delete(app);
}

void DameEmu::BootUp(const char* ROM_DIR) {
	//Load cartridge
	std::ifstream ROM;
	ROM.open(ROM_DIR, std::ios::binary);
	for (uint16_t i = 0; ROM.get((char&)memory[i]); i++);

	//Gameboy boot state after bios
	//These values are true only for DMG
	//TODO: Display scrolling logo and boot sound

	//Clear VRAM
	for (uint16_t i = 0x0; i < sizeof(VRAM); i++) {
		VRAM[i] = 0x0;
	}

	AF = 0x01B0;
	BC = 0x0013;
	DE = 0x00D8;
	SP = 0xFFFE;
	PC = 0x0100;

	TIMA = 0x00;
	TMA = 0x00;
	TAC = 0x00;
	NR10 = 0x80;
	NR11 = 0xBF;
	NR12 = 0xF3;
	NR14 = 0xBF;
	NR21 = 0x3F;
	NR22 = 0x00;
	NR24 = 0xBF;
	NR30 = 0x7F;
	NR31 = 0xFF;
	NR32 = 0x9F;
	NR33 = 0xBF;
	NR41 = 0xFF;
	NR42 = 0x00;
	NR43 = 0x00;
	NR44 = 0xBF;
	NR50 = 0x77;
	NR51 = 0xF3;
	NR52 = 0xF1;
	LCDC = 0x91;
	SCY = 0x00;
	SCX = 0x00;
	LYC = 0x00;
	BGP = 0xFC;
	OBP0 = 0xFF;
	OBP1 = 0xFF;
	WY = 0x00;
	WX = 0x00;
	InterruptEnable = 0x00;
}

EmuStatus DameEmu::Cycle() {
	uint8_t opcode = memory[PC];
	Instruction ins = instructions[opcode];

	debug_msg("%04X: ", PC);

	switch (ins.length) {
	case 1:
		debug_msg(ins.mnemonic);
		IR = opcode;
		PC += 1;
		break;
	case 2:
		debug_msg(ins.mnemonic, n);
		IR = (opcode << 8) | n;
		PC += 2;
		break;
	case 3:
		debug_msg(ins.mnemonic, nn);
		IR = (opcode << 16) | nn;
		PC += 3;
		break;
	default:
		break;
	};

	(this->*ins.execute)();
	printf("\n");
	
	return status;
}

void DameEmu::CB() {
	uint8_t opcode = (uint8_t)(IR & 0xFF);
	Instruction cb_ins = cb_instructions[opcode];

	debug_msg(cb_ins.mnemonic);
	(this->*cb_ins.execute)();
}