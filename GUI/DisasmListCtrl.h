#pragma once
#include "../Disassembler.h"
#include <wx/listctrl.h>
#include <vector>
#include <memory>
#include <unordered_map>


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
	void StoreAddressTable(AddressTable& at);
	void ShowAddress(uint16_t a);

protected:
	wxString OnGetItemText(long item, long column) const;

private:
	std::vector<InsData> disasmData;
	AddressTable addressTable; // Address is key, index is value
	uint16_t selectedItem = 0;


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
