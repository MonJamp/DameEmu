#pragma once
#include "../Disassembler.h"
#include "DisasmList.h"
#include "BreakpointList.h"
#include <wx/panel.h>
#include <memory>


class DisasmPanel : public wxPanel
{
public:
	DisasmPanel(std::shared_ptr<Debugger> debugger, wxWindow* parent);
	void OnPause();

private:
	void OnAddBreakpoint(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
	DisasmList* disasmList;
	BreakpointList* bpView;
	std::shared_ptr<Debugger> debugger;

};