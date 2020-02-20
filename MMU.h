#pragma once
#include <cstdint>
#include <cstdint>

// Port/Mode Registers
constexpr auto REG_P1	= 0xFF00;
constexpr auto REG_SB	= 0xFF01;
constexpr auto REG_SC	= 0xFF02;
constexpr auto REG_DIV	= 0xFF04;
constexpr auto REG_TIMA = 0xFF05;
constexpr auto REG_TMA	= 0xFF06;
constexpr auto REG_TAC	= 0xFF07;
// Sound Registers
// Sound Mode 1
constexpr auto REG_NR10 = 0xFF10;
constexpr auto REG_NR11 = 0xFF11;
constexpr auto REG_NR12 = 0xFF12;
constexpr auto REG_NR13 = 0xFF13;
constexpr auto REG_NR14 = 0xFF14;
// Sound Mode 2
constexpr auto REG_NR21 = 0xFF16;
constexpr auto REG_NR22 = 0xFF17;
constexpr auto REG_NR23 = 0xFF18;
constexpr auto REG_NR24 = 0xFF19;
// Sound Mode 3
constexpr auto REG_NR30 = 0xFF1A;
constexpr auto REG_NR31 = 0xFF1B;
constexpr auto REG_NR32 = 0xFF1C;
constexpr auto REG_NR33 = 0xFF1D;
constexpr auto REG_NR34 = 0xFF1E;
// Sound Mode 4
constexpr auto REG_NR41 = 0xFF20;
constexpr auto REG_NR42 = 0xFF21;
constexpr auto REG_NR43 = 0xFF22;
constexpr auto REG_NR44 = 0xFF23;
// Sound Control
constexpr auto REG_NR50 = 0xFF24;
constexpr auto REG_NR51 = 0xFF25;
constexpr auto REG_NR52 = 0xFF26;
// Wave RAM
constexpr auto WAVE_RAM = 0xFF30;
constexpr auto WAVE_RAM_SIZE = 0x10;
// LCD Display Registers
constexpr auto REG_LCDC	= 0xFF40;
constexpr auto REG_STAT	= 0xFF41;
constexpr auto REG_SCY	= 0xFF42;
constexpr auto REG_SCX	= 0xFF43;
constexpr auto REG_LY	= 0xFF44;
constexpr auto REG_LYC	= 0xFF45;
constexpr auto REG_DMA	= 0xFF46;
constexpr auto REG_BGP	= 0xFF47;
constexpr auto REG_OBP0	= 0xFF48;
constexpr auto REG_OBP1	= 0xFF49;
constexpr auto REG_WY	= 0xFF4A;
constexpr auto REG_WX	= 0xFF4B;
// Unmap DMG boot ROM
// Maybe necessary to emulate boot ROM
constexpr auto REG_ROM	= 0xFF50;
// Interupt Registers
constexpr auto REG_IF	= 0xFF0F;
constexpr auto REG_IE	= 0xFFFF;


namespace Memory {
	namespace Registers {
		union Interupt {
			Interupt(uint8_t& r) : state(&r) {}
			uint8_t* state;
			struct {
				unsigned vblank : 1;
				unsigned lcdc : 1;
				unsigned timer : 1;
				unsigned serial : 1;
				unsigned input : 1;
			} *field;
			
		};

		struct LCD {
			union {
				uint8_t* state;
				struct {
					unsigned bg_on : 1;
					unsigned obj_on : 1;
					unsigned obj_flag : 1;
					unsigned bg_area_flag : 1;
					unsigned bg_data_flag : 1;
					unsigned window_on : 1;
					unsigned window_area_flag : 1;
					unsigned lcd_on : 1;
				} *field;
			} lcdc;

			union {
				uint8_t* state;
				struct {
					unsigned mode_flag : 2;
					unsigned match_flag : 1;
					unsigned interupt_selection : 4;
				} *field;
			} stat;
			
			uint8_t& scy;
			uint8_t& scx;
			uint8_t& ly;
			uint8_t& lyc;
			uint8_t& dma;
			uint8_t& bgp;
			uint8_t& obp0;
			uint8_t& obp1;
			uint8_t& wy;
			uint8_t& wx;
		};
	};

	class MMU {
	public:
		MMU();
		~MMU();

		void LoadCartridge(const char* rom_dir);
		uint8_t Read(uint16_t address);
		void Write(uint16_t address, uint8_t value);

		Registers::Interupt* interupt_flag;
		Registers::Interupt* interupt_enable;
		Registers::LCD* lcd;
	private:
		void Reset();

		//Memory map
		union {
			uint8_t memory_map[0x10000];
			struct {
				uint8_t ROM[0x8000];			//0000h - 7FFFh
				uint8_t VRAM[0x2000];			//8000h - 9FFFh
				uint8_t externalRAM[0x2000];	//A000h - BFFFh
				uint8_t WRAM[0x2000];			//C000h - DFFFh
				uint8_t echoRAM[0x1E00];		//E000h - FDFFh
				uint8_t OAM[0xA0];				//FE00h - FE9Fh
				uint8_t unusable[0x60];			//FEA0h - FEFFh
				uint8_t IO_Regs[0x80];			//FF00h - FF7Fh
				uint8_t highRAM[0x7F];			//FF80h - FFFEh
			};
		};
	};
};