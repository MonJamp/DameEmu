#pragma once
#include "InstructionSet.h"
#include "Cartridge.h"
#include <vector>
#include <string>


struct Disassembly {
	uint16_t address = 0x0000;
	uint32_t opcode = 0x0;
	Instruction ins;
	std::vector<std::string> operand_values;
	std::string comment;
};

class Dissassembler {
public:
	Dissassembler();
	~Dissassembler();

	void LoadCartridge(const char* filename);
	void Disassemble();

	size_t GetNumOfInstructions();
	std::string GetAddress(uint16_t index);
	std::string GetOpcode(uint16_t index);
	std::string GetMnemonic(uint16_t index);
	std::string GetOperands(uint16_t index);
	std::string GetComment(uint16_t index);
	bool isDisassembled() { return disassembled; }

private:
	void Reset();
	std::string GetOperandValues(Operand& operand);
	uint8_t fetch();
	void StoreNextInstruction();

	bool disassembled;
	uint16_t pc;
	uint32_t ir; // ir aka instruction registers stores full instruction
	Cartridge* cart;
	std::vector<Disassembly> disassembly;
};