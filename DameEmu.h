#pragma once
#include "Bus.h"
#include "Disassembler.h"
#include "Cartridge.h"
#include <string>
#include <cstdint>
#include <memory>


class DameEmu {
public:
	DameEmu(std::shared_ptr<Cartridge>& cart);
	~DameEmu();

	void Step();
	std::shared_ptr<Debugger> GetDebugger() { return debugger; }

private:
	std::shared_ptr <Bus> bus;
	std::shared_ptr<Debugger> debugger;
};