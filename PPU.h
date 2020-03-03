#pragma once
#include <cstdint>

class Bus;

class PPU {
public:
	PPU(Bus* b);

	void UpdateScreen(uint8_t cycles);
private:
	Bus* bus;
	int16_t scanline_counter;
};