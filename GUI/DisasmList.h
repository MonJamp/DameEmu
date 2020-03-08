#pragma once
#include "../Disassembler.h"
#include <wx/wx.h>
#include <wx/listctrl.h>

class DisasmListView : public wxListView
{
public:
	struct InsData
	{
		wxString address;
		wxString opcode;
		wxString mnemonic;
		wxString operand;
		wxString comment;
	};

public:
	DisasmListView(std::shared_ptr<Debugger> d, wxWindow* parent);

	void RefreshValues();
	void ShowAddress(uint16_t a);

protected:
	wxString OnGetItemText(long item, long column) const;

private:
	std::vector<InsData> disasmData;
	std::shared_ptr<Debugger> debugger;
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