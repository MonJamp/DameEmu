#pragma once
#include "../Disassembler.h"
#include <wx/listctrl.h>
#include <vector>
#include <unordered_map>

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
	void StoreJumpTable(std::unordered_map<uint16_t, uint16_t>& jt);

	void GoToAddress(uint16_t address);

protected:
	wxString OnGetItemText(long item, long column) const;

private:
	std::vector<InsData> disasmData;
	std::unordered_map<uint16_t, uint16_t> jumpTable;

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
