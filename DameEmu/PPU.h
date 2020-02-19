#pragma once
#include "MMU.h"

class PPU {
public:
	PPU(Memory::MMU& mmu);

	void UpdateScreen(uint8_t cycles);
private:
	int16_t scanline_counter;
	Memory::MMU& mmu;
};