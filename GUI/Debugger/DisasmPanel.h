#pragma once
#include "../../Disassembler.h"
#include "DisasmList.h"
#include "BreakpointList.h"
#include "MemoryBrowser.h"
#include "../wx_header.h"
#include <memory>


class DisasmPanel : public wxPanel
{
public:
	DisasmPanel(std::shared_ptr<Debugger> debugger, wxWindow* parent);
	void OnPause();

private:
	void OnToggleBreakpoint(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
	DisasmList* disasmList;
	wxNotebook* notebook;
	BreakpointList* bpList;
	MemoryBrowser* memBrowser;
	std::shared_ptr<Debugger> debugger;

};