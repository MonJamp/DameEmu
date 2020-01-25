#include "DameEmu.h"
#include <fstream>
#include <iostream>

DameEmu::DameEmu(const char* ROM_DIR, const char* BIOS_DIR) {
	BootUp(ROM_DIR, BIOS_DIR);
}

DameEmu::~DameEmu() {
	if (app != NULL) {
		app->close();
	}

	delete(app);
}

void DameEmu::BootUp(const char* ROM_DIR, const char* BIOS_DIR) {
	std::ifstream BIOS;
	BIOS.open(BIOS_DIR, std::ios::binary);
	for (uint8_t i = 0x0; BIOS.get((char&)memory[i]); i++);

	std::ifstream ROM;
	ROM.open(ROM_DIR, std::ios::binary);
	for (uint8_t i = ROM_ENTRY; ROM.get((char&)memory[i]); i++);
}