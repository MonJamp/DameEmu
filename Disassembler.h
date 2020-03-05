#pragma once
#include "InstructionSet.h"
#include "Cartridge.h"
#include <vector>
#include <string>
#include <unordered_map>

#ifdef __GNUG__
#include <memory>
#endif


struct DisasmData {
	std::string addressToStr();
	std::string opcodeToStr();
	std::string mnemonicToStr();
	std::string operandsToStr();
	std::string commentToStr();

	uint16_t address = 0x0000;
	uint32_t opcode = 0x0;
	Instruction ins;
	std::string comment;
};

using Disassembly = std::vector<DisasmData>;


class Disassembler {
public:
	Disassembler();
	~Disassembler();

	void LoadCartridge(std::shared_ptr<Cartridge>& cart);
	void Disassemble();

	std::shared_ptr<std::vector<DisasmData>> GetDisassembly() { return disassembly; }

	std::unordered_map<uint16_t, uint16_t> jumpTable;
	uint16_t it = 0;
private:
	void Reset();
	void CacheConstOperands(Operand& operand);
	uint8_t fetch();
	DisasmData DisassembleInstruction();

	Instruction curr_ins;
	uint16_t index;
	uint32_t ir; // ir aka instruction registers stores full instruction
	std::shared_ptr<Cartridge> cart;
	std::shared_ptr<Disassembly> disassembly;
	
};
