#include "Dissassembler.h"
#include "OpcodeTables.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>


Dissassembler::Dissassembler()
{
	PC = 0;
	cart = new Cartridge();

}

Dissassembler::~Dissassembler()
{
	delete cart;
}

void Dissassembler::LoadCartridge(const char* filename)
{
	cart->open(filename);
}

void Dissassembler::Disassemble()
{
	while (PC < ROM_MAX_SIZE)
	{
		StoreNextInstruction();
	}
}

size_t Dissassembler::GetNumOfInstructions()
{
	return instructions.size();
}


template<typename T>
std::string intToHex(T n, uint8_t bytes, const char* prefix = "")
{
	std::stringstream ss;
	ss	<< prefix
		<< std::setfill('0') << std::setw(bytes * 2)
		<< std::hex << std::uppercase << n;
	return ss.str();
}

std::string Dissassembler::GetAddress(uint16_t index)
{
	uint16_t address = instructions[index].address;
	return intToHex(address, 2);
}

std::string Dissassembler::GetMnemonic(uint16_t index)
{
	return instructions[index].mnemonic;
}

std::string Dissassembler::GetOperand(uint16_t index)
{
	if (instructions[index].operand == NO_OPERAND)
	{
		return std::string("");
	}
	else
	{
		uint32_t operand = instructions[index].operand;
		uint8_t bytes = instructions[index].operand_bytes;
		return intToHex(operand, bytes, "0x");
	}
}

inline uint8_t Dissassembler::fetch()
{
	return cart->read(PC++);
}

void Dissassembler::StoreNextInstruction() {
	uint16_t address = PC;
	uint8_t opcode = fetch();
	uint8_t operand_bytes = opcodeTable[opcode].operand_bytes;
	uint32_t operand = NO_OPERAND;

	if (operand_bytes == 1)
	{
		operand = fetch();
	}
	else if (operand_bytes == 2)
	{
		operand = fetch() | (fetch() << 8);
	}

	Instruction ins;
	ins.address = address;
	ins.mnemonic = opcodeTable[opcode].mnemonic;
	ins.operand = operand;
	ins.operand_bytes = operand_bytes;

	instructions.push_back(ins);
}