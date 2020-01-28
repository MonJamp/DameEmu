#pragma once
#include <SFML/Graphics.hpp>
#include <string>
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

	//Store information about instructions
	//Also used as a hashmap to instruction's functions
	struct Instruction {
		const char* mnemonic;
		uint8_t length;
		void (DameEmu::*execute)();
	} static instructions[256], cb_instructions[256];

private:
	//Emulator variables
	sf::RenderWindow* app;
	EmuStatus status;
	//TODO: Implement cycles
	uint8_t cycles;
	//Instruction register stores current opcode with operands
	uint32_t IR;

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

	void UNDEFINED();
	void UNIMPLEMENTED();
	//8-bit load
	void LD_r_n(uint8_t& r);
	void LD_HL_n();
	void LD_r_r(uint8_t& r1, uint8_t& r2);
	void LD_r_HL(uint8_t& r);
	void LD_HL_r(uint8_t& r);
	//8-bit arithmetic
	void INC(uint8_t& r);
	void INC_HL();

	void LD_BC();						//01
	void LD_BC_A();						//02
	void INC_C();						//0C
	void LD_C_n();						//0E
	void LD_DE();						//11
	void LD_DE_A();						//12
	void JR_NZ();						//20
	void LD_HL();						//21
	void LD_HLI_A();					//22
	void LD_SP();						//31
	void LD_HLD_A();					//32
	void LD_A_n();						//3E
	void LD_HL_A();						//77
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
	void CB();							//CB
	void LD_C_A();						//E2
	void LD_A_C();						//F2

	//CB Instructions

	//Bit operations
	void BIT(uint8_t b, uint8_t& r);
	void BIT_HL(uint8_t b);

	void BIT_0_B();						//40
	void BIT_0_C();						//41
	void BIT_0_D();						//42
	void BIT_0_E();						//43
	void BIT_0_H();						//44
	void BIT_0_L();						//45
	void BIT_0_HL();					//46
	void BIT_0_A();						//47
	void BIT_1_B();						//48
	void BIT_1_C();						//49
	void BIT_1_D();						//4A
	void BIT_1_E();						//4B
	void BIT_1_H();						//4C
	void BIT_1_L();						//4D
	void BIT_1_HL();					//4E
	void BIT_1_A();						//4F
	void BIT_2_B();						//50
	void BIT_2_C();						//51
	void BIT_2_D();						//52
	void BIT_2_E();						//53
	void BIT_2_H();						//54
	void BIT_2_L();						//55
	void BIT_2_HL();					//56
	void BIT_2_A();						//57
	void BIT_3_B();						//58
	void BIT_3_C();						//59
	void BIT_3_D();						//5A
	void BIT_3_E();						//5B
	void BIT_3_H();						//5C
	void BIT_3_L();						//5D
	void BIT_3_HL();					//5E
	void BIT_3_A();						//5F
	void BIT_4_B();						//60
	void BIT_4_C();						//61
	void BIT_4_D();						//62
	void BIT_4_E();						//63
	void BIT_4_H();						//64
	void BIT_4_L();						//65
	void BIT_4_HL();					//66
	void BIT_4_A();						//67
	void BIT_5_B();						//68
	void BIT_5_C();						//69
	void BIT_5_D();						//6A
	void BIT_5_E();						//6B
	void BIT_5_H();						//6C
	void BIT_5_L();						//6D
	void BIT_5_HL();					//6E
	void BIT_5_A();						//6F
	void BIT_6_B();						//70
	void BIT_6_C();						//71
	void BIT_6_D();						//72
	void BIT_6_E();						//73
	void BIT_6_H();						//74
	void BIT_6_L();						//75
	void BIT_6_HL();					//76
	void BIT_6_A();						//77
	void BIT_7_B();						//78
	void BIT_7_C();						//79
	void BIT_7_D();						//7A
	void BIT_7_E();						//7B
	void BIT_7_H();						//7C
	void BIT_7_L();						//7D
	void BIT_7_HL();					//7E
	void BIT_7_A();						//7F
};