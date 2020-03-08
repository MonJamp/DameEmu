#include "DisasmFrame.h"


wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_BUTTON(ButtonID::Step, DisasmFrame::OnStep)
	EVT_BUTTON(ButtonID::RunBreak, DisasmFrame::OnRunBreak)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame(std::shared_ptr<Debugger> d, wxWindow* parent)
	: wxFrame(parent, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400)),
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

	disasmList = new DisasmList(debugger, this);
	hbox->Add(disasmList, wxSizerFlags(3).Expand());
	regPanel = new RegPanel(this);
	hbox->Add(regPanel, wxSizerFlags(1).Expand());

	vbox->Add(hbox, wxSizerFlags(1).Expand());
	SetSizer(vbox);
}

void DisasmFrame::OnStep(wxCommandEvent& evt)
{
	debugger->Step();
	regPanel->UpdateValues(debugger->cpuState);
	disasmList->ShowAddress(debugger->cpuState.pc);
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
		disasmList->ShowAddress(debugger->cpuState.pc);
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
		disasmList->ShowAddress(debugger->cpuState.pc);
	}
	else
	{
		evt.RequestMore(true);
	}
}
