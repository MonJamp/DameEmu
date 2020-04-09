#include "DameEmu.h"
#include "Cartridge.h"
#include <fstream>

DameEmu::DameEmu(std::shared_ptr<Cartridge>& cart)
{
	bus.reset(new Bus);
	bus->InsertCartridge(cart);
	debugger.reset(new Debugger(bus));
}

DameEmu::~DameEmu()
{

}

void DameEmu::SetCanvas(sf::RenderWindow* canvas)
{
	bus->ppu.SetCanvas(canvas);
}

void DameEmu::Step() {
	bus->Clock();
}