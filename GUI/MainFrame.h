#pragma once
#include "../DameEmu.h"
#include "DisasmFrame.h"
#include "wx_header.h"

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

	enum MenuID
	{
		LOAD,
		RUN,
		EXIT,
		SETTINGS,
		DEBUGGER,
		ABOUT
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

	DisasmFrame* disasmFrame = NULL;

	wxString rom_dir;
	DameEmu* dameEmu = nullptr;
};