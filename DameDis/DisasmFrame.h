#pragma once
#include "Dissassembler.h"
#include <wx/wx.h>
#include <wx\listctrl.h>

class DisasmFrame : public wxFrame
{
public:
	DisasmFrame();
	~DisasmFrame();

private:
	void InitMenuBar();
	void InitDisassemblyList();
	void PopulateList();

	void OnOpen(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

	wxListCtrl* listDisasm;

	Dissassembler* disasm;

	enum class ColumnID
	{
		Empty,
		Address,
		Opcode,
		Mnemonic,
		Operand
	};
};

