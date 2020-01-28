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

DameEmu::DameEmu(const char* ROM_DIR, const char* BIOS_DIR) {
	BootUp(ROM_DIR, BIOS_DIR);

	status = OK;
}

DameEmu::~DameEmu() {
	if (app != NULL) {
		app->close();
	}

	delete(app);
}

void DameEmu::BootUp(const char* ROM_DIR, const char* BIOS_DIR) {
	std::ifstream BIOS;
	BIOS.open(BIOS_DIR, std::ios::binary);
	for (uint16_t i = 0x0; BIOS.get((char&)memory[i]); i++);

	std::ifstream ROM;
	ROM.open(ROM_DIR, std::ios::binary);
	for (uint16_t i = ROM_ENTRY; ROM.get((char&)memory[i]); i++);

	PC = 0;
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