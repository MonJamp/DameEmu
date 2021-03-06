#pragma once
#include "CPU.h"
#include "PPU.h"
#include "Cartridge/Cartridge.h"
#include <cstdint>
#include <array>
#include <memory>


// Port/Mode Registers

constexpr auto ADDR_P1 = 0xFF00;
constexpr auto ADDR_SB = 0xFF01;
constexpr auto ADDR_SC = 0xFF02;
constexpr auto ADDR_DIV = 0xFF04;
constexpr auto ADDR_TIMA = 0xFF05;
constexpr auto ADDR_TMA = 0xFF06;
constexpr auto ADDR_TAC = 0xFF07;

// Sound Registers
// Sound Mode 1

constexpr auto ADDR_NR10 = 0xFF10;
constexpr auto ADDR_NR11 = 0xFF11;
constexpr auto ADDR_NR12 = 0xFF12;
constexpr auto ADDR_NR13 = 0xFF13;
constexpr auto ADDR_NR14 = 0xFF14;

// Sound Mode 2

constexpr auto ADDR_NR21 = 0xFF16;
constexpr auto ADDR_NR22 = 0xFF17;
constexpr auto ADDR_NR23 = 0xFF18;
constexpr auto ADDR_NR24 = 0xFF19;

// Sound Mode 3

constexpr auto ADDR_NR30 = 0xFF1A;
constexpr auto ADDR_NR31 = 0xFF1B;
constexpr auto ADDR_NR32 = 0xFF1C;
constexpr auto ADDR_NR33 = 0xFF1D;
constexpr auto ADDR_NR34 = 0xFF1E;

// Sound Mode 4

constexpr auto ADDR_NR41 = 0xFF20;
constexpr auto ADDR_NR42 = 0xFF21;
constexpr auto ADDR_NR43 = 0xFF22;
constexpr auto ADDR_NR44 = 0xFF23;

// Sound Control

constexpr auto ADDR_NR50 = 0xFF24;
constexpr auto ADDR_NR51 = 0xFF25;
constexpr auto ADDR_NR52 = 0xFF26;

// Wave RAM
constexpr auto ADDR_WAVE_RAM = 0xFF30;
constexpr auto WAVE_RAM_SIZE = 0x10;
// LCD Display Registers

constexpr auto ADDR_LCDC = 0xFF40;
constexpr auto ADDR_STAT = 0xFF41;
constexpr auto ADDR_SCY = 0xFF42;
constexpr auto ADDR_SCX = 0xFF43;
constexpr auto ADDR_LY = 0xFF44;
constexpr auto ADDR_LYC = 0xFF45;
constexpr auto ADDR_DMA = 0xFF46;
constexpr auto ADDR_BGP = 0xFF47;
constexpr auto ADDR_OBP0 = 0xFF48;
constexpr auto ADDR_OBP1 = 0xFF49;
constexpr auto ADDR_WY = 0xFF4A;
constexpr auto ADDR_WX = 0xFF4B;

// Unmap DMG boot ROM
// Maybe necessary to emulate boot ROM
constexpr auto ADDR_ROM = 0xFF50;
// Interupt Registers

constexpr auto ADDR_IF = 0xFF0F;
constexpr auto ADDR_IE = 0xFFFF;

// Bit operations

#define BIT_CHECK(x, y) ((x & (y)) == (y))
#define BIT_SET(x, y)	(x |= (y))
#define BIT_CLEAR(x, y) (x &= ~(y))

// Interupt flags

#define INT_VBLANK	(1 << 0)
#define INT_LCDC	(1 << 1)
#define INT_TIMER	(1 << 2)
#define INT_SERIAL	(1 << 3)
#define INT_INPUT	(1 << 4)

// STAT modes

// Mode 00
// CPU has access to VRAM
#define STAT_HBLANK 0b00
// Mode 01
#define STAT_VBLANK 0b01
// Mode 10
// LCD is reading OAM so it's not accessible to CPU
#define STAT_OAM	0b10
// Mode 11
// LCD is using both OAM and VRAM so it's not accessible to CPU
#define STAT_VRAM	0b11

namespace Memory {
	struct Registers {
		using Register = uint8_t;

		union P1 {
			uint8_t raw;
			struct {
				unsigned P10 : 1;
				unsigned P11 : 1;
				unsigned P12 : 1;
				unsigned P13 : 1;
				unsigned P14 : 1;
				unsigned P15 : 1;
			} field;
		} input;

		struct Serial {
			Register sb;

			union SC {
				uint8_t raw;
				struct {
					unsigned terminal : 1;
					unsigned rate : 1;
					unsigned unused : 5;
					unsigned transfer : 1;
				};
			} sc;
		} serial;

		struct Timer {
			Register tima;
			Register tma;

			union TAC {
				uint8_t raw;
				struct {
					// 0 = 4096  Hz : 1024 t clocks
					// 1 = 262144 Hz : 16 t clocks
					// 2 = 65536 Hz : 64 t clocks
					// 3 = 16386 Hz : 256 t clocks
					unsigned clock : 2;
					unsigned enabled : 1;
				};
			} tac;
		} timer;

		struct Sound {
			Register nr10;
			Register nr11;
			Register nr12;
			Register nr13;
			Register nr14;
			Register nr21;
			Register nr22;
			Register nr23;
			Register nr24;
			Register nr30;
			Register nr31;
			Register nr32;
			Register nr33;
			Register nr34;
			Register nr41;
			Register nr42;
			Register nr43;
			Register nr44;
			Register nr50;
			Register nr51;
			Register nr52;
			Register wav[WAVE_RAM_SIZE];
		} sound;

		// LCD Registers
		struct LCD {
			union LCDC {
				uint8_t raw;
				struct {
					// DMG: set = draw background
					// CGB: always set
					unsigned bg_on : 1;
					unsigned obj_on : 1;
					// OBJ Block Composition Selection
					// set = 8 x 16
					// clear = 8 x 8
					unsigned obj_flag : 1;
					// BG Code Area Selection
					// set = 0x9C00 - 0x9BFF
					// clear = 0x9800 - 0x9FFF
					unsigned bg_area_flag : 1;
					// BG Character Data Selection
					// set = 0x8000 - 0x8FFF
					// clear = 0x8800 - 0x97FF
					// When data is in 0x8800, 0x9000 is base ptr
					// and uses signed addressing
					unsigned bg_data_flag : 1;
					unsigned window_on : 1;
					// Window Code Area Selection
					// set = 0x9C00 - 0x9FFF
					// clear = 0x9800 - 0x9BFF
					unsigned window_area_flag : 1;
					// set = lcd is on
					// clear = lcd is off
					unsigned lcd_on : 1;
				};
			} lcdc;

			//TODO: Emulate STAT

			union STAT {
				uint8_t raw;
				struct {
					// Mode Flag
					// Read only
					unsigned mode_flag : 2;
					// Match Flag
					// Set when LYC = LY
					// Read only
					unsigned match_flag : 1;
					// Interupt during mode 00
					unsigned hblank_check : 1;
					// Interupt during mode 01
					unsigned vblank_check : 1;
					// Interupt during mode 10
					unsigned oam_check : 1;
					// Interupt when LYC == LY
					unsigned lyc_check : 1;
					unsigned unused : 1;
				};
			} stat;

			Register scy;
			Register scx;
			Register ly;
			Register lyc;
			Register dma;
			// BG Palette Data
			union BGP {
				uint8_t raw;
				struct {
					unsigned palette00 : 2;
					unsigned palette01 : 2;
					unsigned palette10 : 2;
					unsigned palette11 : 2;
				};
			} bgp;
			// OBJ Palette Data
			union OBP {
				uint8_t raw;
				struct {
					// Transparent
					unsigned palette00 : 2;
					unsigned palette01 : 2;
					unsigned palette10 : 2;
					unsigned palette11 : 2;
				};
			} obp0, obp1;
			// Window y-coordinate
			Register wy;
			// Window x-coordinate
			Register wx;
		} lcd;
	
		// Misc

		// Internal counter
		// div is the MSB of counter
		union Counter {
			uint16_t raw;
			struct {
				uint8_t unused;
				uint8_t div;
			};
		} counter;

		Register boot;
		Register int_enable;
		Register int_request;
	};

	struct Map
	{
		union
		{
			std::array<uint8_t, 0x8000> raw;
			struct {
				std::array<uint8_t, 0x2000> vram;
				std::array<uint8_t, 0x2000> eram;
				std::array<uint8_t, 0x2000> ram;
				std::array<uint8_t, 0x1E00> echo;
				std::array<uint8_t, 0x00A0> oam;
				std::array<uint8_t, 0x60> unused;
				std::array<uint8_t, 0x80> registers;
				std::array<uint8_t, 0x007F> high_ram;
				uint8_t int_enable;
			};
		};
	};
}


class Bus
{
friend class Debugger;
friend class DameEmu;
public:
	Bus();
	~Bus();

	void Reset();

	void InsertCartridge(const std::string& filename);
	void Write(uint16_t address, uint8_t data);
	uint8_t Read(uint16_t address);
	void Clock();

	Memory::Map& GetMemoryDump();
	std::array<uint8_t, 0x2000>* GetVRAM() { return &map->vram; }

	Memory::Registers regs;
private:
	void UpdateTimers(uint8_t cycles);
	void dmaTransfer(uint8_t data);
private:
	CPU cpu;
	PPU ppu;
	bool pulseTimer;
	std::unique_ptr<Cartridge> cart;
	std::shared_ptr<Memory::Map> map;

};