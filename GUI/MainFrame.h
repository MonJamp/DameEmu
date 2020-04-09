#pragma once
#include "../DameEmu.h"
#include "../Cartridge.h"
#include "GLPanel.h"
#include "DebugFrame.h"
#include "wx_header.h"

#ifdef __GNUG__
#include <memory>
#endif

class MainFrame : public wxFrame
{
public:
	MainFrame();
	~MainFrame();

private:
	void OnLoadROM(wxCommandEvent& evt);
	void OnRunEmu(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);
	void OnIdleRun(wxIdleEvent& evt);
	void OnSettings(wxCommandEvent& evt);
	void OnDebugger(wxCommandEvent& evt);
	void OnAbout(wxCommandEvent& evt);
#ifdef _DEBUG
	void OnCheckCart(wxCommandEvent& evt);
#endif

	enum MenuID
	{
		LOAD,
		RUN,
		EXIT,
		SETTINGS,
		DEBUGGER,
		ABOUT,
#ifdef _DEBUG
		CHECK
#endif
	};

	wxDECLARE_EVENT_TABLE();

	wxMenuBar* menuBar;
	wxMenu* systemMenu;
	wxMenu* emulationMenu;
	wxMenu* miscMenu;
	wxMenuItem* openItem;
	wxMenuItem* runItem;
	wxMenuItem* exitItem;
	wxMenuItem* settingsItem;
	wxMenuItem* debuggerItem;
	wxMenuItem* aboutItem;

	GLPanel* glPanel = NULL;
	DisasmFrame* disasmFrame = NULL;

	DameEmu* dameEmu = nullptr;
	std::shared_ptr<Cartridge> cart;
};