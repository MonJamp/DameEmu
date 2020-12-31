#include "Mapper.h"

Mapper::Mapper(std::vector<RomBank*>& romBanks, std::vector<RamBank*>& ramBanks)
	: romBanks(romBanks), ramBanks(ramBanks)
{
}

Mapper::~Mapper()
{
}

uint8_t Mapper::ROM_Read(const uint16_t& address)
{
	if (address < 0x4000)
	{
		return romBanks[0]->at(address);
	}
	else
	{
		return romBanks[1]->at(address % 0x4000);
	}
}

void Mapper::ROM_Write(const uint16_t& address, const uint8_t& data)
{
}

uint8_t Mapper::RAM_Read(const uint16_t& address)
{
	return ramBanks[0]->at(address);
}

void Mapper::RAM_Write(const uint16_t& address, const uint8_t& data)
{
	ramBanks[0]->at(address) = data;
}
