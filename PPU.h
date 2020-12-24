#pragma once
#include "GUI/MainCanvas.h"
#include <SFML/Graphics.hpp>
#include <cstdint>


namespace Display
{
	struct Color {
		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
			: r(red), g(green), b(blue), a(alpha)
		{}

		union {
			uint32_t rgba;
			struct {
				uint8_t a;
				uint8_t b;
				uint8_t g;
				uint8_t r;
			};
		};
	};

	enum class Palette {
		Shade00,
		Shade01,
		Shade10,
		Shade11,
		Transparent
	};

	Color GetColor(uint8_t data);
	Color GetColor(Palette p);
}

class Bus;

class PPU {
public:
	PPU(Bus* b);

	void SetCanvas(MainCanvas* canvas);
	void UpdateScreen(uint8_t cycles);
private:
	void DrawLine();
private:
	Bus* bus;
	int16_t scanline_counter;

	MainCanvas* canvas = nullptr;
};