#include "PPU.h"
#include "Bus.h"
#include <array>
#include <vector>


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
		uint16_t bgCharArea = (lcd.lcdc.bg_data_flag) ? 0x8000 : 0x9000;

		for (uint8_t block = 0; block < 20; block++)
		{
			//TODO: Handle cases where scy is not divisble by 8

			uint16_t y_blockOffset = (((lcd.ly + lcd.scy) / 8) * 32);
			uint16_t x_blockOffset = (block + (lcd.scx / 8)) % 32;
			uint16_t bgCodeOffset = (y_blockOffset + x_blockOffset) % 1024;

			uint16_t bgCharOffset;
			// When 0x9000 is base ptr, addressing is signed
			if (bgCharArea == 0x8000)
			{
				uint8_t charCode = bus->Read(bgCodeArea + bgCodeOffset);
				bgCharOffset = (charCode * 0x10) + ((lcd.ly % 8) * 2);
			}
			else
			{
				int8_t charCode = bus->Read(bgCodeArea + bgCodeOffset);
				bgCharOffset = (charCode * 0x10) + ((lcd.ly % 8) * 2);
			}

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

				// Used to fix x position when scx is not divisble by 8
				int8_t x_pos = 0;

				if ((block + (lcd.scx / 8)) >= 32)
				{
					x_pos -= lcd.scx % 8;
				}
				else
				{
					x_pos += lcd.scx % 8;
				}

				x_pos = (i + x_pos) % 8;

				pixels[block * 32 + x_pos * 4] = color.r;
				pixels[block * 32 + x_pos * 4 + 1] = color.g;
				pixels[block * 32 + x_pos * 4 + 2] = color.b;
				pixels[block * 32 + x_pos * 4 + 3] = color.a;

				upperLine <<= 1;
				lowerLine <<= 1;
			}
		}
	}

	// Draw objs
	if (lcd.lcdc.obj_on)
	{
		uint8_t objHeight = (lcd.lcdc.obj_flag) ? 16 : 8;
		uint16_t oamArea = 0xFE00;
		uint16_t objCharArea = 0x8000;

		std::vector<Obj> objs;

		for (uint8_t i = 0; i < 40; i++)
		{
			Obj obj;
			obj.y = bus->Read(oamArea + i * 4);
			obj.x = bus->Read(oamArea + i * 4 + 1);
			obj.chr = bus->Read(oamArea + i * 4 + 2);
			obj.attr.raw = bus->Read(oamArea + i * 4 + 3);

			int16_t y_pos = obj.y - 16;
			int16_t x_pos = obj.x - 8;

			if (x_pos >= 0
				&& x_pos < 160
				&& y_pos >= 0
				&& (lcd.ly / 8) == (y_pos / 8))
			{
				objs.push_back(obj);
			}

			// In 8 x 16 mode, check for the second half of the sprite
			if (objHeight == 16)
			{
				if (x_pos >= 0
					&& x_pos < 160
					&& y_pos >= 0
					&& (lcd.ly / 8) == ((y_pos + 8) / 8))
				{
					objs.push_back(obj);
				}
			}

			// 10 objects per line limit
			if (objs.size() == 10)
			{
				break;
			}
		}

		// Stores the x positions of the sprites to be drawn
		// A sprite which was drawn first has priority over
		// another sprite if they share the same x position
		std::vector<uint8_t> xOfObjs;

		for (auto obj : objs)
		{
			int16_t y_pos = obj.y - 16;
			int16_t x_pos = obj.x - 8;


			// Check if sprite has same x position as another drawn sprite

			bool sameX = false;
			for (auto x : xOfObjs)
			{
				if (x == obj.x)
				{
					sameX = true;
					break;
				}
			}

			if (sameX == true)
			{
				continue;
			}
			else
			{
				xOfObjs.push_back(obj.x);
			}
			
			//TODO: Check Object Priority for lower x position
			// A sprite can't draw over another sprite with a lower x position

			uint16_t objCharOffset = (obj.chr * 0x10);
			if (obj.attr.vertical_flip && objHeight == 8)
			{
				objCharOffset += ((objHeight - (lcd.ly % objHeight)) * 2) - 2;
			}
			else if(objHeight == 8)
			{
				objCharOffset += ((lcd.ly % objHeight)) * 2;
			}
			// To flip 8 x 16 is the reverse process of flipping 8 x 8
			else if (obj.attr.vertical_flip && objHeight == 16)
			{
				objCharOffset += ((lcd.ly % objHeight)) * 2;
			}
			else if(objHeight == 16)
			{
				objCharOffset += ((objHeight - (lcd.ly % objHeight)) * 2) - 2;
			}

			uint8_t upperLine = bus->Read(objCharArea + objCharOffset);
			uint8_t lowerLine = bus->Read(objCharArea + objCharOffset + 1);
			// OBJ Palette Data
			Memory::Registers::LCD::OBP obp = (obj.attr.palette) ? lcd.obp1 : lcd.obp0;

			for (uint8_t i = 0; i < 8; i++)
			{
				bool upperBit;
				bool lowerBit;

				if (obj.attr.horizontal_flip)
				{
					upperBit = upperLine & (1 << i);
					lowerBit = lowerLine & (1 << i);
				}
				else
				{
					upperBit = upperLine & (128 >> i);
					lowerBit = lowerLine & (128 >> i);
				}

				Display::Color color(0x00, 0x00, 0x00, 0x00);

				if (upperBit && lowerBit)
				{
					color = Display::GetColor(obp.palette11);
				}
				else if (upperBit && !lowerBit)
				{
					color = Display::GetColor(obp.palette01);
				}
				else if (!upperBit && lowerBit)
				{
					color = Display::GetColor(obp.palette10);
				}
				else
				{
					// Transparent pixels don't need to be drawn
					color = Display::GetColor(Display::Palette::Transparent);
					continue;
				}

				//TODO: Align sprites horizontally when obj.x is not divisible by 8

				// BG has priority
				if (obj.attr.priority)
				{
					// Sprites can still be visible when BG has priority
					// if the BG color is 0
					//TODO: Proper check to see if color is 0
					if (pixels[(x_pos / 8) * 32 + i * 4] == 0xFF)
					{
						pixels[(x_pos / 8) * 32 + i * 4] = color.r;
						pixels[(x_pos / 8) * 32 + i * 4 + 1] = color.g;
						pixels[(x_pos / 8) * 32 + i * 4 + 2] = color.b;
						pixels[(x_pos / 8) * 32 + i * 4 + 3] = color.a;
					}
					
					continue;
				}

				pixels[(x_pos / 8) * 32 + i * 4] = color.r;
				pixels[(x_pos / 8) * 32 + i * 4 + 1] = color.g;
				pixels[(x_pos / 8) * 32 + i * 4 + 2] = color.b;
				pixels[(x_pos / 8) * 32 + i * 4 + 3] = color.a;
			}
		}
	}

	//TODO: Draw window

	canvas->UpdateLine(lcd.ly + 1, pixels);
}