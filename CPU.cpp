#include "CPU.h"
#include "InstructionSet.h"
#include "Bus.h"


CPU::CPU(Bus* b)
{
	bus = b;

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

	if (bus->regs.int_enable.vblank) {
		if (bus->regs.int_request.vblank) {
			interupt_master_enable = false;
			bus->regs.int_request.vblank = 0;
			bus->Write(SP - 1, (PC & 0xFF00) >> 8);
			bus->Write(SP - 2, (PC & 0x00FF));
			SP = SP - 2;
			PC = 0x0040;
			cycles += 5;
		}
	}

	//TODO: Service other interupts
}

void CPU::write(uint16_t address, uint8_t data)
{
	bus->Write(address, data);
}

uint8_t CPU::read(uint16_t address)
{
	return bus->Read(address);
}

inline uint8_t CPU::GetByteAtPC() {
	return bus->Read(PC++);
}

inline uint16_t CPU::GetWordAtPC() {
	uint16_t word;
	word = bus->Read(PC++);
	word |= bus->Read(PC++) << 8;
	return word;
}

uint8_t CPU::Step() {
	cycles = 0;
	HandleInterupts();

	uint8_t opcode = GetByteAtPC();

	switch (insTable[opcode].getLength()) {
	case 1:
		break;
	case 2:
		operand = GetByteAtPC();
		break;
	case 3:
		operand = GetWordAtPC();
		break;
	default:
		break;
	};

	(this->*jumpTable[opcode].execute)();

	return cycles;
}

void CPU::CB() {
	uint8_t opcode = static_cast<uint8_t>(operand);
	(this->*cb_jumpTable[opcode].execute)();
}