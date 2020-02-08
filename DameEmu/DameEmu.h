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

//Returns 1 if bit is set
#define FLAG_CHECK(x)	((F & (x)) == (x))
#define FLAG_SET(x)		(F |= (x))
#define FLAG_CLEAR(x)	(F &= ~(x))


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
	void LD_r_r(uint8_t& r1, uint8_t& r2);
	void LD_r_HL(uint8_t& r);
	void LD_HL_r(uint8_t& r);
	void LD_E_n();
	void LD_L_n();
	void CP(uint8_t r);
	void CP_B();
	void CP_C();
	void CP_D();
	void CP_E();
	void CP_H();
	void CP_L();
	void CP_A();
	void CP_n();
	void CP_HL();
	void CPL();
	//8-bit arithmetic
	void INC(uint8_t& r);
	void INC(uint16_t& rr);
	void DEC(uint8_t& r);
	//16-bit load
	void POP(uint16_t& rr);
	void PUSH(uint16_t& rr);

	//Control Flow
	void RET();
	void RETI();
	void RET_NZ();
	void RET_Z();
	void RET_NC();
	void RET_C();
	void RST(uint8_t t);
	void RST_00();
	void RST_08();
	void RST_10();
	void RST_18();
	void RST_20();
	void RST_28();
	void RST_30();
	void RST_38();
	void JP();
	void JP_HL();
	void JP_NZ();
	void JP_Z();
	void JP_NC();
	void JP_C();
	void JR();
	void JR_NZ();
	void JR_Z();
	void JR_NC();
	void JR_C();
	void CALL();
	void CALL_NZ();
	void CALL_Z();
	void CALL_NC();
	void CALL_C();

	void LD_BC();						//01
	void LD_BC_A();						//02
	void INC_BC();						//03
	void INC_B();						//04
	void DEC_B();						//05
	void LD_B_n();						//06
	void RLCA();						//07
	void LD_A_BC();						//0A
	void INC_C();						//0C
	void DEC_C();						//0D
	void LD_C_n();						//0E
	void RRCA();						//0F
	void LD_DE();						//11
	void LD_DE_A();						//12
	void INC_DE();						//13
	void INC_D();						//14
	void DEC_D();						//15
	void LD_D_n();						//16
	void RLA();							//17
	void LD_A_DE();						//1A
	void INC_E();						//1C
	void DEC_E();						//1D
	void RRA();							//1F
	void LD_HL();						//21
	void LD_HLI_A();					//22
	void INC_HL();						//23
	void INC_H();						//24
	void DEC_H();						//25
	void LD_H_n();						//26
	void LD_A_HLI();					//2A
	void INC_L();						//2C
	void DEC_L();						//2D
	void LD_SP();						//31
	void LD_HLD_A();					//32
	void INC_SP();						//33
	void INC_HLP();						//34
	void DEC_HL();						//35
	void LD_HL_n();						//36
	void LD_A_HLD();					//3A
	void INC_A();						//3C
	void DEC_A();						//3D
	void LD_A_n();						//3E
	void LD_B_B();						//40
	void LD_B_C();						//41
	void LD_B_D();						//42
	void LD_B_E();						//43
	void LD_B_H();						//44
	void LD_B_L();						//45
	void LD_B_HL();						//46
	void LD_B_A();						//47
	void LD_C_B();						//48
	void LD_C_C();						//49
	void LD_C_D();						//4A
	void LD_C_E();						//4B
	void LD_C_H();						//4C
	void LD_C_L();						//4D
	void LD_C_HL();						//4E
	void LD_C_A();						//4F
	void LD_D_B();						//50
	void LD_D_C();						//51
	void LD_D_D();						//52
	void LD_D_E();						//53
	void LD_D_H();						//54
	void LD_D_L();						//55
	void LD_D_HL();						//56
	void LD_D_A();						//57
	void LD_E_B();						//58
	void LD_E_C();						//59
	void LD_E_D();						//5A
	void LD_E_E();						//5B
	void LD_E_H();						//5C
	void LD_E_L();						//5D
	void LD_E_HL();						//5E
	void LD_E_A();						//5F
	void LD_H_B();						//60
	void LD_H_C();						//61
	void LD_H_D();						//62
	void LD_H_E();						//63
	void LD_H_H();						//64
	void LD_H_L();						//65
	void LD_H_HL();						//66
	void LD_H_A();						//67
	void LD_L_B();						//68
	void LD_L_C();						//69
	void LD_L_D();						//6A
	void LD_L_E();						//6B
	void LD_L_H();						//6C
	void LD_L_L();						//6D
	void LD_L_HL();						//6E
	void LD_L_A();						//6F
	void LD_HL_B();						//70
	void LD_HL_C();						//71
	void LD_HL_D();						//72
	void LD_HL_E();						//73
	void LD_HL_H();						//74
	void LD_HL_L();						//75
	void LD_HL_A();						//77
	void LD_A_B();						//78
	void LD_A_C();						//79
	void LD_A_D();						//7A
	void LD_A_E();						//7B
	void LD_A_H();						//7C
	void LD_A_L();						//7D
	void LD_A_HL();						//7E
	void LD_A_A();						//7F
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
	void POP_BC();						//C1
	void PUSH_BC();						//C5
	void CB();							//CB
	void POP_DE();						//D1
	void PUSH_DE();						//D5
	void LDH_n_A();						//E0
	void POP_HL();						//E1
	void LD_Ca_A();						//E2
	void PUSH_HL();						//E5
	void LD_nn_A();						//EA
	void LDH_A_n();						//F0
	void POP_AF();						//F1
	void LD_A_Ca();						//F2
	void PUSH_AF();						//F5
	void LD_A_nn();						//FA

	//CB Instructions

	//Bit operations
	void BIT(uint8_t b, uint8_t& r);
	void BIT_HL(uint8_t b);
	//Rotate
	void RLC(uint8_t& r);
	void RRC(uint8_t& r);
	void RL(uint8_t& r);
	void RR(uint8_t& r);

	void RLC_B();						//00
	void RLC_C();						//01
	void RLC_D();						//02
	void RLC_E();						//03
	void RLC_H();						//04
	void RLC_L();						//05
	void RLC_HL();						//06
	void RLC_A();						//07
	void RRC_B();						//08
	void RRC_C();						//09
	void RRC_D();						//0A
	void RRC_E();						//0B
	void RRC_H();						//0C
	void RRC_L();						//0D
	void RRC_HL();						//0E
	void RRC_A();						//0F
	void RL_B();						//10
	void RL_C();						//11
	void RL_D();						//12
	void RL_E();						//13
	void RL_H();						//14
	void RL_L();						//15
	void RL_HL();						//16
	void RL_A();						//17
	void RR_B();						//18
	void RR_C();						//19
	void RR_D();						//1A
	void RR_E();						//1B
	void RR_H();						//1C
	void RR_L();						//1D
	void RR_HL();						//1E
	void RR_A();						//1F
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