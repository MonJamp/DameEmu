#pragma once
#include "Bus.h"
#include "Disassembler.h"

#include <string>
#include <cstdint>
#include <memory>
#include <array>


class DameEmu {
public:
	DameEmu();
	DameEmu(const std::string& filename);
	~DameEmu();

	void Step();
	void Reset();

	std::shared_ptr<Debugger> GetDebugger() { return debugger; }
	Cartridge& GetCart() { return *bus->cart; }

private:
	std::shared_ptr <Bus> bus;
	std::shared_ptr<Debugger> debugger;
};