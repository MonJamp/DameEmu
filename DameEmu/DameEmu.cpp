#include "DameEmu.h"
#include <fstream>

DameEmu::DameEmu(const char* ROM_DIR) :
	mmu(),
	cpu(mmu),
	ppu(mmu)
{
	BootUp(ROM_DIR);
}

DameEmu::~DameEmu() {
	if (app != NULL) {
		app->close();
	}

	delete(app);
}

void DameEmu::BootUp(const char* ROM_DIR) {
	mmu.LoadCartridge(ROM_DIR);
}

void DameEmu::Run() {
	while (true) {
		uint8_t cycles = cpu.Step();
		ppu.UpdateScreen(cycles);
	}
}