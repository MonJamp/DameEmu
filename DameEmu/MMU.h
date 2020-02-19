#pragma once
#include <cstdint>

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
				union {
					uint8_t inputOutput[0x80];	//FF00h - FF7Fh
					struct {
						//Ports
						uint8_t P1;		//Joypad		- FF00h
						uint8_t SB;		//Serial Data	- FF01h
						uint8_t SC;		//Serial Control- FF02h
						uint8_t DIV;	//Divider		- FF04h
						uint8_t TIMA;	//Timer			- FF05h
						uint8_t TMA;	//Timer Modulo	- FF06h
						uint8_t TAC;	//Timer Control	- FF07h
						uint8_t unused1[0x8];	//FF08h - FF0Eh
						uint8_t InterruptFlag;			//FF0Fh
						//Sound Mode 1 registers
						uint8_t NR10;	//Sweep			- FF10h
						uint8_t NR11;	//Length-Wave	- FF11h
						uint8_t NR12;	//Envelope		- FF12h
						uint8_t NR13;	//Frequency lo	- FF13h
						uint8_t NR14;	//Frequency hi	- FF14h
						uint8_t unused2;	//Unused
						//Sound Mode 2 registers
						uint8_t NR21;	//Length-Wave	- FF16h
						uint8_t NR22;	//Envelope		- FF17h
						uint8_t NR23;	//Frequency lo	- FF18h
						uint8_t NR24;	//Frequency hi	- FF19h
						//Sound Mode 3 registers
						uint8_t NR30;	//On-Off		- FF1Ah
						uint8_t NR31;	//Length		- FF1Bh
						uint8_t NR32;	//Output level	- FF1Ch
						uint8_t NR33;	//Frequency lo	- FF1Dh
						uint8_t NR34;	//Frequency hi	- FF1Eh
						uint8_t unused4;	//Unused
						//Sound Mode 4 registers
						uint8_t NR41;	//Length		- FF20h
						uint8_t NR42;	//Envelope		- FF21h
						uint8_t NR43;	//Poly Counter	- FF22h
						uint8_t NR44;	//Consecutive	- FF23h
						//General Sound registers
						uint8_t NR50;	//Channel-Vol	- FF24h
						uint8_t NR51;	//Selection		- FF25h
						uint8_t NR52;	//Master On-Off	- FF26h
						uint8_t unused5[0x9]; //Unused
						uint8_t WaveRam[0x10];	//FF30h - FF3Fh
						//LCD Display registers
						uint8_t LCDC;	//LCD Control	- FF40h
						uint8_t STAT;	//LCDC Status	- FF41h
						uint8_t SCY;	//Scroll Y		- FF42h
						uint8_t SCX;	//Scroll X		- FF43h
						uint8_t LY;		//LCDC Y-Coord	- FF44h
						uint8_t LYC;	//LY Compare	- FF45h
						uint8_t DMA;	//DMA Address	- FF46h
						uint8_t BGP;	//BG-Win Palette- FF47h
						uint8_t OBP0;	//Sprite Pallete- FF48h
						uint8_t OBP1;	//Sprite Pallete- FF49h
						uint8_t WY;		//Window Y-Pos	- FF4Ah
						uint8_t WX;		//Window X-Pos	- FF4Bh
						//At FF50h there is a register which when written to
						//unmaps the boot ROM and maps the cartridge in its place
					};
				};
				uint8_t highRAM[0x7F];			//FF80h - FFFEh
				uint8_t InterruptEnable;				//FFFFh
			};
		};
	};
};