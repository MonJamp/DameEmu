#include "DameEmu.h"
#include <fstream>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#define debug_msg(x) cout << x
#else
#define debug_msg(x)
#endif

#define hi (opcode & 0xF0) >> 4
#define lo opcode & 0x0F
#define nn (uint16_t)(memory[PC + 2] << 8 | memory[PC + 1])
#define n (uint8_t)(memory[PC + 1])
#define e (int8_t)(memory[PC + 1])

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

	debug_msg(std::hex << PC << ": ");

	switch (hi) {
	case 0x0:
		switch (lo) {
		case 0x1: LD_BC(nn); break;
		case 0x2: LD_BC_A(); break;
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x1:
		switch (lo) {
		case 0x1: LD_DE(nn); break;
		case 0x2: LD_DE_A(); break;
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x2:
		switch (lo) {
		case 0x0: JR_NZ(e); break;
		case 0x1: LD_HL(nn); break;
		case 0x2: LD_HLI_A(); break;
		default:
			UNKNOWN(opcode);
			return HALT;
		} break;
	case 0x3:
		switch (lo) {
		case 0x1: LD_SP(nn); break;
		case 0x2: LD_HLD_A(); break;
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
		case 0x0: AND_B(); break;
		case 0x1: AND_C(); break;
		case 0x2: AND_D(); break;
		case 0x3: AND_E(); break;
		case 0x4: AND_H(); break;
		case 0x5: AND_L(); break;
		case 0x6: AND_HL(); break;
		case 0x7: AND_A(); break;
		case 0x8: XOR_B(); break;
		case 0x9: XOR_C(); break;
		case 0xA: XOR_D(); break;
		case 0xB: XOR_E(); break;
		case 0xC: XOR_H(); break;
		case 0xD: XOR_L(); break;
		case 0xE: XOR_HL(); break;
		case 0xF: XOR_A(); break;
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
		case 0xB: return CB(n);
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

EmuStatus DameEmu::CB(uint8_t opcode) {
	switch (hi) {
	case 0x0:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x1:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x2:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x3:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x4:
		switch (lo) {
		case 0x0: BIT(0, B); break;
		case 0x1: BIT(0, C); break;
		case 0x2: BIT(0, D); break;
		case 0x3: BIT(0, E); break;
		case 0x4: BIT(0, H); break;
		case 0x5: BIT(0, L); break;
		case 0x6: BIT_HL(0); break;
		case 0x7: BIT(0, A); break;
		case 0x8: BIT(1, B); break;
		case 0x9: BIT(1, C); break;
		case 0xA: BIT(1, D); break;
		case 0xB: BIT(1, E); break;
		case 0xC: BIT(1, H); break;
		case 0xD: BIT(1, L); break;
		case 0xE: BIT_HL(1); break;
		case 0xF: BIT(1, A); break;
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x5:
		switch (lo) {
		case 0x0: BIT(2, B); break;
		case 0x1: BIT(2, C); break;
		case 0x2: BIT(2, D); break;
		case 0x3: BIT(2, E); break;
		case 0x4: BIT(2, H); break;
		case 0x5: BIT(2, L); break;
		case 0x6: BIT_HL(2); break;
		case 0x7: BIT(2, A); break;
		case 0x8: BIT(3, B); break;
		case 0x9: BIT(3, C); break;
		case 0xA: BIT(3, D); break;
		case 0xB: BIT(3, E); break;
		case 0xC: BIT(3, H); break;
		case 0xD: BIT(3, L); break;
		case 0xE: BIT_HL(3); break;
		case 0xF: BIT(3, A); break;
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x6:
		switch (lo) {
		case 0x0: BIT(4, B); break;
		case 0x1: BIT(4, C); break;
		case 0x2: BIT(4, D); break;
		case 0x3: BIT(4, E); break;
		case 0x4: BIT(4, H); break;
		case 0x5: BIT(4, L); break;
		case 0x6: BIT_HL(4); break;
		case 0x7: BIT(4, A); break;
		case 0x8: BIT(5, B); break;
		case 0x9: BIT(5, C); break;
		case 0xA: BIT(5, D); break;
		case 0xB: BIT(5, E); break;
		case 0xC: BIT(5, H); break;
		case 0xD: BIT(5, L); break;
		case 0xE: BIT_HL(5); break;
		case 0xF: BIT(5, A); break;
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x7:
		switch (lo) {
		case 0x0: BIT(6, B); break;
		case 0x1: BIT(6, C); break;
		case 0x2: BIT(6, D); break;
		case 0x3: BIT(6, E); break;
		case 0x4: BIT(6, H); break;
		case 0x5: BIT(6, L); break;
		case 0x6: BIT_HL(6); break;
		case 0x7: BIT(6, A); break;
		case 0x8: BIT(7, B); break;
		case 0x9: BIT(7, C); break;
		case 0xA: BIT(7, D); break;
		case 0xB: BIT(7, E); break;
		case 0xC: BIT(7, H); break;
		case 0xD: BIT(7, L); break;
		case 0xE: BIT_HL(7); break;
		case 0xF: BIT(7, A); break;
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x8:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0x9:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0xA:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0xB:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0xC:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0xD:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0xE:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	case 0xF:
		switch (lo) {
		default:
			UNKNOWN_CB(opcode);
			return HALT;
		} break;
	default:
		UNKNOWN_CB(opcode);
		return HALT;
	}

	return OK;
}