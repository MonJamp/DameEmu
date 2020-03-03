#pragma once
#include "Bus.h"
#include <string>
#include <cstdint>


class DameEmu {
public:
	DameEmu(const char* rom_dir);
	~DameEmu();

	void Step();

private:
	Bus bus;
};