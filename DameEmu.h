#pragma once
#include "Bus.h"
#include "Disassembler.h"
#include "Cartridge.h"
#include "GUI/MainCanvas.h"
#include <string>
#include <cstdint>
#include <memory>


class DameEmu {
public:
	DameEmu();
	DameEmu(const std::string& filename);
	~DameEmu();

	void SetCanvas(MainCanvas* canvas);
	void Step();

	std::shared_ptr<Debugger> GetDebugger() { return debugger; }
	Cartridge& GetCart() { return *bus->cart; }

private:
	std::shared_ptr <Bus> bus;
	std::shared_ptr<Debugger> debugger;
};