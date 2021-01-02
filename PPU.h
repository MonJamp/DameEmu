#pragma once
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

struct Obj {
	uint8_t y;
	uint8_t x;
	uint8_t chr;
	struct Attribute {
		union {
			uint8_t raw;
			struct {
				// Color pallete for CGB mode
				unsigned cgb_palette : 3;
				// Character bank for CGB mode
				unsigned cgb_bank : 1;
				// Select OBP0 or OBP1
				unsigned palette : 1;
				// Flip horizontal
				unsigned horizontal_flip : 1;
				// Flip vertical
				unsigned vertical_flip : 1;
				// Display priority flag
				// set = BG priority
				// clear = OBJ priority
				unsigned priority : 1;
			};
		};
	} attr;
};

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
	int16_t scanline_counter;

	sf::RenderWindow* canvas = nullptr;
};