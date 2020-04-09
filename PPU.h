#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

class Bus;

class PPU {
public:
	PPU(Bus* b);

	void SetCanvas(sf::RenderWindow* canvas);
	void UpdateScreen(uint8_t cycles);
private:
	void DrawLine();
private:
	Bus* bus;
	sf::RenderWindow* canvas = nullptr;
	int16_t scanline_counter;
};