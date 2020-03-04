#pragma once
#include "../Disassembler.h"
#include "DisasmListCtrl.h"
#include <wx/wx.h>


class DisasmFrame : public wxFrame
{
public:
	DisasmFrame(wxWindow* parent);
	~DisasmFrame();

private:
	void InitMenuBar();

	void OnOpen(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

	DisasmListCtrl disasmList;
};

