#include "DisasmList.h"


wxBEGIN_EVENT_TABLE(DisasmList, wxListView)
	EVT_LIST_ITEM_RIGHT_CLICK(EventID::LIST_CTRL, DisasmList::OnListRightClick)
	EVT_MENU(EventID::AddBreakpoint, DisasmList::OnPopupClick)
wxEND_EVENT_TABLE()

DisasmList::DisasmList(std::shared_ptr<Debugger> d, wxWindow* parent)
	: wxListView(parent, EventID::LIST_CTRL, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_VIRTUAL | wxLC_NO_HEADER),
	debugger(d)
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

	RefreshValues();
}

void DisasmList::RefreshValues()
{
	Disassembly& disasm = debugger->GetDisassembly();
	addressToIndex = debugger->GetAddressTable();

	disasmData.clear();

	for (auto i : disasm)
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

	ShowAddress(debugger->cpuState.pc);
}

void DisasmList::ShowAddress(uint16_t a)
{
	// Deselect previous item
	Select(selectedItem, false);
	// Select new item
	selectedItem = addressToIndex[a];
	Focus(selectedItem);
	Select(selectedItem, true);
}

void DisasmList::OnListRightClick(wxListEvent& evt)
{
	long item = evt.GetIndex();

	wxMenu popMenu;
	popMenu.SetClientData(reinterpret_cast<void*>(item));
	popMenu.Append(EventID::AddBreakpoint, _("&Add Breakpoint"));
	PopupMenu(&popMenu);
}

void DisasmList::OnPopupClick(wxCommandEvent& evt)
{
	long item = reinterpret_cast<long>(static_cast<wxMenu*>(evt.GetEventObject())->GetClientData());

	if (evt.GetId() == EventID::AddBreakpoint)
	{
		uint8_t address = this->debugger->GetDisassembly()[item].address;
		this->debugger->AddBreakpoint(address);
	}
}

wxString DisasmList::OnGetItemText(long item, long column) const
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