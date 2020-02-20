#pragma once
#include "CPU.h"
#include "PPU.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdint>


class DameEmu {
public:
	DameEmu(const char* rom_dir);
	~DameEmu();

	void Run();

	uint8_t GetCycles() { return cpu.GetCycles(); }
	sf::RenderWindow* GetApp() { return app; };

private:
	CPU cpu;
	Memory::MMU mmu;
	PPU ppu;
	//Emulator variables
	sf::RenderWindow* app;
};