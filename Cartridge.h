#pragma once
#include <cstdint>
#include <string>
#include <array>

// TODO: Handle different size ROMs
constexpr uint16_t ROM_MAX_SIZE = 0x8000;


class Cartridge
{
public:
	Cartridge();
	~Cartridge();

	void open(const std::string& filename);
	uint8_t read(const uint16_t& address);
	size_t size();

private:
	std::array<uint8_t, ROM_MAX_SIZE> rom;

};
