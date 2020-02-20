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

#ifdef D_SERIAL_OUT
		// Get serial output from blargg tests
		if (mmu.Read(REG_SC) == 0x81) {
			printf("%c", mmu.Read(REG_SB));
			mmu.Write(REG_SC, 0x00);
		}
#endif
	}
}