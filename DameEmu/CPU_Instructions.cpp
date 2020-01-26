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

void DameEmu::UNKNOWN_CB(uint8_t opcode) {
	debug_msg("Unknown OP: CB " << (int)opcode << endl);
}

void DameEmu::LD_r_n(uint8_t& r, uint8_t n) {
	debug_msg("LD " << (int)r << ", " << (int)n << endl);
	
	r = n;

	cycles += 2;
	PC += 2;
}

void DameEmu::LD_HL_n(uint8_t n) {
	debug_msg("LD (" << HL << "), " << (int)n << endl);

	memory[HL] = n;

	cycles += 3;
	PC += 2;
}

void DameEmu::LD_r_r(uint8_t& r1, uint8_t& r2) {
	debug_msg("LD " << (int)r1 << ", " << (int)r2 << ";" << endl);

	r1 = r2;

	cycles += 1;
	PC += 1;
}

void DameEmu::LD_r_HL(uint8_t& r) {
	debug_msg("LD " << (int)r << ", (" << HL << ");" << endl);

	r = memory[HL];

	cycles += 2;
	PC += 1;
}

void DameEmu::LD_HL_r(uint8_t& r) {
	debug_msg("LD (" << HL << "), " << (int)r << ";" << endl);

	memory[HL] = r;

	cycles += 2;
	PC += 1;
}

void DameEmu::LD_BC(uint16_t nn) {
	BC = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD BC, " << nn << endl);
}

void DameEmu::LD_BC_A() {
	memory[BC] = A;

	cycles += 2;
	PC += 1;

	debug_msg("LD (BC), A; (" << BC << ") <- " << (int)A << endl);
}

void DameEmu::LD_DE(uint16_t nn) {
	DE = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD DE, " << nn << endl);
}

void DameEmu::LD_DE_A() {
	memory[DE] = A;

	cycles += 2;
	PC += 1;

	debug_msg("LD (DE), A; (" << BC << ") <- " << (int)A << endl);
}

void DameEmu::JR_NZ(int8_t e) {
	debug_msg("JR NZ " << (int)e << "; ");

	cycles += 2;
	PC += 2;

	if (FLAG_Z == 0) {
		cycles += 1;
		PC += e;

		debug_msg("True, PC <- " << PC << endl);
	}
	else {
		debug_msg("False, PC <- " << PC << endl);
	}
}

void DameEmu::LD_HL(uint16_t nn) {
	HL = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD HL, " << nn << endl);
}

void DameEmu::LD_HLI_A() {
	memory[HL] = A;
	HL++;

	cycles += 2;
	PC += 1;

	debug_msg("LD (HL+), A; (" << HL - 1 << ") <- " << (int)A << endl);
}

void DameEmu::LD_SP(uint16_t nn) {
	SP = nn;

	cycles += 3;
	PC += 3;

	debug_msg("LD SP, " << nn << endl);
}

void DameEmu::LD_HLD_A() {
	memory[HL] = A;
	HL--;

	cycles += 2;
	PC += 1;

	debug_msg("LD (HL-), A; (" << HL+1 << ") <- " << (int)A << endl);
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

void DameEmu::LD_C_A() {
	memory[0xFF00 + C] = A;

	cycles += 2;
	PC += 1;

	debug_msg("LD (" << 0xFF00 + (int)C << "), A;" << endl);
}

void DameEmu::LD_A_C() {
	A = memory[0xFF00 + C];

	cycles += 2;
	PC += 1;

	debug_msg("LD A, (" << 0xFF00 + (int)C << ");" << endl);
}

//CB Instructions
void DameEmu::BIT(uint8_t b, uint8_t& r) {
	(r & (1 << b)) ? CLEAR_Z : SET_Z;

	SET_H;
	CLEAR_N;

	cycles += 2;
	PC += 2;

	debug_msg("BIT " << (int)b << ", " << (int)r << "; Z <- " << (int)FLAG_Z << endl);
}

void DameEmu::BIT_HL(uint8_t b) {
	(memory[HL] & (1 << b)) ? CLEAR_Z : SET_Z;

	SET_H;
	CLEAR_N;

	cycles += 3;
	PC += 2;

	debug_msg("BIT " << b << ", (HL); Z <- " << (int)FLAG_Z << endl);
}