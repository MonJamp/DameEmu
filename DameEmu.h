#pragma once
#include "Bus.h"
#include "Disassembler.h"
#include "Cartridge.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdint>
#include <memory>


class DameEmu {
public:
	DameEmu();
	DameEmu(const std::string& filename);
	~DameEmu();

	void SetCanvas(sf::RenderWindow* canvas);
	void Step();

	std::shared_ptr<Debugger> GetDebugger() { return debugger; }
	const Cartridge& GetCart() { return *bus->cart; }

private:
	std::shared_ptr <Bus> bus;
	std::shared_ptr<Debugger> debugger;
};