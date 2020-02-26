#pragma once
#include "Dissassembler.h"
#include <wx/wx.h>

class DisasmFrame : public wxFrame
{
public:
	DisasmFrame();
	~DisasmFrame();

private:
	void InitMenuBar();

	void OnOpen(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

	Dissassembler* disasm;
};

