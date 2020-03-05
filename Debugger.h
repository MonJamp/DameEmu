#pragma once
#include "Bus.h"
#include <cstdint>
#include <string>
#include <vector>


struct CpuState
{
	uint16_t PC;
	uint16_t SP;
	uint16_t AF;
	uint16_t BC;
	uint16_t DE;
	uint16_t HL;
	bool IME;
};

class Debugger
{
public:
	Debugger(Bus* b);

	void Step();

	CpuState GetCpuState();
private:
	Bus* bus;
};