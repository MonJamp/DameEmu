#pragma once
#include "InstructionSet.h"
#include "Bus.h"
#include "Cartridge.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>
#include <unordered_map>


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
using AddressTable = std::unordered_map<uint16_t, uint16_t>;


class Disassembler {
public:
	Disassembler();
	~Disassembler();

	void LoadCartridge(std::shared_ptr<Cartridge>& c);
	void Disassemble();

	std::shared_ptr<std::vector<DisasmData>> GetDisassembly() { return disassembly; }

public:
	AddressTable addressTable; // Address is key, index is value

private:
	void Reset();
	void CacheConstOperands(Operand& operand);
	uint8_t fetch();
	DisasmData DisassembleInstruction();

	Instruction curr_ins;
	uint16_t pc;
	uint32_t ir; // ir aka instruction registers stores full instruction
	std::shared_ptr<Cartridge> cart;
	std::shared_ptr<Disassembly> disassembly;
};

class Debugger
{
public:
	struct CpuState
	{
		uint16_t pc;
		uint16_t sp;
		uint16_t af;
		uint16_t bc;
		uint16_t de;
		uint16_t hl;
		bool ime;
	};

	CpuState cpuState;

public:
	Debugger(std::shared_ptr<Bus>& b);

	void Step();
	std::shared_ptr<std::vector<DisasmData>> GetDisassembly() { return disassembler->GetDisassembly(); }
	AddressTable GetAddressTable() { return disassembler->addressTable; }
private:
	std::shared_ptr<Bus> bus;
	std::shared_ptr<Disassembler> disassembler;

private:
	void UpdateCpuState();
};
