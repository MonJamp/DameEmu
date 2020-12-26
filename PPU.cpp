#include "PPU.h"
#include "Bus.h"
#include <array>


Display::Color Display::GetColor(uint8_t data) {
	switch (data) {
	case 0b00: return Color(0xFF, 0xFF, 0xFF, 0xFF);
	case 0b01: return Color(0xA9, 0xA9, 0xA9, 0xFF);
	case 0b10: return Color(0x54, 0x54, 0x54, 0xFF);
	case 0b11: return Color(0x00, 0x00, 0x00, 0xFF);
	default: return Color(0x00, 0x00, 0x00, 0x00);
	}
}

Display::Color Display::GetColor(Display::Palette p)
{
	return GetColor(static_cast<uint8_t>(p));
}

PPU::PPU(Bus* b)
	: bus(b)
{
	scanline_counter = 456;
}

void PPU::SetCanvas(MainCanvas* c)
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

	if (bus->regs.lcd.ly < 144)
	{
		DrawLine();
		bus->regs.lcd.stat.match_flag = STAT_HBLANK;
	}
	else if (bus->regs.lcd.ly == 144) {
		//TODO: LCDC should be off during vblank

		BIT_SET(bus->regs.int_request, INT_VBLANK);

		if (bus->regs.lcd.stat.vblank_check)
		{
			BIT_SET(bus->regs.int_request, INT_LCDC);
		}

		bus->regs.lcd.stat.match_flag = STAT_VBLANK;

		//TODO: Only draw canvas during vblank?
	}
	else if (bus->regs.lcd.ly > 153) {
		bus->regs.lcd.ly = 0;
		bus->regs.lcd.stat.match_flag = STAT_HBLANK;
		return;
	}

	bus->regs.lcd.ly++;

	if (bus->regs.lcd.lyc == bus->regs.lcd.ly)
	{
		bus->regs.lcd.stat.match_flag = 1;

		if (bus->regs.lcd.stat.lyc_check)
		{
			BIT_SET(bus->regs.int_request, INT_LCDC);
		}
	}
}

void PPU::DrawLine()
{
	Memory::Registers::LCD& lcd = bus->regs.lcd;

	std::array<uint8_t, 160 * 4> pixels = canvas->GetLine(lcd.ly);

	// Draw bg
	if (lcd.lcdc.bg_on)
	{
		uint16_t bgCodeArea = (lcd.lcdc.bg_area_flag) ? 0x9C00 : 0x9800;
		uint16_t bgCharArea = (lcd.lcdc.bg_data_flag) ? 0x8000 : 0x8800;

		for (uint8_t block = 0; block < 20; block++)
		{
			uint16_t blockOffset = (((lcd.ly + lcd.scy) / 8) * 32);
			uint16_t bgCodeOffset = (block + blockOffset) % 1024;
			uint8_t charCode = bus->Read(bgCodeArea + bgCodeOffset);

			uint16_t bgCharOffset = (charCode * 0x10) + ((lcd.ly % 8) * 2);
			uint8_t upperLine = bus->Read(bgCharArea + bgCharOffset);
			uint8_t lowerLine = bus->Read(bgCharArea + bgCharOffset + 1);

			for (uint8_t i = 0; i < 8; i++)
			{
				bool upperBit = upperLine & 128;
				bool lowerBit = lowerLine & 128;
				Display::Color color(0x00, 0x00, 0x00, 0x00);

				if (upperBit && lowerBit)
				{
					color = Display::GetColor(lcd.bgp.palette11);
				}
				else if (upperBit && !lowerBit)
				{
					color = Display::GetColor(lcd.bgp.palette01);
				}
				else if (!upperBit && lowerBit)
				{
					color = Display::GetColor(lcd.bgp.palette10);
				}
				else
				{
					color = Display::GetColor(lcd.bgp.palette00);
				}

				pixels[block * 32 + i * 4] = color.r;
				pixels[block * 32 + i * 4 + 1] = color.g;
				pixels[block * 32 + i * 4 + 2] = color.b;
				pixels[block * 32 + i * 4 + 3] = color.a;

				upperLine <<= 1;
				lowerLine <<= 1;
			}
		}
	}

	//TODO: Draw window

	//TODO: Draw OBJs

	canvas->UpdateLine(lcd.ly + 1, pixels);
}