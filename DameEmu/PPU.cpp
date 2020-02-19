#include "PPU.h"

PPU::PPU(Memory::MMU& mmu) :
	mmu(mmu)
{
	scanline_counter = 456;
}

void PPU::UpdateScreen(uint8_t cycles) {
	if (!mmu.lcd->lcdc.field->lcd_on) {
		return;
	}

	scanline_counter -= cycles;
	if (scanline_counter > 0) {
		return;
	}

	scanline_counter = 456;

	mmu.lcd->ly++;
	if (mmu.lcd->ly == 144) {
		mmu.interupt_flag->field->vblank = 1;
	}
	else if (mmu.lcd->ly > 153) {
		mmu.lcd->ly = 0;
	}
	else {
		//DrawLine
	}
}