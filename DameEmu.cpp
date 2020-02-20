#include "DameEmu.h"
#include <fstream>

DameEmu::DameEmu(const char* rom_dir) :
	mmu(),
	cpu(mmu),
	ppu(mmu)
{
	mmu.LoadCartridge(rom_dir);
}

DameEmu::~DameEmu() {
	if (app != NULL) {
		app->close();
	}

	delete(app);
}

void DameEmu::Run() {
	while (true) {
		uint8_t cycles = cpu.Step();
		ppu.UpdateScreen(cycles);

#ifdef D_SERIAL_OUT
		// Get serial output from blargg tests
		if (mmu.Read(REG_SC) == 0x81) {
			printf("%c", mmu.Read(REG_SB));
			mmu.Write(REG_SC, 0x00);
		}
#endif
	}
}