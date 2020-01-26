#include "DameEmu.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#define debug_msg(x) cout << x
#else
#define debug_msg(x)
#endif

#define isAddHalfCarry(x, y) ((((x & 0xF) + (y & 0xF)) & 0x10) == 0x10)

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

	if (FLAG_CHECK(FLAG_ZERO) == 0) {
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

void DameEmu::INC(uint8_t& r) {
	isAddHalfCarry(r, 1) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	r++;
	(r == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 1;
	PC += 1;

	debug_msg("INC " << r - 1 << ";" << endl);
}

void DameEmu::INC_HL() {
	isAddHalfCarry(memory[HL], 1) ? FLAG_SET(FLAG_HALFCARRY) : FLAG_CLEAR(FLAG_HALFCARRY);
	memory[HL]++;
	(memory[HL] == 0) ? FLAG_SET(FLAG_ZERO) : FLAG_CLEAR(FLAG_ZERO);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 3;
	PC += 1;

	debug_msg("INC " << (int)memory[HL] - 1 << ";" << endl);
}

void DameEmu::AND_B() {
	A &= B;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("AND B = " << (int)A << endl);
}

void DameEmu::AND_C() {
	A &= C;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("AND C = " << (int)A << endl);
}

void DameEmu::AND_D() {
	A &= D;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("AND D = " << (int)A << endl);
}

void DameEmu::AND_E() {
	A &= E;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("AND E = " << (int)A << endl);
}

void DameEmu::AND_H() {
	A &= H;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("AND H = " << (int)A << endl);
}

void DameEmu::AND_L() {
	A &= L;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("AND L = " << (int)A << endl);
}

void DameEmu::AND_HL() {
	A &= memory[HL];

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 2;
	PC += 1;

	debug_msg("AND (HL) = " << (int)A << endl);
}

void DameEmu::AND_A() {
	A &= A;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("AND A = " << (int)A << endl);
}

void DameEmu::XOR_B() {
	A ^= B;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("XOR B = " << (int)A << endl);
}

void DameEmu::XOR_C() {
	A ^= C;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("XOR C = " << (int)A << endl);
}

void DameEmu::XOR_D() {
	A ^= D;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("XOR D = " << (int)A << endl);
}

void DameEmu::XOR_E() {
	A ^= E;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("XOR E = " << (int)A << endl);
}

void DameEmu::XOR_H() {
	A ^= H;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("XOR H = " << (int)A << endl);
}

void DameEmu::XOR_L() {
	A ^= L;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 1;
	PC += 1;

	debug_msg("XOR L = " << (int)A << endl);
}

void DameEmu::XOR_HL() {
	A ^= (HL);

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

	cycles += 2;
	PC += 1;

	debug_msg("XOR (HL) = " << (int)A << endl);
}

void DameEmu::XOR_A() {
	A ^= A;

	FLAG_CLEAR(FLAG_CARRY);
	FLAG_CLEAR(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);
	FLAG_CLEAR(FLAG_ZERO);
	if (A == 0)
		FLAG_SET(FLAG_ZERO);

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
	(r & (1 << b)) ? FLAG_CLEAR(FLAG_ZERO) : FLAG_SET(FLAG_ZERO);

	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 2;
	PC += 2;

	debug_msg("BIT " << (int)b << ", " << (int)r << "; Z <- " << (int)FLAG_CHECK(FLAG_ZERO) << endl);
}

void DameEmu::BIT_HL(uint8_t b) {
	(memory[HL] & (1 << b)) ? FLAG_CLEAR(FLAG_ZERO) : FLAG_SET(FLAG_ZERO);

	FLAG_SET(FLAG_HALFCARRY);
	FLAG_CLEAR(FLAG_NEGATIVE);

	cycles += 3;
	PC += 2;

	debug_msg("BIT " << b << ", (HL); Z <- " << (int)FLAG_CHECK(FLAG_ZERO) << endl);
}