#include "CPU.h"
#include "InstructionSet.h"
#include "Bus.h"


CPU::CPU(Bus* b)
	: bus(b)
{
	Reset();
}

void CPU::Reset() {
	cycles = 0;
	interupt_master_enable = false;
	halt = false;
	stop = false;

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
void CPU::HandleInterupts()
{
	uint8_t service = bus->regs.int_enable & bus->regs.int_request;
	// There are interupts which can be serviced
	if (service)
	{
		// TODO handle instructions when in halt mode and ime=0
		// When IME is disabled and system is halted the next instruction is skipped
		// Apparently this doesn't occur on CGB mode
		// I've read contradicting information on this however
		halt = false;
		stop = false;

		if (interupt_master_enable)
		{
			if (BIT_CHECK(service, INT_VBLANK))
			{
				interupt_master_enable = false;
				BIT_CLEAR(bus->regs.int_request, INT_VBLANK);
				PUSH(PC);
				PC = 0x0040;
			}
			else if (BIT_CHECK(service, INT_LCDC))
			{
				interupt_master_enable = false;
				BIT_CLEAR(bus->regs.int_request, INT_LCDC);
				PUSH(PC);
				PC = 0x0048;
			}
			else if (BIT_CHECK(service, INT_TIMER))
			{
				interupt_master_enable = false;
				BIT_CLEAR(bus->regs.int_request, INT_TIMER);
				PUSH(PC);
				PC = 0x0050;
			}
			else if (BIT_CHECK(service, INT_SERIAL))
			{
				interupt_master_enable = false;
				BIT_CLEAR(bus->regs.int_request, INT_SERIAL);
				PUSH(PC);
				PC = 0x0058;
			}
			else if (BIT_CHECK(service, INT_INPUT))
			{
				interupt_master_enable = false;
				BIT_CLEAR(bus->regs.int_request, INT_INPUT);
				PUSH(PC);
				PC = 0x0060;
			}

			// PUSH increments cycles by 4 + 1 = 5
			cycles += 1;
		}
	}
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

	if (halt || stop)
	{
		return cycles;
	}

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
	uint8_t opcode = GetByteAtPC();
	(this->*cb_jumpTable[opcode].execute)();
}