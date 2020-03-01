#include "DisasmListCtrl.h"


DisasmListCtrl::DisasmListCtrl(wxWindow* parent)
	: wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_VIRTUAL | wxLC_NO_HEADER)
{
	wxListItem itemCol;
	// Empty
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(20);
	InsertColumn(static_cast<long>(ColumnID::Empty), itemCol);
	// Address
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(45);
	InsertColumn(static_cast<long>(ColumnID::Address), itemCol);
	// Opcode
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(70);
	InsertColumn(static_cast<long>(ColumnID::Opcode), itemCol);
	// Mnemonic
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(70);
	InsertColumn(static_cast<long>(ColumnID::Mnemonic), itemCol);
	// Operand
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(150);
	InsertColumn(static_cast<long>(ColumnID::Operand), itemCol);
	// Comment
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(150);
	InsertColumn(static_cast<long>(ColumnID::Comment), itemCol);
}

void DisasmListCtrl::StoreDisassembly(std::shared_ptr<Disassembly> disasm)
{
	disasmData.clear();

	for (auto i : *disasm)
	{
		InsData data;
		data.address = i.addressToStr();
		data.opcode = i.opcodeToStr();
		data.mnemonic = i.mnemonicToStr();
		data.operand = i.operandsToStr();
		data.comment = i.commentToStr();

		disasmData.push_back(data);
	}

	SetItemCount(disasmData.size());
	Refresh();
}

wxString DisasmListCtrl::OnGetItemText(long item, long column) const
{
	if (static_cast<size_t>(item) > disasmData.size())
	{
		return "";
	}

	switch (static_cast<ColumnID>(column))
	{
	case ColumnID::Address:		return disasmData[item].address;
	case ColumnID::Opcode:		return disasmData[item].opcode;
	case ColumnID::Mnemonic:	return disasmData[item].mnemonic;
	case ColumnID::Operand:		return disasmData[item].operand;
	case ColumnID::Comment:		return disasmData[item].comment;
	default: return "";
	}
}
