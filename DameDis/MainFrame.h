#pragma once
#include "Disassembler.h"
#include "DisasmListCtrl.h"
#include <wx/wx.h>


class MainFrame : public wxFrame
{
public:
	MainFrame();
	~MainFrame();

private:
	void InitMenuBar();

	void OnOpen(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

	DisasmListCtrl disasmList;
};

