#include "DameEmu.h"
#include <fstream>

#ifdef _DEBUG
#include <cstdio>
#define debug_msg(...) printf(__VA_ARGS__)
#else
#define debug_msg(x)
#endif

#define nn (uint16_t)(memory[PC + 2] << 8 | memory[PC + 1])
#define n (uint8_t)(memory[PC + 1])
#define e (int8_t)(memory[PC + 1])

//Interrupt flags
#define INT_FLAG_INPUT  (1 << 4)
#define INT_FLAG_SERIAL (1 << 3)
#define INT_FLAG_TIMER  (1 << 2)
#define INT_FLAG_LCDC   (1 << 1)
#define INT_FLAG_VBLANK (1)

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