#pragma once
#include "../DameEmu.h"
#include <memory>


class AppMain
{
public:
	AppMain();
	~AppMain();

	void MainLoop();

	void MenuBar();

private:
	sf::RenderWindow app;
	std::unique_ptr<DameEmu> dameEmu;
};