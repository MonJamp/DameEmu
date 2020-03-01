#include "Disassembler.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>


template<
	typename T,
	typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
std::string intToHexString(T n, std::streamsize bytes, const char* is_prefix = "")
{
	std::stringstream ss;
	ss << is_prefix
		<< std::setfill('0') << std::setw(bytes * 2)
		<< std::hex << std::uppercase << +n;
	return ss.str();
}

std::string DisasmData::addressToStr()
{
	return intToHexString(address, 2);
}

std::string DisasmData::opcodeToStr()
{
	return intToHexString(opcode, ins.getLength());
}

std::string DisasmData::mnemonicToStr()
{
	return ins.mnemonic;
}

std::string DisasmData::operandsToStr()
{
	std::stringstream ss;

	for (auto i : ins.operands)
	{
		std::string operand_str;

		if (i.isTypeRegister())
		{
			std::string prefix = "%";
			operand_str = prefix + Operand::typeToString(i.type);
		}
		else if (i.type == Operand::Type::s8)
		{
			std::string prefix = "0x";
			uint16_t value = i.value;
			if (value > INT8_MAX)
			{
				prefix = "-0x";
				value = UINT8_MAX - value + 1;
			}
			operand_str = intToHexString(value, 1, prefix.c_str());
		}
		else if (i.type == Operand::Type::u8)
		{
			std::string prefix = "0x";
			operand_str = intToHexString(i.value, 1, prefix.c_str());
		}
		else if (i.type == Operand::Type::u16)
		{
			std::string prefix = "0x";
			operand_str = intToHexString(i.value, 2, prefix.c_str());
		}

		switch (i.mode)
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
			uint16_t offset = i.offset;
			std::string offset_str = intToHexString(offset, 2, "0x");
			ss << "(" << offset_str << " + " << operand_str << ")";
			break;
		}
		case Operand::Mode::IndexOffset:
		{
			std::string prefix = "%";
			std::string offreg_str = Operand::typeToString(i.offset_reg);
			ss << "(" << prefix << offreg_str << " + " << operand_str << ")";
			break;
		}
		default:
			ss << operand_str;
			break;
		}

		ss << ", ";
	}
	// Remove last 2 chars
	return ss.str().substr(0, ss.str().size() - 2);
}

std::string DisasmData::commentToStr()
{
	return comment;
}

Disassembler::Disassembler()
{
	Reset();
}

Disassembler::~Disassembler()
{
	
}

void Disassembler::Reset()
{
	pc = 0x0000;
	ir = 0x00;
	disassembly.reset(new std::vector<DisasmData>());
}

void Disassembler::LoadCartridge(const std::string& filename)
{
	Reset();
	cart.open(filename);
}

void Disassembler::Disassemble()
{
	while (pc < ROM_MAX_SIZE && pc < cart.size())
	{
		disassembly->push_back(DisassembleInstruction());
	}
}


// Store operand values
inline void Disassembler::CacheConstOperands(Operand& operand)
{
	switch (operand.type)
	{
	case Operand::Type::s8:
	case Operand::Type::u8:
		operand.value = fetch();
		return;
	case Operand::Type::u16:
		operand.value = fetch() | (fetch() << 8);
		return;
	default: return;
	}
}

inline uint8_t Disassembler::fetch()
{
	uint8_t op = cart.read(pc++);

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

DisasmData Disassembler::DisassembleInstruction() {
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
	for (auto& i : ins.operands)
	{
		CacheConstOperands(i);

		// Find out if instruction target I/O port
		if (!i.isTypeRegister())
		{
			if (ins.mnemonic == "ldh")
			{
				comment = GetIORegister(0xFF00 + i.value);
			}
			else if (ins.mnemonic == "ld" && i.size() == 2)
			{
				comment = GetIORegister(i.value);
			}
		}
	}

	DisasmData dis;
	dis.address = address;
	dis.opcode = ir;
	dis.ins = ins;
	dis.comment = comment;

	return dis;
}