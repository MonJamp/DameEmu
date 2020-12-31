#pragma once
#include "InstructionSet.h"
#include "Bus.h"
#include "Cartridge/Cartridge.h"
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

class Debugger
{
public:
	struct RegState
	{
		struct CPU
		{
			uint16_t pc;
			uint16_t sp;
			uint16_t af;
			uint16_t bc;
			uint16_t de;
			uint16_t hl;
			bool ime;
		} cpu;
		
		struct Mem
		{
			uint8_t p1, sb, sc, div, tima, tma, tac,
				nr10, nr11, nr12, nr13, nr14,
				nr21, nr22, nr23, nr24,
				nr30, nr31, nr32, nr33, nr34,
				nr41, nr42, nr43, nr44,
				nr50, nr51, nr52,
				lcdc, stat, scy, scx, ly, lyc,
				dma, bgp, obp0, obp1, wy, wx,
				int_req, int_ie;
		} mem;
	};

	RegState regState;

public:
	Debugger(std::shared_ptr<Bus> b);

	void Disassemble();
	void Step();
	bool HitBreakpoint();
	bool RemoveBreakpoint(uint16_t address);
	void AddBreakpoint(uint16_t address);
	void ToggleBreakpoint(uint16_t address);

	std::vector<DisasmData> GetDisassembly() { return disassembly; }
	AddressTable GetAddressTable() { return addressTable; }
	Disassembly GetBreakpoints() { return breakpoints; }
	std::shared_ptr<Bus> GetBus() { return bus; }

private:
	void UpdateRegState();

private:
	std::shared_ptr<Bus> bus;
	Disassembly disassembly;
	AddressTable addressTable; // Address is key, index is value
	Disassembly breakpoints;
};
