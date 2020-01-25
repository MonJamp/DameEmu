#include "DameEmu.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#define debug_msg(x) cout << x
#else
#define debug_msg(x)
#endif

void DameEmu::UNKNOWN(uint8_t opcode) {
	debug_msg("Unknown OP: " << (int)opcode << endl);
}

void DameEmu::LD_BC(uint16_t nn) {
	BC = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD BC, " << nn << endl);
}

void DameEmu::LD_DE(uint16_t nn) {
	DE = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD DE, " << nn << endl);
}

void DameEmu::LD_HL(uint16_t nn) {
	HL = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD HL, " << nn << endl);
}

void DameEmu::LD_SP(uint16_t nn) {
	SP = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD SP, " << nn << endl);
}

void DameEmu::AND_B() {
	A &= B;

	CLEAR_CY;
	SET_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("AND B = " << (int)A << endl);
}

void DameEmu::AND_C() {
	A &= C;

	CLEAR_CY;
	SET_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("AND C = " << (int)A << endl);
}

void DameEmu::AND_D() {
	A &= D;

	CLEAR_CY;
	SET_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("AND D = " << (int)A << endl);
}

void DameEmu::AND_E() {
	A &= E;

	CLEAR_CY;
	SET_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("AND E = " << (int)A << endl);
}

void DameEmu::AND_H() {
	A &= H;

	CLEAR_CY;
	SET_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("AND H = " << (int)A << endl);
}

void DameEmu::AND_L() {
	A &= L;

	CLEAR_CY;
	SET_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("AND L = " << (int)A << endl);
}

void DameEmu::AND_HL() {
	A &= memory[HL];

	CLEAR_CY;
	SET_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 2;
	PC += 1;

	debug_msg("AND (HL) = " << (int)A << endl);
}

void DameEmu::AND_A() {
	A &= A;

	CLEAR_CY;
	SET_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("AND A = " << (int)A << endl);
}

void DameEmu::XOR_B() {
	A ^= B;

	CLEAR_CY;
	CLEAR_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("XOR B = " << (int)A << endl);
}

void DameEmu::XOR_C() {
	A ^= C;

	CLEAR_CY;
	CLEAR_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("XOR C = " << (int)A << endl);
}

void DameEmu::XOR_D() {
	A ^= D;

	CLEAR_CY;
	CLEAR_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("XOR D = " << (int)A << endl);
}

void DameEmu::XOR_E() {
	A ^= E;

	CLEAR_CY;
	CLEAR_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("XOR E = " << (int)A << endl);
}

void DameEmu::XOR_H() {
	A ^= H;

	CLEAR_CY;
	CLEAR_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("XOR H = " << (int)A << endl);
}

void DameEmu::XOR_L() {
	A ^= L;

	CLEAR_CY;
	CLEAR_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("XOR L = " << (int)A << endl);
}

void DameEmu::XOR_HL() {
	A ^= (HL);

	CLEAR_CY;
	CLEAR_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 2;
	PC += 1;

	debug_msg("XOR (HL) = " << (int)A << endl);
}

void DameEmu::XOR_A() {
	A ^= A;

	CLEAR_CY;
	CLEAR_H;
	CLEAR_N;
	CLEAR_Z;
	if (A == 0)
		SET_Z;

	cycles += 1;
	PC += 1;

	debug_msg("XOR A = " << (int)A << endl);
}