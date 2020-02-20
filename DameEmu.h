#pragma once
#include "CPU.h"
#include "PPU.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdint>


class DameEmu {
public:
	DameEmu(const char* ROM_DIR);
	~DameEmu();

	void BootUp(const char* ROM_DIR);
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