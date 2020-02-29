#pragma once
#include "InstructionSet.h"
#include "Cartridge.h"
#include <vector>
#include <string>


struct Disassembly {
	uint16_t address = 0x0000;
	uint8_t opcode = 0x0;
	Instruction ins;
	std::vector<std::string> operand_values;
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
	std::string GetOperands(uint16_t index);
	

private:
	std::string operandValueToString(Operand operand);
	uint8_t fetch();
	void StoreNextInstruction();

	uint16_t PC;
	Cartridge* cart;
	std::vector<Disassembly> disassembly;
};