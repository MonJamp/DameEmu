#pragma once
#include <cstdint>

class DameEmu {
public:
//private:
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