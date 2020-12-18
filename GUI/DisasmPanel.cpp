#include "DisasmPanel.h"


wxBEGIN_EVENT_TABLE(DisasmPanel, wxPanel)
	EVT_COMMAND(wxID_ANY, TOGGLE_BREAKPOINT, DisasmPanel::OnToggleBreakpoint)
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
	disasmList->Refresh();
}

void DisasmPanel::OnToggleBreakpoint(wxCommandEvent& evt)
{
	bpList->SetItems(debugger->GetBreakpoints());
}
