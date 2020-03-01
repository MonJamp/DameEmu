#pragma once
#include "Dissassembler.h"
#include "DisasmListCtrl.h"
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

	DisasmListCtrl disasmList;
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

