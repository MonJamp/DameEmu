#pragma once
#include <vector>
#include <string>

struct Instruction {
	std::string mnemonic;
	uint8_t length;
};

struct CB_Instruction {
	std::string mnemonic;
};

class Dissassembler {
public:
	Dissassembler(std::string ROM_DIR);
	void LoadROM(std::string ROM__DIR);
	void PrintInstructions();
	void DisassembleToFile(std::string OUTPUT_DIR);

	static Instruction instructions[256];
	static CB_Instruction cb_instructions[256];
private:
	std::string GetFullDissassembly();
	std::string GetNextInstruction();

	uint16_t PC;
	std::vector<uint8_t> memory;
};