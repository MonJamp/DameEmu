#include "MMU.h"
#include <fstream>

using namespace Memory;

MMU::MMU() {
	interupt_flag = new Registers::Interupt{memory_map[REG_IF]};
	interupt_enable = new Registers::Interupt{memory_map[REG_IE]};
	lcd = new Registers::LCD 
		{
			&memory_map[REG_LCDC],
			&memory_map[REG_STAT],
			memory_map[REG_SCY],
			memory_map[REG_SCX],
			memory_map[REG_LY],
			memory_map[REG_LYC],
			memory_map[REG_DMA],
			memory_map[REG_BGP],
			memory_map[REG_OBP0],
			memory_map[REG_OBP1],
			memory_map[REG_WY],
			memory_map[REG_WX]
		};
	
	Reset();
}

void MMU::Reset() {
	for (uint32_t i = 0; i < sizeof(memory_map); i++) {
		memory_map[i] = 0x0;
	}

	//Gameboy boot state after bios
	//These values are true only for DMG
	memory_map[REG_TIMA] = 0x00;
	memory_map[REG_TMA] = 0x00;
	memory_map[REG_TAC] = 0x00;
	memory_map[REG_NR10] = 0x80;
	memory_map[REG_NR11] = 0xBF;
	memory_map[REG_NR12] = 0xF3;
	memory_map[REG_NR14] = 0xBF;
	memory_map[REG_NR21] = 0x3F;
	memory_map[REG_NR22] = 0x00;
	memory_map[REG_NR24] = 0xBF;
	memory_map[REG_NR30] = 0x7F;
	memory_map[REG_NR31] = 0xFF;
	memory_map[REG_NR32] = 0x9F;
	memory_map[REG_NR33] = 0xBF;
	memory_map[REG_NR41] = 0xFF;
	memory_map[REG_NR42] = 0x00;
	memory_map[REG_NR43] = 0x00;
	memory_map[REG_NR44] = 0xBF;
	memory_map[REG_NR50] = 0x77;
	memory_map[REG_NR51] = 0xF3;
	memory_map[REG_NR52] = 0xF1;
	*lcd->lcdc.state = 0x91;
	lcd->scy = 0x00;
	lcd->scx = 0x00;
	lcd->lyc = 0x00;
	lcd->bgp = 0xFC;
	lcd->obp0 = 0xFF;
	lcd->obp1 = 0xFF;
	lcd->wy = 0x00;
	lcd->wx = 0x00;
	*interupt_enable->state = 0x00;
}

void MMU::LoadCartridge(const char* rom_dir) {
	//TODO: Error checking
	std::ifstream rom_file;
	rom_file.open(rom_dir, std::ios::binary);
	for (uint16_t i = 0; rom_file.get((char&)memory_map[i]); i++);
}

uint8_t MMU::Read(uint16_t address) {
	//TODO: Check read/write status
	return memory_map[address];
}

void MMU::Write(uint16_t address, uint8_t value) {
	//TODO: Check read/write status
	memory_map[address] = value;
}