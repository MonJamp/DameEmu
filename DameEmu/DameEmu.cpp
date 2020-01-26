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

	switch (opcode) {
	case 0x01: LD_BC(nn); break;
	case 0x02: LD_BC_A(); break;
	case 0x06: LD_r_n(B, n); break;
	case 0x0E: LD_r_n(C, n); break;
	case 0x11: LD_DE(nn); break;
	case 0x12: LD_DE_A(); break;
	case 0x16: LD_r_n(D, n); break;
	case 0x1E: LD_r_n(E, n); break;
	case 0x20: JR_NZ(e); break;
	case 0x21: LD_HL(nn); break;
	case 0x22: LD_HLI_A(); break;
	case 0x26: LD_r_n(H, n); break;
	case 0x2E: LD_r_n(L, n); break;
	case 0x31: LD_SP(nn); break;
	case 0x32: LD_HLD_A(); break;
	case 0x36: LD_HL_n(n); break;
	case 0x3E: LD_r_n(A, n); break;
	case 0x40: LD_r_r(B, B); break;
	case 0x41: LD_r_r(B, C); break;
	case 0x42: LD_r_r(B, D); break;
	case 0x43: LD_r_r(B, E); break;
	case 0x44: LD_r_r(B, H); break;
	case 0x45: LD_r_r(B, L); break;
	case 0x46: LD_r_HL(B); break;
	case 0x47: LD_r_r(B, A); break;
	case 0x48: LD_r_r(C, B); break;
	case 0x49: LD_r_r(C, C); break;
	case 0x4A: LD_r_r(C, D); break;
	case 0x4B: LD_r_r(C, E); break;
	case 0x4C: LD_r_r(C, H); break;
	case 0x4D: LD_r_r(C, L); break;
	case 0x4E: LD_r_HL(C); break;
	case 0x4F: LD_r_r(C, A); break;
	case 0x50: LD_r_r(D, B); break;
	case 0x51: LD_r_r(D, C); break;
	case 0x52: LD_r_r(D, D); break;
	case 0x53: LD_r_r(D, E); break;
	case 0x54: LD_r_r(D, H); break;
	case 0x55: LD_r_r(D, L); break;
	case 0x56: LD_r_HL(D); break;
	case 0x57: LD_r_r(D, A); break;
	case 0x58: LD_r_r(E, B); break;
	case 0x59: LD_r_r(E, C); break;
	case 0x5A: LD_r_r(E, D); break;
	case 0x5B: LD_r_r(E, E); break;
	case 0x5C: LD_r_r(E, H); break;
	case 0x5D: LD_r_r(E, L); break;
	case 0x5E: LD_r_HL(E); break;
	case 0x5F: LD_r_r(E, A); break;
	case 0x60: LD_r_r(H, B); break;
	case 0x61: LD_r_r(H, C); break;
	case 0x62: LD_r_r(H, D); break;
	case 0x63: LD_r_r(H, E); break;
	case 0x64: LD_r_r(H, H); break;
	case 0x65: LD_r_r(H, L); break;
	case 0x66: LD_r_HL(H); break;
	case 0x67: LD_r_r(H, A); break;
	case 0x68: LD_r_r(L, B); break;
	case 0x69: LD_r_r(L, C); break;
	case 0x6A: LD_r_r(L, D); break;
	case 0x6B: LD_r_r(L, E); break;
	case 0x6C: LD_r_r(L, H); break;
	case 0x6D: LD_r_r(L, L); break;
	case 0x6E: LD_r_HL(L); break;
	case 0x6F: LD_r_r(L, A); break;
	case 0x70: LD_HL_r(B); break;
	case 0x71: LD_HL_r(C); break;
	case 0x72: LD_HL_r(D); break;
	case 0x73: LD_HL_r(E); break;
	case 0x74: LD_HL_r(H); break;
	case 0x75: LD_HL_r(L); break;
	case 0x76: LD_HL_r(A); break;
	case 0x77: LD_r_r(A, B); break;
	case 0x78: LD_r_r(A, C); break;
	case 0x79: LD_r_r(A, D); break;
	case 0x7A: LD_r_r(A, E); break;
	case 0x7B: LD_r_r(A, H); break;
	case 0x7C: LD_r_r(A, L); break;
	case 0x7D: LD_r_HL(A); break;
	case 0x7E: LD_r_r(A, A); break;
	case 0xA0: AND_B(); break;
	case 0xA1: AND_C(); break;
	case 0xA2: AND_D(); break;
	case 0xA3: AND_E(); break;
	case 0xA4: AND_H(); break;
	case 0xA5: AND_L(); break;
	case 0xA6: AND_HL(); break;
	case 0xA7: AND_A(); break;
	case 0xA8: XOR_B(); break;
	case 0xA9: XOR_C(); break;
	case 0xAA: XOR_D(); break;
	case 0xAB: XOR_E(); break;
	case 0xAC: XOR_H(); break;
	case 0xAD: XOR_L(); break;
	case 0xAE: XOR_HL(); break;
	case 0xAF: XOR_A(); break;
	case 0xCB: return CB(n);
	case 0xE2: LD_C_A(); break;
	case 0xF2: LD_A_C(); break;
	default:
		UNKNOWN(opcode);
		return HALT;
	}

	return OK;
}

EmuStatus DameEmu::CB(uint8_t opcode) {
	switch (opcode) {
	case 0x40: BIT(0, B); break;
	case 0x41: BIT(0, C); break;
	case 0x42: BIT(0, D); break;
	case 0x43: BIT(0, E); break;
	case 0x44: BIT(0, H); break;
	case 0x45: BIT(0, L); break;
	case 0x46: BIT_HL(0); break;
	case 0x47: BIT(0, A); break;
	case 0x48: BIT(1, B); break;
	case 0x49: BIT(1, C); break;
	case 0x4A: BIT(1, D); break;
	case 0x4B: BIT(1, E); break;
	case 0x4C: BIT(1, H); break;
	case 0x4D: BIT(1, L); break;
	case 0x4E: BIT_HL(1); break;
	case 0x4F: BIT(1, A); break;
	case 0x50: BIT(2, B); break;
	case 0x51: BIT(2, C); break;
	case 0x52: BIT(2, D); break;
	case 0x53: BIT(2, E); break;
	case 0x54: BIT(2, H); break;
	case 0x55: BIT(2, L); break;
	case 0x56: BIT_HL(2); break;
	case 0x57: BIT(2, A); break;
	case 0x58: BIT(3, B); break;
	case 0x59: BIT(3, C); break;
	case 0x5A: BIT(3, D); break;
	case 0x5B: BIT(3, E); break;
	case 0x5C: BIT(3, H); break;
	case 0x5D: BIT(3, L); break;
	case 0x5E: BIT_HL(3); break;
	case 0x5F: BIT(3, A); break;
	case 0x60: BIT(4, B); break;
	case 0x61: BIT(4, C); break;
	case 0x62: BIT(4, D); break;
	case 0x63: BIT(4, E); break;
	case 0x64: BIT(4, H); break;
	case 0x65: BIT(4, L); break;
	case 0x66: BIT_HL(4); break;
	case 0x67: BIT(4, A); break;
	case 0x68: BIT(5, B); break;
	case 0x69: BIT(5, C); break;
	case 0x6A: BIT(5, D); break;
	case 0x6B: BIT(5, E); break;
	case 0x6C: BIT(5, H); break;
	case 0x6D: BIT(5, L); break;
	case 0x6E: BIT_HL(5); break;
	case 0x6F: BIT(5, A); break;
	case 0x70: BIT(6, B); break;
	case 0x71: BIT(6, C); break;
	case 0x72: BIT(6, D); break;
	case 0x73: BIT(6, E); break;
	case 0x74: BIT(6, H); break;
	case 0x75: BIT(6, L); break;
	case 0x76: BIT_HL(6); break;
	case 0x77: BIT(6, A); break;
	case 0x78: BIT(7, B); break;
	case 0x79: BIT(7, C); break;
	case 0x7A: BIT(7, D); break;
	case 0x7B: BIT(7, E); break;
	case 0x7C: BIT(7, H); break;
	case 0x7D: BIT(7, L); break;
	case 0x7E: BIT_HL(7); break;
	case 0x7F: BIT(7, A); break;
	default:
		UNKNOWN_CB(opcode);
		return HALT;
	}

	return OK;
}