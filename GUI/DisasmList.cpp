#include "DisasmList.h"
#include <wx/itemattr.h>


wxDEFINE_EVENT(ADD_BREAKPOINT, wxCommandEvent);

wxBEGIN_EVENT_TABLE(DisasmList, wxListView)
	EVT_LIST_ITEM_RIGHT_CLICK(EventID::LIST_CTRL, DisasmList::OnListRightClick)
	EVT_MENU(EventID::AddBreakpoint, DisasmList::OnPopupClick)
wxEND_EVENT_TABLE()

DisasmList::DisasmList(std::shared_ptr<Debugger> d, wxWindow* parent)
	: debugger(d)
{
#ifdef __WINDOWS__
	// Disables vertical gap between columns
	wxSystemThemedControl::EnableSystemTheme(false);
#endif

	Create(parent);

	wxColor fontColor = wxColor(*wxBLACK);
	wxColor bgColor = wxColor(*wxWHITE);
	wxFont font = wxFont(*wxNORMAL_FONT);
	itemAttr = new wxItemAttr(fontColor, bgColor, font);

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

DisasmList::~DisasmList()
{
	delete itemAttr;
}

bool DisasmList::Create(wxWindow* parent)
{
	return wxListView::Create(parent, EventID::LIST_CTRL, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_VIRTUAL | wxLC_NO_HEADER);
}

void DisasmList::RefreshValues()
{
	Disassembly disasm = debugger->GetDisassembly();
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

	ShowAddress(debugger->regState.cpu.pc);
}

void DisasmList::ShowCurrentAddress()
{
	ShowAddress(debugger->regState.cpu.pc);
}

void DisasmList::ShowAddress(uint16_t a)
{
	// Select new item
	uint16_t index = addressToIndex[a];
	Focus(index);
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
		Select(item, false);

		uint16_t address = this->debugger->GetDisassembly()[item].address;
		this->debugger->AddBreakpoint(address);

		wxCommandEvent event(ADD_BREAKPOINT);
		wxPostEvent(this->GetParent(), event);
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

wxItemAttr* DisasmList::OnGetItemAttr(long item) const
{
	// Reset itemAttr
	wxColor bgColor = wxColor(*wxWHITE);
	itemAttr->SetBackgroundColour(bgColor);

	for (auto i : debugger->GetBreakpoints())
	{
		uint16_t index = addressToIndex.at(i.address);
		if (item == index)
		{
			bgColor = wxColor(255, 82, 82);
			itemAttr->SetBackgroundColour(bgColor);
		}
	}

	uint16_t index = addressToIndex.at(debugger->regState.cpu.pc);
	if (item == index)
	{
		bgColor = wxColor(205, 220, 57);
		itemAttr->SetBackgroundColour(bgColor);
	}

	return itemAttr;
}