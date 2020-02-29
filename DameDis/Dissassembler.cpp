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

std::string Dissassembler::GetComment(uint16_t index)
{
	return disassembly[index].comment;
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

// Returns string of which I/O register is associated with the address
std::string GetIORegister(uint16_t address)
{
	switch (address)
	{
	case 0xFF00: return "P1";
	case 0xFF01: return "SB";
	case 0xFF02: return "SC";
	case 0xFF04: return "DIV";
	case 0xFF05: return "TIMA";
	case 0xFF06: return "TMA";
	case 0xFF07: return "TAC";
	case 0xFF10: return "NR10";
	case 0xFF11: return "NR11";
	case 0xFF12: return "NR12";
	case 0xFF13: return "NR13";
	case 0xFF14: return "NR14";
	case 0xFF16: return "NR21";
	case 0xFF17: return "NR22";
	case 0xFF18: return "NR23";
	case 0xFF19: return "NR24";
	case 0xFF1A: return "NR30";
	case 0xFF1B: return "NR31";
	case 0xFF1C: return "NR32";
	case 0xFF1D: return "NR33";
	case 0xFF1E: return "NR34";
	case 0xFF20: return "NR41";
	case 0xFF21: return "NR42";
	case 0xFF22: return "NR43";
	case 0xFF23: return "NR44";
	case 0xFF24: return "NR50";
	case 0xFF25: return "NR51";
	case 0xFF26: return "NR52";
	case 0xFF30:
	case 0xFF31:
	case 0xFF32:
	case 0xFF33:
	case 0xFF34:
	case 0xFF35:
	case 0xFF36:
	case 0xFF37:
	case 0xFF38:
	case 0xFF39: return "Wave RAM";
	case 0xFF40: return "LCDC";
	case 0xFF41: return "STAT";
	case 0xFF42: return "SCY";
	case 0xFF43: return "SCX";
	case 0xFF44: return "LY";
	case 0xFF45: return "LYC";
	case 0xFF46: return "DMA";
	case 0xFF47: return "BGP";
	case 0xFF48: return "OBP0";
	case 0xFF49: return "OBP1";
	case 0xFF4A: return "WY";
	case 0xFF4B: return "WX";
	case 0xFF50: return "Map ROM";
	case 0xFF0F: return "IF";
	case 0xFFFF: return "IE";
	default:	return "";
	}
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

	std::string comment = "";
	std::vector<std::string> operand_values;
	for (auto& i : ins.operands)
	{
		operand_values.push_back(GetOperandValues(i));


		// Find out if instruction target I/O port
		if (ins.mnemonic == "ldh" && !i.isTypeRegister())
		{
			comment = GetIORegister(i.value);
		}
	}

	Disassembly dis;
	dis.address = address;
	dis.opcode = ir;
	dis.ins = ins;
	dis.operand_values = operand_values;
	dis.comment = comment;

	disassembly.push_back(dis);
}