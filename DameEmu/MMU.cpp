#include "MMU.h"
#include <fstream>

using namespace Memory;

MMU::MMU() {
	interupt_flag = new Registers::Interupt(memory_map[0xFF0F]);
	interupt_enable = new Registers::Interupt(memory_map[0xFFFF]);
	lcd = new Registers::LCD 
		{
			&memory_map[0xFF40],
			&memory_map[0xFF41],
			memory_map[0xFF42],
			memory_map[0xFF43],
			memory_map[0xFF44],
			memory_map[0xFF45],
			memory_map[0xFF46],
			memory_map[0xFF47],
			memory_map[0xFF48],
			memory_map[0xFF49],
			memory_map[0xFF4A],
			memory_map[0xFF4B]
		};
	
	Reset();
}

void MMU::Reset() {
	for (uint32_t i = 0; i < sizeof(memory_map); i++) {
		memory_map[i] = 0x0;
	}

	//Gameboy boot state after bios
	//These values are true only for DMG
	TIMA = 0x00;
	TMA = 0x00;
	TAC = 0x00;
	NR10 = 0x80;
	NR11 = 0xBF;
	NR12 = 0xF3;
	NR14 = 0xBF;
	NR21 = 0x3F;
	NR22 = 0x00;
	NR24 = 0xBF;
	NR30 = 0x7F;
	NR31 = 0xFF;
	NR32 = 0x9F;
	NR33 = 0xBF;
	NR41 = 0xFF;
	NR42 = 0x00;
	NR43 = 0x00;
	NR44 = 0xBF;
	NR50 = 0x77;
	NR51 = 0xF3;
	NR52 = 0xF1;
	LCDC = 0x91;
	SCY = 0x00;
	SCX = 0x00;
	LYC = 0x00;
	BGP = 0xFC;
	OBP0 = 0xFF;
	OBP1 = 0xFF;
	WY = 0x00;
	WX = 0x00;
	InterruptEnable = 0x00;
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