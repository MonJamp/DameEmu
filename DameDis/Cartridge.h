#pragma once
#include <cstdint>
#include <string>
#include <array>


constexpr uint16_t ROM_MAX_SIZE = 0x8000;


class Cartridge
{
public:
	Cartridge();
	~Cartridge();

	void open(const char* filename);
	uint8_t read(const uint16_t& address);

private:
	std::array<uint8_t, ROM_MAX_SIZE> rom;

};
