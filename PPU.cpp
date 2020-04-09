#include "PPU.h"
#include "Bus.h"

PPU::PPU(Bus* b)
	: bus(b)
{
	scanline_counter = 456;
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
		//DrawLine
	}
}