#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge()
	: rom{0}
{

}

Cartridge::~Cartridge()
{

}

// TODO: Check validity of cartridge
void Cartridge::open(const std::string& filename)
{
	std::ifstream file;
	file.open(filename, std::ios::binary);

	file.seekg(0, file.end);
	size_t file_size = static_cast<size_t>(file.tellg());
	file.seekg(0, file.beg);

	rom.fill(0);
	file.read((char*)(&rom[0]), file_size);

	file.close();

	ParseHeader();
}

uint8_t logo[] =
{
	0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
	0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
	0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};

bool Cartridge::isValid()
{
	// Logo test
	for (int i = 0x104; i <= 0x133; i++)
	{
		if (rom[i] != logo[i - 0x104])
		{
			return false;
		}
	}

	// Checksum test
	uint8_t sum = 0;
	for (int i = 0x134; i <= 0x14C; i++)
	{
		sum = sum - rom[i] - 1;
	}

	if (sum != header.checksum)
	{
		return false;
	}

	return true;
}

void Cartridge::ParseHeader()
{
	// Get title
	std::string title = "";
	for (int i = 0x134; i <= 0x143; i++)
	{
		char c = rom[i];
		if (c == '\0')
			break;
		title.push_back(c);
	}

	header.title = title;

	// Get mode
	if (rom[0x143] == 0x80 || rom[0x143] == 0xC0)
	{
		header.mode = Mode::CGB;
	}

	if (rom[0x146] == 0x03)
	{
		header.mode = Mode::SGB;
	}

	// Get mapper
	switch (rom[147])
	{
		case 0x00:
		{
			header.mapper = Mapper::ROM;
			break;
		}
		case 0x01:
		{
			header.mapper = Mapper::MBC1;
			break;
		}
		case 0x02:
		{
			header.mapper = Mapper::MBC1;
			header.has_ram = true;
			break;
		}
		case 0x03:
		{
			header.mapper = Mapper::MBC1;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x05:
		{
			header.mapper = Mapper::MBC2;
			break;
		}
		case 0x06:
		{
			header.mapper = Mapper::MBC2;
			header.has_battery = true;
			break;
		}
		case 0x08:
		{
			header.mapper = Mapper::ROM;
			header.has_ram = true;
			break;
		}
		case 0x09:
		{
			header.mapper = Mapper::ROM;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x0B:
		{
			header.mapper = Mapper::MMM01;
			break;
		}
		case 0x0C:
		{
			header.mapper = Mapper::MMM01;
			header.has_ram = true;
			break;
		}
		case 0x0D:
		{
			header.mapper = Mapper::MMM01;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x0F:
		{
			header.mapper = Mapper::MBC3;
			header.has_timer = true;
			header.has_battery = true;
			break;
		}
		case 0x10:
		{
			header.mapper = Mapper::MBC3;
			header.has_timer = true;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x11:
		{
			header.mapper = Mapper::MBC3;
			break;
		}
		case 0x12:
		{
			header.mapper = Mapper::MBC3;
			header.has_ram = true;
			break;
		}
		case 0x13:
		{
			header.mapper = Mapper::MBC3;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x19:
		{
			header.mapper = Mapper::MBC5;
			break;
		}
		case 0x1A:
		{
			header.mapper = Mapper::MBC5;
			header.has_ram = true;
			break;
		}
		case 0x1B:
		{
			header.mapper = Mapper::MBC5;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x1C:
		{
			header.mapper = Mapper::MBC5;
			header.has_rumble = true;
			break;
		}
		case 0x1D:
		{
			header.mapper = Mapper::MBC5;
			header.has_rumble = true;
			header.has_ram = true;
			break;
		}
		case 0x1E:
		{
			header.mapper = Mapper::MBC5;
			header.has_rumble = true;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0x20:
		{
			header.mapper = Mapper::MBC6;
			break;
		}
		case 0x22:
		{
			header.mapper = Mapper::MBC5;
			header.has_sensor = true;
			header.has_rumble = true;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		case 0xFC:
		{
			header.mapper = Mapper::CAM;
			break;
		}
		case 0xFD:
		{
			header.mapper = Mapper::TAMA5;
			break;
		}
		case 0xFE:
		{
			header.mapper = Mapper::HuC3;
			break;
		}
		case 0xFF:
		{
			header.mapper = Mapper::HuC1;
			header.has_ram = true;
			header.has_battery = true;
			break;
		}
		default:
		{
			header.mapper = Mapper::Unknown;
			break;
		}
	}

	// Get ROM size
	header.rom_size = rom[0x148];

	// Get RAM size
	header.ram_size = rom[0x149];

	// Header Checksum
	header.checksum = rom[0x14D];
}

uint8_t Cartridge::read(const uint16_t& address)
{
	return rom[address];
}

size_t Cartridge::size()
{
	return rom.size();
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
	case Mapper::ROM:
		info.append("ROM");
		break;
	case Mapper::MBC1:
		info.append("MBC1");
		break;
	case Mapper::MBC2:
		info.append("MBC2");
		break;
	case Mapper::MBC3:
		info.append("MBC3");
		break;
	case Mapper::MBC5:
		info.append("MBC5");
		break;
	case Mapper::MBC6:
		info.append("MBC6");
		break;
	case Mapper::MBC7:
		info.append("MBC7");
		break;
	case Mapper::MMM01:
		info.append("MMM01");
		break;
	case Mapper::CAM:
		info.append("CAM");
		break;
	case Mapper::TAMA5:
		info.append("TAMA5");
		break;
	case Mapper::HuC3:
		info.append("HuC3");
		break;
	case Mapper::HuC1:
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
