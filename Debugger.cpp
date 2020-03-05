#include "Debugger.h"

Debugger::Debugger(Bus* b)
{
	bus = b;
}

void Debugger::Step()
{
	bus->Clock();
}

CpuState Debugger::GetCpuState()
{
	CpuState cpuState;
	cpuState.PC = bus->cpu.PC;
	cpuState.SP = bus->cpu.SP;
	cpuState.AF = bus->cpu.AF;
	cpuState.BC = bus->cpu.BC;
	cpuState.DE = bus->cpu.DE;
	cpuState.HL = bus->cpu.HL;
	cpuState.IME = bus->cpu.interupt_master_enable;
	return cpuState;
}
