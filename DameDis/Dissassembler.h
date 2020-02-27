#pragma once
#include "Cartridge.h"
#include <vector>
#include <string>

struct Opcode {
	std::string mnemonic;
	uint8_t operand_bytes;
};

struct CB_Opcode : public Opcode {
	CB_Opcode(std::string mnemonic) : Opcode{ mnemonic, 0 } {};
	std::string mnemonic;
};

constexpr uint32_t NO_OPERAND = 0x10000;

struct Instruction {
	uint16_t address;
	std::string mnemonic;
	uint32_t operand;
};

class Dissassembler {
public:
	Dissassembler();
	~Dissassembler();

	void LoadCartridge(const char* filename);
	void Disassemble();

	static Opcode opcodeTable[256];
	static CB_Opcode cb_opcodeTable[256];
private:
	uint8_t fetch();
	void StoreNextInstruction();

	uint16_t PC;
	Cartridge* cart;
	std::vector<Instruction> instructions;
};