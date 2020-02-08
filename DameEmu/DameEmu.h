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
	//Misc
	void NOP();
	void STOP();
	void HALT();
	void DI();
	void EI();
	//Load instructions
	void LD_A_A();	
	void LD_A_B();	
	void LD_A_BC();	
	void LD_A_C();	
	void LD_A_Ca();	
	void LD_A_D();	
	void LD_A_DE();	
	void LD_A_E();	
	void LD_A_H();	
	void LD_A_HL();	
	void LD_A_HLD();
	void LD_A_HLI();
	void LD_A_L();	
	void LD_A_n();	
	void LD_A_nn();	
	void LD_B_A();	
	void LD_B_B();	
	void LD_B_C();	
	void LD_B_D();	
	void LD_B_E();	
	void LD_B_H();	
	void LD_B_HL();	
	void LD_B_L();	
	void LD_B_n();	
	void LD_BC();	
	void LD_BC_A();	
	void LD_C_A();	
	void LD_C_B();	
	void LD_C_C();	
	void LD_C_D();	
	void LD_C_E();	
	void LD_C_H();	
	void LD_C_HL();	
	void LD_C_L();	
	void LD_C_n();	
	void LD_Ca_A();	
	void LD_D_A();	
	void LD_D_B();	
	void LD_D_C();	
	void LD_D_D();	
	void LD_D_E();	
	void LD_D_H();	
	void LD_D_HL();	
	void LD_D_L();	
	void LD_D_n();	
	void LD_DE();	
	void LD_DE_A();	
	void LD_E_A();	
	void LD_E_B();	
	void LD_E_C();	
	void LD_E_D();	
	void LD_E_E();	
	void LD_E_H();	
	void LD_E_HL();	
	void LD_E_L();	
	void LD_E_n();
	void LD_H_A();	
	void LD_H_B();	
	void LD_H_C();	
	void LD_H_D();	
	void LD_H_E();	
	void LD_H_H();	
	void LD_H_HL();	
	void LD_H_L();	
	void LD_H_n();	
	void LD_HL();	
	void LD_HL_A();	
	void LD_HL_B();	
	void LD_HL_C();	
	void LD_HL_D();	
	void LD_HL_E();	
	void LD_HL_H();	
	void LD_HL_L();	
	void LD_HL_n();	
	void LD_HL_r(uint8_t& r);
	void LD_HLD_A();
	void LD_HLI_A();
	void LD_L_A();	
	void LD_L_B();	
	void LD_L_C();	
	void LD_L_D();	
	void LD_L_E();	
	void LD_L_H();	
	void LD_L_HL();	
	void LD_L_L();	
	void LD_L_n();
	void LD_nn_A();	
	void LD_r_HL(uint8_t& r);
	void LD_r_n(uint8_t& r);
	void LD_r_r(uint8_t& r1, uint8_t& r2);
	void LD_SP();	
	void LDH_A_n();	
	void LDH_n_A();	
	void LD_nn_SP();
	void LD_HL_SP_e();
	void LD_SP_HL();
	void POP(uint16_t& rr);
	void POP_AF();
	void POP_BC();
	void POP_DE();
	void POP_HL();
	void PUSH(uint16_t& rr);
	void PUSH_AF();
	void PUSH_BC();
	void PUSH_DE();
	void PUSH_HL();
	//Arithmetic
	void ADD_HL(uint16_t rr);
	void ADD_HL_BC();
	void ADD_HL_DE();
	void ADD_HL_HL();
	void ADD_HL_SP();
	void ADD_SP_e();
	void ADD(uint8_t r);
	void ADD_A();
	void ADD_B();
	void ADD_C();
	void ADD_D();
	void ADD_E();
	void ADD_H();
	void ADD_L();
	void ADD_HL();
	void ADD_n();
	void ADC(uint8_t r);
	void ADC_A();
	void ADC_B();
	void ADC_C();
	void ADC_D();
	void ADC_E();
	void ADC_H();
	void ADC_L();
	void ADC_HL();
	void ADC_n();
	void SUB(uint8_t r);
	void SUB_A();
	void SUB_B();
	void SUB_C();
	void SUB_D();
	void SUB_E();
	void SUB_H();
	void SUB_L();
	void SUB_HL();
	void SUB_n();
	void SBC(uint8_t r);
	void SBC_A();
	void SBC_B();
	void SBC_C();
	void SBC_D();
	void SBC_E();
	void SBC_H();
	void SBC_L();
	void SBC_HL();
	void SBC_n();
	void AND(uint8_t r);
	void AND_A();
	void AND_B();
	void AND_C();
	void AND_D();
	void AND_E();
	void AND_H();
	void AND_L();
	void AND_HL();
	void AND_n();
	void CP(uint8_t r);
	void CP_A();
	void CP_B();
	void CP_C();
	void CP_D();
	void CP_E();
	void CP_H();
	void CP_L();
	void CP_HL();
	void CP_n();
	void DAA();
	void CPL();
	void CCF();
	void SCF();
	void DEC(uint16_t& rr);
	void DEC(uint8_t& r);
	void DEC_A();
	void DEC_B();
	void DEC_BC();
	void DEC_C();
	void DEC_D();
	void DEC_DE();
	void DEC_E();
	void DEC_H();
	void DEC_L();
	void DEC_HL();
	void DEC_SP();
	void DEC_HLP();
	void INC(uint16_t& rr);
	void INC(uint8_t& r);
	void INC_A();
	void INC_B();
	void INC_BC();
	void INC_C();
	void INC_D();
	void INC_DE();
	void INC_E();
	void INC_H();
	void INC_L();
	void INC_HL();
	void INC_SP();
	void INC_HLP();
	void OR(uint8_t r);
	void OR_A();
	void OR_B();
	void OR_C();
	void OR_D();
	void OR_E();
	void OR_H();
	void OR_L();
	void OR_HL();
	void OR_n();
	void XOR(uint8_t r);
	void XOR_A();
	void XOR_B();
	void XOR_C();
	void XOR_D();
	void XOR_E();
	void XOR_H();
	void XOR_L();
	void XOR_HL();
	void XOR_n();
	//Accumalator rotates
	void RLA();
	void RLCA();
	void RRA();
	void RRCA();
	//Control Flow
	void CALL();
	void CALL_C();
	void CALL_NC();
	void CALL_NZ();
	void CALL_Z();
	void JP();
	void JP_C();
	void JP_HL();
	void JP_NC();
	void JP_NZ();
	void JP_Z();
	void JR();
	void JR_C();
	void JR_NC();
	void JR_NZ();
	void JR_Z();
	void RET();
	void RET_C();
	void RET_NC();
	void RET_NZ();
	void RET_Z();
	void RETI();
	void RST(uint8_t t);
	void RST_00();
	void RST_08();
	void RST_10();
	void RST_18();
	void RST_20();
	void RST_28();
	void RST_30();
	void RST_38();
	//CB Instructions
	void CB();
	//Bit operations
	void BIT(uint8_t b, uint8_t& r);
	void BIT_0_A();
	void BIT_0_B();
	void BIT_0_C();
	void BIT_0_D();
	void BIT_0_E();
	void BIT_0_H();
	void BIT_0_HL();
	void BIT_0_L();
	void BIT_1_A();
	void BIT_1_B();
	void BIT_1_C();
	void BIT_1_D();
	void BIT_1_E();
	void BIT_1_H();
	void BIT_1_HL();
	void BIT_1_L();
	void BIT_2_A();
	void BIT_2_B();
	void BIT_2_C();
	void BIT_2_D();
	void BIT_2_E();
	void BIT_2_H();
	void BIT_2_HL();
	void BIT_2_L();
	void BIT_3_A();
	void BIT_3_B();
	void BIT_3_C();
	void BIT_3_D();
	void BIT_3_E();
	void BIT_3_H();
	void BIT_3_HL();
	void BIT_3_L();
	void BIT_4_A();
	void BIT_4_B();
	void BIT_4_C();
	void BIT_4_D();
	void BIT_4_E();
	void BIT_4_H();
	void BIT_4_HL();
	void BIT_4_L();
	void BIT_5_A();
	void BIT_5_B();
	void BIT_5_C();
	void BIT_5_D();
	void BIT_5_E();
	void BIT_5_H();
	void BIT_5_HL();
	void BIT_5_L();
	void BIT_6_A();
	void BIT_6_B();
	void BIT_6_C();
	void BIT_6_D();
	void BIT_6_E();
	void BIT_6_H();
	void BIT_6_HL();
	void BIT_6_L();
	void BIT_7_A();
	void BIT_7_B();
	void BIT_7_C();
	void BIT_7_D();
	void BIT_7_E();
	void BIT_7_H();
	void BIT_7_HL();
	void BIT_7_L();
	void BIT_HL(uint8_t b);
	//Rotate
	void RL(uint8_t& r);
	void RL_A();
	void RL_B();
	void RL_C();
	void RL_D();
	void RL_E();
	void RL_H();
	void RL_HL();
	void RL_L();
	void RLC(uint8_t& r);
	void RLC_A();
	void RLC_B();
	void RLC_C();
	void RLC_D();
	void RLC_E();
	void RLC_H();
	void RLC_HL();
	void RLC_L();
	void RR(uint8_t& r);
	void RR_A();
	void RR_B();
	void RR_C();
	void RR_D();
	void RR_E();
	void RR_H();
	void RR_HL();
	void RR_L();
	void RRC(uint8_t& r);
	void RRC_A();
	void RRC_B();
	void RRC_C();
	void RRC_D();
	void RRC_E();
	void RRC_H();
	void RRC_HL();
	void RRC_L();
};