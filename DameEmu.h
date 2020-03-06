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

public:
	std::shared_ptr<Debugger> debugger;

private:
	std::shared_ptr <Bus> bus;
};