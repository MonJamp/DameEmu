#include "CPU.h"

#ifdef D_LOG_INS
#include <cstdio>
#define debug_msg(...) printf(__VA_ARGS__)
#else
#define debug_msg(...)
#endif

CPU::CPU(Memory::MMU& mmu) :
	mmu(mmu)
{
	Reset();
}

void CPU::Reset() {
	cycles = 0;
	interupt_master_enable = false;

	//Gameboy boot state after bios
	//These values are true only for DMG
	//TODO: Display scrolling logo and boot sound
	AF = 0x01B0;
	BC = 0x0013;
	DE = 0x00D8;
	SP = 0xFFFE;
	PC = 0x0100;
}

//Service interrupt if IME is enabled and corresponding IE/IF fields are set
//When servicing IME is disabled, corresponding IF is disabled,
//PC is pushed to stack and jumps to interrupt handler address
//This process takes 5 cycles
void CPU::HandleInterupts() {
	if (!interupt_master_enable) {
		return;
	}

	if (mmu.interupt_enable->field->vblank) {
		if (mmu.interupt_flag->field->vblank) {
			interupt_master_enable = false;
			mmu.interupt_flag->field->vblank = 0;
			mmu.Write(SP - 1, (PC & 0xFF00) >> 8);
			mmu.Write(SP - 2, (PC & 0x00FF));
			SP = SP - 2;
			PC = 0x0040;
			cycles += 5;

			debug_msg("VBlank service started...\n");
		}
	}

	//TODO: Service other interupts
}

inline uint8_t CPU::GetByteAtPC() {
	return mmu.Read(PC++);
}

inline uint16_t CPU::GetWordAtPC() {
	uint16_t word;
	word = mmu.Read(PC++);
	word |= mmu.Read(PC++) << 8;
	return word;
}

uint8_t CPU::Step() {
	cycles = 0;
	HandleInterupts();

	debug_msg("%04X: ", PC);

	uint8_t opcode = GetByteAtPC();
	Instruction ins = instructions[opcode];

	switch (ins.length) {
	case 1:
		debug_msg(ins.mnemonic);
		break;
	case 2:
		operand = GetByteAtPC();
		debug_msg(ins.mnemonic, operand);
		break;
	case 3:
		operand = GetWordAtPC();
		debug_msg(ins.mnemonic, operand);
		break;
	default:
		break;
	};

	(this->*ins.execute)();
	debug_msg("\n");

	return cycles;
}

void CPU::CB() {
	uint8_t opcode = static_cast<uint8_t>(operand);
	Instruction cb_ins = cb_instructions[opcode];

	debug_msg(cb_ins.mnemonic);

	(this->*cb_ins.execute)();
}