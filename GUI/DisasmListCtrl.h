#pragma once
#include "../Disassembler.h"
#include <wx/listctrl.h>
#include <vector>

#ifdef __GNUG__
#include <memory>
#endif


struct InsData
{
	wxString address;
	wxString opcode;
	wxString mnemonic;
	wxString operand;
	wxString comment;
};

class DisasmListCtrl : public wxListCtrl
{
public:
	DisasmListCtrl(wxWindow* parent);

	void StoreDisassembly(std::shared_ptr<Disassembly> disasm);

protected:
	wxString OnGetItemText(long item, long column) const;

private:
	std::vector<InsData> disasmData;

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
