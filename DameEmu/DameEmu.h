#pragma once
#include <SFML/Graphics.hpp>

#include <cstdint>

#define LCD_HEIGHT 166
#define LCD_WIDTH 144

#define ROM_ENTRY 0x150

//Flag registers
#define FLAG_Z   ((F & 0b10000000) >> 7)
#define FLAG_N   ((F & 0b01000000) >> 6)
#define FLAG_H   ((F & 0b00100000) >> 5)
#define FLAG_CY  ((F & 0b00010000) >> 4)
#define SET_Z   (F |= 1 << 7)
#define SET_N   (F |= 1 << 6)
#define SET_H   (F |= 1 << 5)
#define SET_CY  (F |= 1 << 4)
#define CLEAR_Z   (F ^= 1 << 7)
#define CLEAR_N   (F ^= 1 << 6)
#define CLEAR_H   (F ^= 1 << 5)
#define CLEAR_CY  (F ^= 1 << 4)


class DameEmu {
public:
	DameEmu(const char* ROM_DIR, const char* BIOS_DIR);
	~DameEmu();

	void BootUp(const char* ROM_DIR, const char* BIOS_DIR);
	void Cycle();

	sf::RenderWindow* GetApp() { return app; };

private:
	sf::RenderWindow* app;

	//CPU registers
	union {
		uint16_t AF;
		struct {
			uint8_t F;
			uint8_t A;
		};
	};

	union {
		uint16_t BC;
		struct {
			uint8_t C;
			uint8_t B;
		};
	};

	union {
		uint16_t DE;
		struct {
			uint8_t E;
			uint8_t D;
		};
	};

	union {
		uint16_t HL;
		struct {
			uint8_t L;
			uint8_t H;
		};
	};

	uint16_t PC;
	uint16_t SP;

	//Memory map
	union {
		uint8_t memory[0xFFFF];
		struct {
			uint8_t ROM[0x8000];			//0000h - 7FFFh
			uint8_t vRAM[0x2000];			//8000h - 9FFFh
			uint8_t externalRAM[0x2000];	//A000h - BFFFh
			uint8_t workRAM[0x2000];		//C000h - DFFFh
			uint8_t echoRAM[0x1DFF];		//E000h - FDFFh
			//Internal CPU RAM
			uint8_t oam[0xA0];				//FE00h - FE9Fh
			uint8_t unusable[0x60];			//FEA0h - FEFFh
			uint8_t ioRegisters[0x80];		//FF00h - FF7Fh
			uint8_t stackRAM[0x7F];			//FF80h - FFFEh
			uint8_t interruptsEnable;		//FFFFh - FFFFh
		};
	};
};