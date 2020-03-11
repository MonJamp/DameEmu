#include "DisasmPanel.h"


wxBEGIN_EVENT_TABLE(DisasmPanel, wxPanel)
	EVT_COMMAND(wxID_ANY, ADD_BREAKPOINT, DisasmPanel::OnAddBreakpoint)
wxEND_EVENT_TABLE()

DisasmPanel::DisasmPanel(std::shared_ptr<Debugger> d, wxWindow* parent)
	: wxPanel(parent), debugger(d)
{
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

	disasmList = new DisasmList(debugger, this);
	vbox->Add(disasmList, wxSizerFlags(3).Expand());

	bpList = new BreakpointList(this);
	vbox->Add(bpList, wxSizerFlags(1).Expand());

	SetSizer(vbox);
}

void DisasmPanel::OnPause()
{
	disasmList->ShowCurrentAddress();
}

void DisasmPanel::OnAddBreakpoint(wxCommandEvent& evt)
{
	bpList->SetItems(debugger->GetBreakpoints());
}
