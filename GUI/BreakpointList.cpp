#include "BreakpointList.h"


BreakpointList::BreakpointList(wxWindow* parent)
{
#ifdef __WINDOWS__
	// Disables vertical gap between columns
	wxSystemThemedControl::EnableSystemTheme(false);
#endif

	Create(parent);
	Init();
}

bool BreakpointList::Create(wxWindow* parent)
{
	return wxListView::Create(parent, wxID_ANY, wxDefaultPosition, wxSize(0,0),
		wxLC_REPORT | wxLC_VIRTUAL);
}

void BreakpointList::Init()
{
	wxListItem itemCol;
	// Address
	itemCol.SetText("Address");
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(70);
	InsertColumn(static_cast<long>(ColumnID::Address), itemCol);
	// Opcode
	itemCol.SetText("Instruction");
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(150);
	InsertColumn(static_cast<long>(ColumnID::Instruction), itemCol);
	// Enabled
	itemCol.SetText("Enabled");
	itemCol.SetId(wxID_ANY);
	itemCol.SetWidth(70);
	InsertColumn(static_cast<long>(ColumnID::Enabled), itemCol);
}

wxString BreakpointList::OnGetItemText(long item, long column) const
{
	if (static_cast<size_t>(item) > bpData.size())
	{
		return "";
	}

	switch (static_cast<ColumnID>(column))
	{
	case ColumnID::Address:		return bpData[item].address;
	case ColumnID::Instruction:	return bpData[item].instruction;
	case ColumnID::Enabled:		return bpData[item].enabled;
	default:	return "";
	}
}

void BreakpointList::SetItems(Disassembly breakpoints)
{
	bpData.clear();

	for (auto i : breakpoints)
	{
		BreakpointData data;
		data.address = i.addressToStr();
		data.instruction = i.mnemonicToStr() + i.operandsToStr();
		data.enabled = "true";

		bpData.push_back(data);
	}

	SetItemCount(bpData.size());
	Refresh();
}
