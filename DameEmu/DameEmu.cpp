#include "DameEmu.h"
#include <fstream>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#define debug_msg(x) cout << x
#else
#define debug_msg(x)
#endif

DameEmu::DameEmu(const char* ROM_DIR, const char* BIOS_DIR) {
	BootUp(ROM_DIR, BIOS_DIR);
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
	const unsigned char hi = (opcode & 0xF0) >> 4;
	const unsigned char lo = (opcode & 0x0F);
	uint16_t nn = memory[PC + 2] << 8 | memory[PC + 1];

	debug_msg(std::hex << PC << ": ");

	switch (hi) {
	case 0x0:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x1:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x2:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x3:
		switch (lo) {
		case 0x1: LDSP(nn); break;
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x4:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x5:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x6:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x7:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x8:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x9:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0xA:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0xB:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0xC:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0xD:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0xE:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0xF:
		switch (lo) {
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	default:
		UNKNOWN(opcode);
		return HALT;
	}

	return OK;
}