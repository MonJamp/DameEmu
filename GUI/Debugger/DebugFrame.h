#pragma once
#include "../../Disassembler.h"
#include "DisasmPanel.h"
#include "RegPanel.h"
#include "VramFrame.h"
#include <wx/wx.h>
#include <memory>
#include <vector>


enum ButtonID
{
	Step, RunBreak
};

class ButtonPanel : public wxPanel
{
public:
	ButtonPanel(wxWindow* parent) : wxPanel(parent)
	{
		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

		// TODO Add buttons for continue, pause, step in, step out, step over
		wxButton* stepBtn = new wxButton(this, ButtonID::Step, "Step");
		hbox->Add(stepBtn, wxSizerFlags(0).Align(wxLEFT));

		runbreakBtn = new wxButton(this, ButtonID::RunBreak, "Run");
		hbox->Add(runbreakBtn, wxSizerFlags(0).Align(wxLEFT));

		SetSizer(hbox);
	}

public:
	wxButton* runbreakBtn;

private:
};

class DisasmFrame : public wxFrame
{
public:
	DisasmFrame(std::shared_ptr<Debugger> d, wxWindow* parent);
	~DisasmFrame();

private:
	void InitWidgets();

	void OnStep(wxCommandEvent& evt);
	void OnRunBreak(wxCommandEvent& evt);
	void RunLoop(wxIdleEvent& evt);

	void OnShowVram(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
	enum MenuID
	{
		SHOW_VRAM
	};

private:
	bool running;

	wxMenuBar* menuBar;
	wxMenu* windowsMenu;
	wxMenuItem* vramItem;

	DisasmPanel* disasmPanel;
	RegPanel* regPanel;
	ButtonPanel* btnPanel;
	
	VramFrame* vramFrame = NULL;

	std::shared_ptr<Debugger> debugger;
};