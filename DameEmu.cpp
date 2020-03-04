#include "DameEmu.h"
#include "Cartridge.h"
#include <fstream>

DameEmu::DameEmu(std::shared_ptr<Cartridge>& cart)
{
	bus.InsertCartridge(cart);
}

DameEmu::~DameEmu()
{

}

void DameEmu::Step() {
	bus.Clock();
}