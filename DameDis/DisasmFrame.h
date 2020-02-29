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
	void ResetDisassemblyList();
	void ReserveListItems(unsigned int size);
	void PopulateList(wxIdleEvent& evt);

	void OnDisassemble(wxIdleEvent& evt);

	void OnOpen(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

	wxListCtrl* listDisasm;
	unsigned int listIterator;

	Dissassembler* disasm;

	enum class ColumnID
	{
		Empty,
		Address,
		Opcode,
		Mnemonic,
		Operand,
		Comment
	};
};

