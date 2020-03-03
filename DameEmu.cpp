#include "DameEmu.h"
#include "Cartridge.h"
#include <fstream>

DameEmu::DameEmu(const char* rom_dir)
{
	std::shared_ptr<Cartridge> cart;
	cart->open(rom_dir);
	bus.InsertCartridge(cart);
}

DameEmu::~DameEmu()
{

}

void DameEmu::Step() {
	bus.Clock();
}