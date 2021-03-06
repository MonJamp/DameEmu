#include "DameEmu.h"
#include <fstream>


DameEmu::DameEmu()
{
	bus = std::make_shared<Bus>();
	debugger = std::make_shared<Debugger>(bus);

	debugger->Disassemble();
}

DameEmu::DameEmu(const std::string& filename)
{
	bus = std::make_shared<Bus>();
	debugger = std::make_shared<Debugger>(bus);

	bus->InsertCartridge(filename);
	debugger->Disassemble();
}

DameEmu::~DameEmu()
{

}

void DameEmu::Step() {
	bus->Clock();
}

void DameEmu::Reset()
{
	bus->Reset();
}