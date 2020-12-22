#include "DebugFrame.h"


wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_BUTTON(ButtonID::Step, DisasmFrame::OnStep)
	EVT_BUTTON(ButtonID::RunBreak, DisasmFrame::OnRunBreak)
	EVT_MENU(MenuID::SHOW_VRAM, DisasmFrame::OnShowVram)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame(std::shared_ptr<Debugger> d, wxWindow* parent)
	: wxFrame(parent, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(750, 550)),
	debugger(d)
{
	menuBar = new wxMenuBar();
	windowsMenu = new wxMenu();
	vramItem = new wxMenuItem(windowsMenu, MenuID::SHOW_VRAM, _("&VRAM"));
	windowsMenu->Append(vramItem);
	menuBar->Append(windowsMenu, _("Windows"));
	SetMenuBar(menuBar);

	InitWidgets();

	running = false;
	regPanel->UpdateValues(debugger->regState);
}

DisasmFrame::~DisasmFrame()
{
	
}

void DisasmFrame::InitWidgets()
{
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	btnPanel = new ButtonPanel(this);
	vbox->Add(btnPanel, wxSizerFlags(0));

	disasmPanel = new DisasmPanel(debugger, this);
	hbox->Add(disasmPanel, wxSizerFlags(5).Expand());

	regPanel = new RegPanel(this);
	hbox->Add(regPanel, wxSizerFlags(2).Expand().Border(wxALL, 10));

	vbox->Add(hbox, wxSizerFlags(1).Expand());
	SetSizerAndFit(vbox);
	//TODO: Use default color theme for windows?
#ifndef __WXGTK__
	SetBackgroundColour(*wxWHITE);
#endif
}

void DisasmFrame::OnStep(wxCommandEvent& evt)
{
	debugger->Step();
	regPanel->UpdateValues(debugger->regState);
	disasmPanel->OnPause();
}

void DisasmFrame::OnRunBreak(wxCommandEvent& evt)
{
	if (!running)
	{
		running = true;
		btnPanel->runbreakBtn->SetLabel("Break");
		Connect(wxEVT_IDLE, wxIdleEventHandler(DisasmFrame::RunLoop));
	}
	else
	{
		running = false;
		btnPanel->runbreakBtn->SetLabel("Run");
		Disconnect(wxEVT_IDLE, wxIdleEventHandler(DisasmFrame::RunLoop));
		regPanel->UpdateValues(debugger->regState);
		disasmPanel->OnPause();
	}
}

void DisasmFrame::RunLoop(wxIdleEvent& evt)
{
	debugger->Step();

	if (debugger->HitBreakpoint())
	{
		running = false;
		btnPanel->runbreakBtn->SetLabel("Run");
		Disconnect(wxEVT_IDLE, wxIdleEventHandler(DisasmFrame::RunLoop));
		regPanel->UpdateValues(debugger->regState);
		disasmPanel->OnPause();
	}
	else
	{
		evt.RequestMore(true);
	}
}

void DisasmFrame::OnShowVram(wxCommandEvent& evt)
{
	if(wxWindow::FindWindowByName("VRAM Viewer"))
	{
		vramFrame->Raise();
	}
	else
	{
		vramFrame = new VramFrame(debugger->GetBus(), this);
		vramFrame->SetName("VRAM Viewer");
		vramFrame->Show();
	}
}
