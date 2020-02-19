#pragma once
#include "CPU.h"
#include "PPU.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdint>

#define LCD_HEIGHT 166
#define LCD_WIDTH 144

#define ROM_ENTRY 0x100


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