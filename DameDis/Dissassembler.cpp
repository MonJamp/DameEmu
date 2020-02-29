#include "Dissassembler.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>


Dissassembler::Dissassembler()
{
	Reset();
}

Dissassembler::~Dissassembler()
{
	delete cart;
}

void Dissassembler::Reset()
{
	disassembled = false;
	pc = 0x0000;
	ir = 0x00;

	if (cart != nullptr)
	{
		delete cart;
	}
	cart = new Cartridge();
}

void Dissassembler::LoadCartridge(const char* filename)
{
	Reset();
	cart->open(filename);
}

void Dissassembler::Disassemble()
{
	if (pc < ROM_MAX_SIZE && pc < cart->size())
	{
		StoreNextInstruction();
	}
	else
	{
		disassembled = true;
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

std::string Dissassembler::GetOpcode(uint16_t index)
{
	uint8_t ins_length = disassembly[index].ins.getLength();
	return intToHexString(disassembly[index].opcode, ins_length);
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


// Store operand values and return it in a string
std::string Dissassembler::GetOperandValues(Operand& operand)
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
		operand.value = value;
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
		operand.value = value;
		operand_str = intToHexString(value, 1, prefix.c_str());
	}
	else if (operand.type == Operand::Type::u16)
	{
		std::string prefix = "0x";
		uint16_t value = fetch() | (fetch() << 8);
		operand.value = value;
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
		operand.value += offset;
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
	uint8_t op = cart->read(pc++);

	if (ir == 0)
	{
		ir = op;
	}
	else
	{
		ir = (ir << 8) | op;
	}

	return op;
}

void Dissassembler::StoreNextInstruction() {
	ir = 0x00;

	uint16_t address = pc;
	uint8_t opcode = fetch();

	Instruction ins = insTable[opcode];
	if (ins.is_prefix)
	{
		opcode = fetch();
		ins = cb_insTable[opcode];
	}

	std::vector<std::string> operand_values;
	for (auto& i : ins.operands)
	{
		operand_values.push_back(GetOperandValues(i));
	}

	Disassembly dis;
	dis.address = address;
	dis.opcode = ir;
	dis.ins = ins;
	dis.operand_values = operand_values;

	disassembly.push_back(dis);
}