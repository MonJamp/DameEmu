#include "PPU.h"
#include "Bus.h"

PPU::PPU(Bus* b)
	: bus(b)
{
	scanline_counter = 456;
}

void PPU::SetCanvas(sf::RenderWindow* c)
{
	canvas = c;
}

void PPU::UpdateScreen(uint8_t cycles) {
	if (!bus->regs.lcd.lcdc.lcd_on) {
		return;
	}

	scanline_counter -= cycles;
	if (scanline_counter > 0) {
		return;
	}

	scanline_counter = 456;

	bus->regs.lcd.ly++;
	if (bus->regs.lcd.ly == 144) {
		BIT_SET(bus->regs.int_enable, INT_VBLANK);
	}
	else if (bus->regs.lcd.ly > 153) {
		bus->regs.lcd.ly = 0;
	}
	else {
		DrawLine();
	}
}

void PPU::DrawLine()
{
	uint8_t* pixels = new uint8_t[8 * 8 * 4];
	
	for (int i = 0; i < 16; i += 2)
	{
		uint8_t line1 = bus->Read(0x8000 + i);
		uint8_t line2 = bus->Read(0x8000 + i + 1);

		for (int j = 0; j < 8; j++)
		{
			uint8_t pixel = (line1 & 1) | (line2 & 1);

			if (pixel > 0)
			{
				pixels[i * 8 + j * 4] = 255;
				pixels[i * 8 + j * 4 + 1] = 255;
				pixels[i * 8 + j * 4 + 2] = 255;
				pixels[i * 8 + j * 4 + 3] = 255;
			}
			else
			{
				pixels[i * 8 + j * 4] = 0;
				pixels[i * 8 + j * 4 + 1] = 0;
				pixels[i * 8 + j * 4 + 2] = 0;
				pixels[i * 8 + j * 4 + 3] = 255;
			}

			line1 >= 1;
			line2 >= 1;
		}
	}
	
	sf::Texture texture;
	texture.create(8, 8);
	texture.update(pixels);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setScale(5.f, 5.f);
	canvas->draw(sprite);

	delete[] pixels;
}