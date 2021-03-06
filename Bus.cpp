#include "Bus.h"
#ifdef D_SERIAL_OUT
#include <cstdio>
#endif

Bus::Bus()
	: cpu(this), ppu(this)
{
	cart = std::make_unique<Cartridge>();
	map = std::make_shared<Memory::Map>();
	Reset();
}

Bus::~Bus()
{

}

void Bus::Reset()
{
	cpu.Reset();
	map->raw.fill(0x00);

	pulseTimer = false;

	//TODO: Temporary until joypad is implemented
	regs.input.raw = 0xF;

	// Gameboy boot state after bios
	// These values are true only for DMG
	regs.counter.raw = 0xABCC;
	regs.timer.tima = 0x00;
	regs.timer.tma = 0x00;
	regs.timer.tac.raw = 0x00;
	regs.sound.nr10 = 0x80;
	regs.sound.nr11 = 0xBF;
	regs.sound.nr12 = 0xF3;
	regs.sound.nr14 = 0xBF;
	regs.sound.nr21 = 0x3F;
	regs.sound.nr22 = 0x00;
	regs.sound.nr24 = 0xBF;
	regs.sound.nr30 = 0x7F;
	regs.sound.nr31 = 0xFF;
	regs.sound.nr32 = 0x9F;
	regs.sound.nr33 = 0xBF;
	regs.sound.nr41 = 0xFF;
	regs.sound.nr42 = 0x00;
	regs.sound.nr43 = 0x00;
	regs.sound.nr44 = 0xBF;
	regs.sound.nr50 = 0x77;
	regs.sound.nr51 = 0xF3;
	regs.sound.nr52 = 0xF1;
	regs.lcd.lcdc.raw = 0x91;
	regs.lcd.scy = 0x00;
	regs.lcd.scx = 0x00;
	regs.lcd.lyc = 0x00;
	regs.lcd.bgp.raw = 0xFC;
	regs.lcd.obp0.raw = 0xFF;
	regs.lcd.obp1.raw = 0xFF;
	regs.lcd.wy = 0x00;
	regs.lcd.wx = 0x00;
	regs.int_enable = 0x00;
}

void Bus::InsertCartridge(const std::string& filename)
{
	cart->open(filename);
}

void Bus::Write(uint16_t address, uint8_t data)
{
	if (address >= 0x0000 && address <= 0x7FFF)
	{
		cart->ROM_Write(address, data);
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		map->vram[address - 0x8000] = data;
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		cart->RAM_Write(address, data);
	}
	else if (address >= 0xC000 && address <= 0xDFFF)
	{
		map->ram[address - 0xC000] = data;
	}
	else if (address >= 0xE000 && address <= 0xFDFF)
	{
		// Prohibited
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		map->oam[address - 0xFE00] = data;
	}
	else if (address >= 0xFEA0 && address <= 0xFEFF)
	{
		// Prohibited
	}
	else if (address >= 0xFF00 && address <= 0xFF7F)
	{
		map->registers[address - 0xFF00] = data;

		// TODO writes to registers not accurate
		if (address >= ADDR_WAVE_RAM && address < ADDR_WAVE_RAM + WAVE_RAM_SIZE)
		{
			regs.sound.wav[address % ADDR_WAVE_RAM] = data;
		}

		switch (address) {
		case ADDR_P1:
			//TODO: Temporary until joypad is implemented
			//regs.input.raw = data;
			break;
		case ADDR_SB:
			regs.serial.sb = data;
			break;
		case ADDR_SC:
			regs.serial.sc.raw = data;
			break;
		case ADDR_DIV:
			// Writing to div resets internal counter
			regs.counter.raw = 0x00;
			break;
		case ADDR_TIMA:
			regs.timer.tima = data;
			break;
		case ADDR_TMA:
			regs.timer.tma = data;
			break;
		case ADDR_TAC:
			regs.timer.tac.raw = data;
			break;
		case ADDR_NR10:
			regs.sound.nr10 = data;
			break;
		case ADDR_NR11:
			regs.sound.nr11 = data;
			break;
		case ADDR_NR12:
			regs.sound.nr12 = data;
			break;
		case ADDR_NR13:
			regs.sound.nr13 = data;
			break;
		case ADDR_NR14:
			regs.sound.nr14 = data;
			break;
		case ADDR_NR21:
			regs.sound.nr21 = data;
			break;
		case ADDR_NR22:
			regs.sound.nr22 = data;
			break;
		case ADDR_NR23:
			regs.sound.nr23 = data;
			break;
		case ADDR_NR24:
			regs.sound.nr24 = data;
			break;
		case ADDR_NR30:
			regs.sound.nr30 = data;
			break;
		case ADDR_NR31:
			regs.sound.nr31 = data;
			break;
		case ADDR_NR32:
			regs.sound.nr32 = data;
			break;
		case ADDR_NR33:
			regs.sound.nr33 = data;
			break;
		case ADDR_NR34:
			regs.sound.nr34 = data;
			break;
		case ADDR_NR41:
			regs.sound.nr41 = data;
			break;
		case ADDR_NR42:
			regs.sound.nr42 = data;
			break;
		case ADDR_NR43:
			regs.sound.nr43 = data;
			break;
		case ADDR_NR44:
			regs.sound.nr44 = data;
			break;
		case ADDR_NR50:
			regs.sound.nr50 = data;
			break;
		case ADDR_NR51:
			regs.sound.nr51 = data;
			break;
		case ADDR_NR52:
			regs.sound.nr52 = data;
			break;
		case ADDR_LCDC:
			regs.lcd.lcdc.raw = data;
			break;
		case ADDR_STAT:
			regs.lcd.stat.raw = 0b10000000 | (data & 0b11111000);
			break;
		case ADDR_SCY:
			regs.lcd.scy = data;
			break;
		case ADDR_SCX:
			regs.lcd.scx = data;
			break;
		case ADDR_LY:
			regs.lcd.ly = data;
			break;
		case ADDR_LYC:
			regs.lcd.lyc = data;
			break;
		case ADDR_DMA:
			regs.lcd.dma = data;
			// DMA Transfer
			dmaTransfer(data);
			break;
		case ADDR_BGP:
			regs.lcd.bgp.raw = data;
			break;
		case ADDR_OBP0:
			regs.lcd.obp0.raw = data;
			break;
		case ADDR_OBP1:
			regs.lcd.obp1.raw = data;
			break;
		case ADDR_WY:
			regs.lcd.wy = data;
			break;
		case ADDR_WX:
			regs.lcd.wx = data;
			break;
		case ADDR_IF:
			regs.int_request = data;
			break;
		default: 
			break;
		}
	}
	else if (address >= 0xFF80 && address <= 0xFFFE)
	{
		map->high_ram[address - 0xFF80] = data;
	}
	else if (address == 0xFFFF)
	{
		map->int_enable = data;
		regs.int_enable = data;
	}
}

uint8_t Bus::Read(uint16_t address)
{
	uint8_t data = 0x00;

	if (address >= 0x0000 && address <= 0x7FFF)
	{
		data = cart->ROM_Read(address);
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		data = map->vram[address - 0x8000];
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		data = cart->RAM_Read(address);
	}
	else if (address >= 0xC000 && address <= 0xDFFF)
	{
		data = map->ram[address - 0xC000];
	}
	else if (address >= 0xE000 && address <= 0xFDFF)
	{
		// Prohibited
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		data = map->oam[address - 0xFE00];
	}
	else if (address >= 0xFEA0 && address <= 0xFEFF)
	{
		// Prohibited
	}
	else if (address >= 0xFF00 && address <= 0xFF7F)
	{
		// TODO reads from registers not accurate
		if (address >= ADDR_WAVE_RAM && address < ADDR_WAVE_RAM + WAVE_RAM_SIZE)
		{
			data = regs.sound.wav[address % WAVE_RAM_SIZE];
		}

		switch (address) {
		case ADDR_P1:
			data = regs.input.raw;
			break;
		case ADDR_SB:
			data = regs.serial.sb;
			break;
		case ADDR_SC:
			data = regs.serial.sc.raw;
			break;
		case ADDR_DIV:
			data = regs.counter.div;
			break;
		case ADDR_TIMA:
			data = regs.timer.tima;
			break;
		case ADDR_TMA:
			data = regs.timer.tma;
			break;
		case ADDR_TAC:
			data = regs.timer.tac.raw;
			break;
		case ADDR_NR10:
			data = regs.sound.nr10;
			break;
		case ADDR_NR11:
			data = regs.sound.nr11;
			break;
		case ADDR_NR12:
			data = regs.sound.nr12;
			break;
		case ADDR_NR13:
			data = regs.sound.nr13;
			break;
		case ADDR_NR14:
			data = regs.sound.nr14;
			break;
		case ADDR_NR21:
			data = regs.sound.nr21;
			break;
		case ADDR_NR22:
			data = regs.sound.nr22;
			break;
		case ADDR_NR23:
			data = regs.sound.nr23;
			break;
		case ADDR_NR24:
			data = regs.sound.nr24;
			break;
		case ADDR_NR30:
			data = regs.sound.nr30;
			break;
		case ADDR_NR31:
			data = regs.sound.nr31;
			break;
		case ADDR_NR32:
			data = regs.sound.nr32;
			break;
		case ADDR_NR33:
			data = regs.sound.nr33;
			break;
		case ADDR_NR34:
			data = regs.sound.nr34;
			break;
		case ADDR_NR41:
			data = regs.sound.nr41;
			break;
		case ADDR_NR42:
			data = regs.sound.nr42;
			break;
		case ADDR_NR43:
			data = regs.sound.nr43;
			break;
		case ADDR_NR44:
			data = regs.sound.nr44;
			break;
		case ADDR_NR50:
			data = regs.sound.nr50;
			break;
		case ADDR_NR51:
			data = regs.sound.nr51;
			break;
		case ADDR_NR52:
			data = regs.sound.nr52;
			break;
		case ADDR_LCDC:
			data = regs.lcd.lcdc.raw;
			break;
		case ADDR_STAT:
			// bit 7 always return 1
			// bit 0-1 return 0 when lcd is off
			if (regs.lcd.lcdc.lcd_on)
			{
				data = 0b10000000 | (regs.lcd.stat.raw & 0b01111111);
			} else {
				data = 0b10000000 | (regs.lcd.stat.raw & 0b01111100);
			}
			break;
		case ADDR_SCY:
			data = regs.lcd.scy;
			break;
		case ADDR_SCX:
			data = regs.lcd.scx;
			break;
		case ADDR_LY:
			data = regs.lcd.ly;
			break;
		case ADDR_LYC:
			data = regs.lcd.lyc;
			break;
		case ADDR_DMA:
			data = regs.lcd.dma;
			break;
		case ADDR_BGP:
			data = regs.lcd.bgp.raw;
			break;
		case ADDR_OBP0:
			data = regs.lcd.obp0.raw;
			break;
		case ADDR_OBP1:
			data = regs.lcd.obp1.raw;
			break;
		case ADDR_WY:
			data = regs.lcd.wy;
			break;
		case ADDR_WX:
			data = regs.lcd.wx;
			break;
		case ADDR_IF:
			data = regs.int_request;
			break;
		default:
			data = 0;
			break;
		}
	}
	else if (address >= 0xFF80 && address <= 0xFFFE)
	{
		data = map->high_ram[address - 0xFF80];
	}
	else if (address == 0xFFFF)
	{
		data = regs.int_enable;
	}

	return data;
}

void Bus::Clock()
{
	uint8_t cycles = cpu.Step();
	if (!cpu.stop)
	{
		ppu.UpdateScreen(cycles);
		UpdateTimers(cycles);
	}

#ifdef D_SERIAL_OUT
	// Get serial output from blargg tests
	if (regs.serial.sc.raw == 0x81) {
		printf("%c", regs.serial.sb);
		regs.serial.sc.raw = 0x00;
	}
#endif
}

void Bus::UpdateTimers(uint8_t cycles)
{
	// Update internal counter
	regs.counter.raw += cycles * 4;

	if (regs.timer.tac.enabled)
	{
		uint8_t bit = 0;
		switch (regs.timer.tac.clock)
		{
		case 0:
			bit = 9;
			break;
		case 1:
			bit = 3;
			break;
		case 2:
			bit = 5;
			break;
		case 3:
			bit = 7;
			break;
		}

		if (!(regs.counter.raw & (1 << bit)) && pulseTimer)
		{
			if (regs.timer.tima == 0xFF)
			{
				regs.timer.tima = regs.timer.tma;
				BIT_SET(regs.int_request, INT_TIMER);
			}
			else
			{
				regs.timer.tima++;
			}

			pulseTimer = false;
		}
		else if ((regs.counter.raw & (1 << bit)))
		{
			pulseTimer = true;
		}
	}
}

// For memory browser. Excludes program area
Memory::Map& Bus::GetMemoryDump()
{
	return *map;
}

void Bus::dmaTransfer(uint8_t data)
{
	uint16_t address = data * 0x100;
	for (unsigned int i = 0; i < map->oam.size(); i++)
	{
		uint8_t oam_data = Read(address + i);
		Write(0xFE00 + i, oam_data);
	}
}