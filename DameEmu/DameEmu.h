#pragma once
#include <SFML/Graphics.hpp>

#include <cstdint>

#define LCD_HEIGHT 166
#define LCD_WIDTH 144

#define ROM_ENTRY 0x150

//Flag registers
#define FLAG_ZERO		(1 << 7)
#define FLAG_NEGATIVE	(1 << 6)
#define FLAG_HALFCARRY	(1 << 5)
#define FLAG_CARRY		(1 << 4)

#define FLAG_CHECK(x)	(F & x)
#define FLAG_SET(x)		(F |= x)
#define FLAG_CLEAR(x)	(F &= ~x)


//For debugging instructions
enum EmuStatus {
	OK,
	HALT
};

class DameEmu {
public:
	DameEmu(const char* ROM_DIR, const char* BIOS_DIR);
	~DameEmu();

	void BootUp(const char* ROM_DIR, const char* BIOS_DIR);
	EmuStatus Cycle();

	sf::RenderWindow* GetApp() { return app; };

private:
	//Emulator variables
	sf::RenderWindow* app;
	//TODO: Implement cycles
	uint8_t cycles;

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

	//Instructions
	void UNKNOWN(uint8_t opcode);
	void UNKNOWN_CB(uint8_t opcode);
	//06 16 26 0E 1E 2E 3E
	void LD_r_n(uint8_t& r, uint8_t n);
	//36
	void LD_HL_n(uint8_t n);
	//40 41 42 43 44 45 47 48 49 4A 4B 4C 4D 4F
	//50 51 52 53 54 55 57 58 59 5A 5B 5C 5D 5F
	//60 61 62 63 64 65 67 68 69 6A 6B 6C 6D 6F
	//78 79 7A 7B 7C 7D 7F
	void LD_r_r(uint8_t& r1, uint8_t& r2);
	//46 4E 56 5E 66 6E 7E
	void LD_r_HL(uint8_t& r);
	//70 71 72 73 74 75 77
	void LD_HL_r(uint8_t& r);
	void LD_BC(uint16_t nn);			//01
	void LD_BC_A();						//02
	void LD_DE(uint16_t nn);			//11
	void LD_DE_A();						//12
	void JR_NZ(int8_t e);				//20
	void LD_HL(uint16_t nn);			//21
	void LD_HLI_A();					//22
	void LD_SP(uint16_t nn);			//31
	void LD_HLD_A();					//32
	//8-bit arithmetic
	void INC(uint8_t& r);
	void INC_HL();
	void AND_B();						//A0
	void AND_C();						//A1
	void AND_D();						//A2
	void AND_E();						//A3
	void AND_H();						//A4
	void AND_L();						//A5
	void AND_HL();						//A6
	void AND_A();						//A7
	void XOR_B();						//A8
	void XOR_C();						//A9
	void XOR_D();						//AA
	void XOR_E();						//AB
	void XOR_H();						//AC
	void XOR_L();						//AD
	void XOR_HL();						//AE
	void XOR_A();						//AF
	EmuStatus CB(uint8_t opcode);		//CB
	void LD_C_A();						//E2
	void LD_A_C();						//F2

	//CB Instructions
	//CB 40 41 42 43 44 45 47 48 49 4A 4B 4C 4D 4F
	//50 51 52 53 54 55 57 58 59 5A 5B 5C 5D 5F
	//60 61 62 63 64 65 67 68 69 6A 6B 6C 6D 6F
	//70 71 72 73 74 75 77 78 79 7A 7B 7C 7D 7F
	void BIT(uint8_t b, uint8_t& r);
	//CB 46 4E 56 5E 66 6E 76 7E
	void BIT_HL(uint8_t b);
};