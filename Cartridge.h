#pragma once
#include <cstdint>
#include <string>
#include <array>

// TODO: Handle different size ROMs
constexpr uint16_t ROM_MAX_SIZE = 0x8000;


class Cartridge
{
public:
	enum class Mode
	{
		DMG, CGB, SGB
	};

	enum class Mapper
	{
		ROM, MBC1, MBC2, MBC3, MBC5, MBC6, MBC7,
		MMM01, CAM, TAMA5, HuC3, HuC1, Unknown
	};

	struct Header
	{
		std::string title;
		Mode mode = Mode::DMG;
		Mapper mapper;
		bool has_battery = false;
		bool has_ram = false;
		bool has_timer = false;
		bool has_rumble = false;
		bool has_sensor = false;
		// Contains value 0x00 - 0x54
		// ROM size calculated by
		// (32(KB) << (value & 0x0F)) + (32(KB) << (value & 0xF0))
		uint8_t rom_size;
		// Contains value 0x00 - 0x05
		// Calculated similar as ROM size except 2KB is the base
		uint8_t ram_size;
		uint8_t checksum;
	};

public:
	Cartridge();
	~Cartridge();

	void open(const std::string& filename);
	bool isValid();
	uint8_t read(const uint16_t& address);
	size_t size();
	uint16_t romSize();
	uint16_t ramSize();
	std::string headerToString();

public:
	Header header;

private:
	void ParseHeader();

private:
	std::array<uint8_t, ROM_MAX_SIZE> rom;

};
