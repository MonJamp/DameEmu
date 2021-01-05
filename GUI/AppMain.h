#pragma once

#include "../DameEmu.h"
#include "MainCanvas.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include "Add-ons/imfilebrowser.h"

#include "DebuggerWidget.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
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

	bool running;
	bool showDebugger;

	ImGui::FileBrowser fileDialog;
	DebuggerWidget* debuggerWidget;
};