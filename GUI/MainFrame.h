#pragma once
#include "../DameEmu.h"
#include "../Cartridge.h"
#include "MainCanvas.h"
#include "Debugger/DebugFrame.h"
#include "wx_header.h"
#include <memory>

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
	void OnUnloadROM(wxCommandEvent& evt);
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
		CHECK,
		UNLOAD
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

	MainCanvas* mainCanvas = NULL;
	DisasmFrame* disasmFrame = NULL;

	std::unique_ptr<DameEmu> dameEmu;
};