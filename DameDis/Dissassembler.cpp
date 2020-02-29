#include "Dissassembler.h"
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
	return disassembly.size();
}


template<typename T>
std::string intToHexString(T n, std::streamsize bytes, const char* is_prefix = "")
{
	std::stringstream ss;
	ss	<< is_prefix
		<< std::setfill('0') << std::setw(bytes * 2)
		<< std::hex << std::uppercase << +n;
	return ss.str();
}

std::string Dissassembler::GetAddress(uint16_t index)
{
	uint16_t address = disassembly[index].address;
	return intToHexString(address, 2);
}

std::string Dissassembler::GetMnemonic(uint16_t index)
{
	return disassembly[index].ins.mnemonic;
}

std::string Dissassembler::GetOperands(uint16_t index)
{
	if (disassembly[index].operand_values.size() > 0)
	{
		std::stringstream ss;

		for (auto i : disassembly[index].operand_values)
		{
			ss << i << ", ";
		}

		// Remove last 2 chars
		return ss.str().substr(0, ss.str().size() - 2);
	}

	return "";
}

std::string Dissassembler::operandValueToString(Operand operand)
{
	std::stringstream ss;
	std::string operand_str;

	if (operand.isTypeRegister())
	{
		std::string prefix = "%";
		operand_str = prefix + Operand::typeToString(operand.type);
	}
	else if (operand.type == Operand::Type::s8)
	{
		std::string prefix = "0x";
		uint8_t value = fetch();
		if (value > INT8_MAX)
		{
			prefix = "-0x";
			value = UINT8_MAX - value + 1;
		}
		operand_str = intToHexString(value, 1, prefix.c_str());
	}
	else if (operand.type == Operand::Type::u8)
	{
		std::string prefix = "0x";
		uint8_t value = fetch();
		operand_str = intToHexString(value, 1, prefix.c_str());
	}
	else if (operand.type == Operand::Type::u16)
	{
		std::string prefix = "0x";
		uint16_t value = fetch() | (fetch() << 8);
		operand_str = intToHexString(value, 2, prefix.c_str());
	}

	switch (operand.mode)
	{
	case Operand::Mode::Address:
		ss << "(" << operand_str << ")";
		break;
	case Operand::Mode::AutoInc:
		ss << "(" << operand_str << "+)";
		break;
	case Operand::Mode::AutoDec:
		ss << "(" << operand_str << "-)";
		break;
	case Operand::Mode::Offset:
	{
		uint16_t offset = operand.offset;
		std::string offset_str = intToHexString(offset, 2, "0x");
		ss << "(" << offset_str << " + " << operand_str << ")";
		break;
	}
	case Operand::Mode::IndexOffset:
	{
		std::string prefix = "%";
		std::string offreg_str = Operand::typeToString(operand.offset_reg);
		ss << "(" << prefix << offreg_str << " + " << operand_str << ")";
		break;
	}
	default:
		ss << operand_str;
		break;
	}

	return ss.str();
}

inline uint8_t Dissassembler::fetch()
{
	return cart->read(PC++);
}

void Dissassembler::StoreNextInstruction() {
	uint16_t address = PC;
	
	uint8_t opcode = fetch();
	Instruction ins = insTable[opcode];
	if (ins.is_prefix)
	{
		opcode = fetch();
		ins = cb_insTable[opcode];
	}

	std::vector<std::string> operand_values;
	for (auto i : ins.operands)
	{
		operand_values.push_back(operandValueToString(i));
	}

	Disassembly dis;
	dis.address = address;
	dis.opcode = opcode;
	dis.ins = ins;
	dis.operand_values = operand_values;

	disassembly.push_back(dis);
}