#include "DebugFrame.h"


wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_BUTTON(ButtonID::Step, DisasmFrame::OnStep)
	EVT_BUTTON(ButtonID::RunBreak, DisasmFrame::OnRunBreak)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame(std::shared_ptr<Debugger> d, wxWindow* parent)
	: wxFrame(parent, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 500)),
	debugger(d)
{
	InitWidgets();

	running = false;
	regPanel->UpdateValues(debugger->cpuState);
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
	hbox->Add(disasmPanel, wxSizerFlags(3).Expand());

	regPanel = new RegPanel(this);
	hbox->Add(regPanel, wxSizerFlags(1).Expand());

	vbox->Add(hbox, wxSizerFlags(1).Expand());
	SetSizer(vbox);
}

void DisasmFrame::OnStep(wxCommandEvent& evt)
{
	debugger->Step();
	regPanel->UpdateValues(debugger->cpuState);
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
		regPanel->UpdateValues(debugger->cpuState);
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
		regPanel->UpdateValues(debugger->cpuState);
		disasmPanel->OnPause();
	}
	else
	{
		evt.RequestMore(true);
	}
}
