#pragma once
#include "CPU.h"
#include "PPU.h"
#include <string>
#include <cstdint>


class DameEmu {
public:
	DameEmu(const char* rom_dir);
	~DameEmu();

	void Run();

	uint8_t GetCycles() { return cpu.GetCycles(); }

private:
	CPU cpu;
	Memory::MMU mmu;
	PPU ppu;
	//Emulator variables
};