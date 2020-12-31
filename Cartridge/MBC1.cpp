#include "Mapper.h"
#include <cstdint>


// Sets all bits after msb
uint8_t GetBankBitMask(uint8_t x)
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;

	return x;
}

MBC1::MBC1(std::vector<RomBank*>& romBanks, std::vector<RamBank*>& ramBanks)
	: Mapper(romBanks, ramBanks)
{
	RAMG = 0;
	BANK1 = 1;
	BANK2 = 0;
	MODE = 0;

	romBankMask = GetBankBitMask(romBanks.size() - 1);
	ramBankMask = GetBankBitMask(ramBanks.size() - 1);
}

MBC1::~MBC1()
{
}

uint8_t MBC1::ROM_Read(const uint16_t& address)
{
	uint8_t romBankNum = 0;

	if (address < 0x4000)
	{
		if (MODE == 1)
		{
			romBankNum = BANK2 << 5;
		}
	}
	else
	{
		romBankNum = (BANK2 << 5) | BANK1;
	}

	romBankNum &= romBankMask;
	return romBanks[romBankNum]->at(address % ROM_BANK_SIZE);
}

void MBC1::ROM_Write(const uint16_t& address, const uint8_t& data)
{
	if (address < 0x2000)
	{
		RAMG = data;
	}
	else if (address < 0x4000)
	{
		BANK1 = data;

		if ((BANK1 % 0x10) == 0)
		{
			BANK1++;
		}
	}
	else if (address < 0x6000)
	{
		BANK2 = data;
	}
	else
	{
		MODE = data;
	}
}

uint8_t MBC1::RAM_Read(const uint16_t& address)
{
	uint8_t data = 0xFF;

	if (RAMG == 0x0A)
	{
		uint8_t ramBankNum = (MODE) ? BANK2 : 0;
		ramBankNum &= ramBankMask;
		data = ramBanks[ramBankNum]->at(address % RAM_BANK_SIZE);
	}

	return data;
}

void MBC1::RAM_Write(const uint16_t& address, const uint8_t& data)
{
	
	if (RAMG == 0x0A)
	{
		uint8_t ramBankNum = (MODE) ? BANK2 : 0;
		ramBankNum &= ramBankMask;
		ramBanks[ramBankNum]->at(address % RAM_BANK_SIZE) = data;
	}
}
