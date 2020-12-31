#include "Cartridge.h"
#include <cassert>

Cartridge::Cartridge()
{
	mapper.reset();
}

Cartridge::~Cartridge()
{
	while (!romBanks.empty())
	{
		RomBank* b = romBanks.back();
		romBanks.pop_back();
		delete b;
	}

	while (!ramBanks.empty())
	{
		RamBank* b = ramBanks.back();
		ramBanks.pop_back();
		delete b;
	}
}

// TODO: Check validity of cartridge
void Cartridge::open(const std::string& filename)
{
	std::ifstream file;
	file.open(filename, std::ios::binary);

	file.seekg(0, file.end);
	size_t file_size = static_cast<size_t>(file.tellg());
	file.seekg(0, file.beg);

	uint32_t romSizeInMemory = file_size + ROM_BANK_SIZE - (file_size % ROM_BANK_SIZE);
	uint8_t numBanks = romSizeInMemory / ROM_BANK_SIZE;
	romBanks.reserve(numBanks);

	for (uint16_t i = 0; i < numBanks; i++)
	{
		RomBank* empty = new RomBank();
		empty->fill(0x00);
		romBanks.push_back(empty);

		file.read((char*)romBanks[i]->data(), ROM_BANK_SIZE);
	}

	file.close();

	ParseHeader();
	AllocateRAM();

	switch (header.mapper)
	{
	case MapperType::ROM: mapper = std::make_unique<Mapper>(romBanks, ramBanks); break;
	case MapperType::MBC1: mapper = std::make_unique<MBC1>(romBanks, ramBanks); break;
	// Unimplemnted mapper
	default: assert(false);
	}
}

uint8_t logo[] =
{
	0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
	0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
	0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};

bool Cartridge::isValid()
{
	RomBank& bank0 = *romBanks[0];

	// Logo test
	for (int i = 0x104; i <= 0x133; i++)
	{
		if (bank0[i] != logo[i - 0x104])
		{
			return false;
		}
	}

	// Checksum test
	uint8_t sum = 0;
	for (int i = 0x134; i <= 0x14C; i++)
	{
		sum = sum - bank0[i] - 1;
	}

	if (sum != header.checksum)
	{
		return false;
	}

	return true;
}

void Cartridge::ParseHeader()
{
	RomBank& bank0 = *romBanks[0];

	// Get title
	std::string title = "";
	for (int i = 0x134; i <= 0x143; i++)
	{
		char c = bank0[i];
		if (c == '\0')
			break;
		title.push_back(c);
	}

	header.title = title;

	// Get mode
	//TODO: 0xC0 means game is only compatible with CGB
	if (bank0[0x143] == 0x80 || bank0[0x143] == 0xC0)
	{
		header.mode = Mode::CGB;
	}

	if (bank0[0x146] == 0x03)
	{
		header.mode = Mode::SGB;
	}

	// Get mapper
	switch (bank0[0x147])
	{
		case 0x00:
		{
			header.mapper = MapperType::ROM;
			break;
		}
		case 0x01:
		{
			header.mapper = MapperType::MBC1;
			break;
		}
		case 0x02:
		{
			header.mapper = MapperType::MBC1;
			header.has_ram = true;
			break;
		}
		case 0x03:
		{
			header.mapper = MapperType::MBC1;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x05:
		{
			header.mapper = MapperType::MBC2;
			break;
		}
		case 0x06:
		{
			header.mapper = MapperType::MBC2;
			header.has_battery = true;
			break;
		}
		case 0x08:
		{
			header.mapper = MapperType::ROM;
			header.has_ram = true;
			break;
		}
		case 0x09:
		{
			header.mapper = MapperType::ROM;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x0B:
		{
			header.mapper = MapperType::MMM01;
			break;
		}
		case 0x0C:
		{
			header.mapper = MapperType::MMM01;
			header.has_ram = true;
			break;
		}
		case 0x0D:
		{
			header.mapper = MapperType::MMM01;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x0F:
		{
			header.mapper = MapperType::MBC3;
			header.has_timer = true;
			header.has_battery = true;
			break;
		}
		case 0x10:
		{
			header.mapper = MapperType::MBC3;
			header.has_timer = true;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x11:
		{
			header.mapper = MapperType::MBC3;
			break;
		}
		case 0x12:
		{
			header.mapper = MapperType::MBC3;
			header.has_ram = true;
			break;
		}
		case 0x13:
		{
			header.mapper = MapperType::MBC3;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x19:
		{
			header.mapper = MapperType::MBC5;
			break;
		}
		case 0x1A:
		{
			header.mapper = MapperType::MBC5;
			header.has_ram = true;
			break;
		}
		case 0x1B:
		{
			header.mapper = MapperType::MBC5;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x1C:
		{
			header.mapper = MapperType::MBC5;
			header.has_rumble = true;
			break;
		}
		case 0x1D:
		{
			header.mapper = MapperType::MBC5;
			header.has_rumble = true;
			header.has_ram = true;
			break;
		}
		case 0x1E:
		{
			header.mapper = MapperType::MBC5;
			header.has_rumble = true;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x20:
		{
			header.mapper = MapperType::MBC6;
			break;
		}
		case 0x22:
		{
			header.mapper = MapperType::MBC5;
			header.has_sensor = true;
			header.has_rumble = true;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0xFC:
		{
			header.mapper = MapperType::CAM;
			break;
		}
		case 0xFD:
		{
			header.mapper = MapperType::TAMA5;
			break;
		}
		case 0xFE:
		{
			header.mapper = MapperType::HuC3;
			break;
		}
		case 0xFF:
		{
			header.mapper = MapperType::HuC1;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		default:
		{
			header.mapper = MapperType::Unknown;
			break;
		}
	}

	// Get ROM size
	header.rom_size = bank0[0x148];

	// Get RAM size
	header.ram_size = bank0[0x149];

	// Header Checksum
	header.checksum = bank0[0x14D];
}

void Cartridge::AllocateRAM()
{
	uint16_t ram_size = 0;

	switch (header.ram_size)
	{
	case 0x1: ram_size = 0x800; break;
	case 0x2: ram_size = 0x2000; break;
	case 0x3: ram_size = 0x8000; break;
	case 0x4: ram_size = 0x20000; break;
	case 0x5: ram_size = 0x10000; break;
	}

	uint8_t numBanks = ram_size / RAM_BANK_SIZE;
	ramBanks.reserve(numBanks);

	for (uint8_t i = 0; i < numBanks; i++)
	{
		RamBank* empty = new RamBank();
		empty->fill(0x00);

		ramBanks.push_back(empty);
	}
}

uint8_t Cartridge::ROM_Read(const uint16_t& address)
{
	return mapper->ROM_Read(address);
}

void Cartridge::ROM_Write(const uint16_t& address, const uint8_t& data)
{
	mapper->ROM_Write(address, data);
}

uint8_t Cartridge::RAM_Read(const uint16_t& address)
{
	return mapper->RAM_Read(address);
}

void Cartridge::RAM_Write(const uint16_t& address, const uint8_t& data)
{
	mapper->RAM_Write(address, data);
}

size_t Cartridge::size()
{
	return romBanks.size() * ROM_BANK_SIZE;
}

uint16_t Cartridge::romSize()
{
	uint16_t size = (32 << (header.rom_size & 0x0F));

	if (header.rom_size > 0x10)
	{
		size += (32 << (header.rom_size & 0xF0));
	}

	return size;
}

uint16_t Cartridge::ramSize()
{
	return (2 << (header.ram_size & 0x0F));
}

std::string Cartridge::headerToString()
{
	std::string info = "";
	info.append("Title: ");
	info.append(header.title);

	info.append("\nMode: ");
	switch (header.mode)
	{
	case Mode::DMG:
		info.append("DMG");
		break;
	case Mode::CGB:
		info.append("CGB");
		break;
	case Mode::SGB:
		info.append("SGB");
		break;
	default: break;
	}

	info.append("\nMapper: ");
	switch (header.mapper)
	{
	case MapperType::ROM:
		info.append("ROM");
		break;
	case MapperType::MBC1:
		info.append("MBC1");
		break;
	case MapperType::MBC2:
		info.append("MBC2");
		break;
	case MapperType::MBC3:
		info.append("MBC3");
		break;
	case MapperType::MBC5:
		info.append("MBC5");
		break;
	case MapperType::MBC6:
		info.append("MBC6");
		break;
	case MapperType::MBC7:
		info.append("MBC7");
		break;
	case MapperType::MMM01:
		info.append("MMM01");
		break;
	case MapperType::CAM:
		info.append("CAM");
		break;
	case MapperType::TAMA5:
		info.append("TAMA5");
		break;
	case MapperType::HuC3:
		info.append("HuC3");
		break;
	case MapperType::HuC1:
		info.append("HuC1");
		break;
	default:
		info.append("Unknown");
		break;
	}

	if (header.has_sensor) info.append("+SENSOR");
	if (header.has_rumble) info.append("+RUMBLE");
	if (header.has_timer) info.append("+TIMER");
	if (header.has_ram) info.append("+RAM");
	if (header.has_battery) info.append("+BATTERY");

	info.append("\nROM size: ");
	info.append(std::to_string(romSize()));
	info.append("KB");

	info.append("\nRAM size: ");
	info.append(std::to_string(ramSize()));
	info.append("KB");

	info.append("\nHeader checksum: ");
	info.append(std::to_string(header.checksum));

	return info;
}
