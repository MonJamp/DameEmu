#pragma once
#include <cstdint>
#include <cassert>
#include <fstream>
#include <array>
#include <vector>


constexpr uint16_t ROM_BANK_SIZE = 0x4000;
constexpr uint16_t RAM_BANK_SIZE = 0x2000;

using RomBank = std::array<uint8_t, ROM_BANK_SIZE>;
using RamBank = std::array<uint8_t, RAM_BANK_SIZE>;

// Default mapper maps ROM directly to address space
class Mapper
{
public:
	Mapper(std::vector<RomBank*>& romBanks, std::vector<RamBank*>& ramBanks);
	~Mapper();

	virtual uint8_t ROM_Read(const uint16_t& address);
	virtual void ROM_Write(const uint16_t& address, const uint8_t& data);
	virtual uint8_t RAM_Read(const uint16_t& address);
	virtual void RAM_Write(const uint16_t& address, const uint8_t& data);

protected:
	std::vector<RomBank*>& romBanks;
	std::vector<RamBank*>& ramBanks;

};

// Register names and descriptions come from Gekkio's "Game Boy: Complete Technical Reference"

class MBC1 : public Mapper
{
public:
	MBC1(std::vector<RomBank*>& romBanks, std::vector<RamBank*>& ramBanks);
	~MBC1();

	virtual uint8_t ROM_Read(const uint16_t& address);
	virtual void ROM_Write(const uint16_t& address, const uint8_t& data);
	virtual uint8_t RAM_Read(const uint16_t& address);
	virtual void RAM_Write(const uint16_t& address, const uint8_t& data);

private:
	// Enable access to RAM
	// Value of 0x0A (0b1010) enables RAM, all other values disable RAM
	unsigned RAMG : 4;
	// Specifies ROM bank number to be used for 0x4000 - 0x7FFF
	unsigned BANK1 : 5;
	// Upper 2 bits of ROM bank number or 2 bit RAM bank number
	unsigned BANK2 : 2;
	// Determines how BANK2 register will be used
	// clear = BANK2 only affects 0x4000 - 0x7FFF
	// set = BANK2 affects 0x0000 - 0x3FFF, 0x4000 - 0x7FFF, 0xA000 - 0xBFFF
	unsigned MODE : 1;

	// Bit masks are used to truncate the bank number if it is greater
	// than the amount of banks

	uint8_t romBankMask;
	uint8_t ramBankMask;
};