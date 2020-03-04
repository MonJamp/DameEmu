#pragma once
#include "Bus.h"
#include "Cartridge.h"
#include <string>
#include <cstdint>

#ifdef __GNUG__
#include <memory>
#endif


class DameEmu {
public:
	DameEmu(std::shared_ptr<Cartridge>& cart);
	~DameEmu();

	void Step();

private:
	Bus bus;
};