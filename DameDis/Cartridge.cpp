#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge()
	: rom{0}, romSize(0)
{

}

Cartridge::~Cartridge()
{

}

// TODO: Check validity of cartridge
void Cartridge::open(const char* filename)
{
	std::ifstream file;
	file.open(filename, std::ios::binary);

	file.seekg(0, file.end);
	romSize = static_cast<size_t>(file.tellg());
	file.seekg(0, file.beg);

	file.read((char*)(&rom[0]), romSize);

	file.close();
}

uint8_t Cartridge::read(const uint16_t& address)
{
	return rom[address];
}

size_t Cartridge::size()
{
	return romSize;
}
