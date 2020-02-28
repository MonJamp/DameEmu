#pragma once
#include "Cartridge.h"
#include <vector>
#include <string>

constexpr uint16_t NO_OPERAND = 0x10000;

struct Instruction {
	uint16_t address;
	std::string mnemonic;
	uint32_t operand;
	uint8_t operand_bytes;
};

class Dissassembler {
public:
	Dissassembler();
	~Dissassembler();

	void LoadCartridge(const char* filename);
	void Disassemble();

	size_t GetNumOfInstructions();
	std::string GetAddress(uint16_t index);
	std::string GetMnemonic(uint16_t index);
	std::string GetOperand(uint16_t index);

private:
	uint8_t fetch();
	void StoreNextInstruction();

	uint16_t PC;
	Cartridge* cart;
	std::vector<Instruction> instructions;
};