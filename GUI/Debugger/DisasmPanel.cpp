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

	// Puts disassembler controls in a tabbed interface
	notebook = new wxNotebook(this, wxID_ANY);
	bpList = new BreakpointList(debugger, notebook);
	memBrowser = new MemoryBrowser(debugger->GetBus(), notebook);
	// Size must be set to show panel in notebook
	// Setting size causes assertion errors in gtk
	bpList->SetSize(bpList->GetBestSize());
	memBrowser->SetSize(memBrowser->GetBestSize());
	// Add pages to notebook
	notebook->AddPage(bpList, "Breakpoints", true);
	notebook->AddPage(memBrowser, "Memory Browser");
	vbox->Add(notebook, wxSizerFlags(1).Expand());

	SetSizer(vbox);
}

void DisasmPanel::OnPause()
{
	disasmList->ShowCurrentAddress();
	disasmList->Refresh();
	memBrowser->RefreshValues();
}

void DisasmPanel::OnToggleBreakpoint(wxCommandEvent& evt)
{
	bpList->SetItems();
}
