#include "BreakpointList.h"


wxBEGIN_EVENT_TABLE(BreakpointList, wxListView)
	//HACK: Enables right clicking listview with no items
	EVT_COMMAND_RIGHT_CLICK(EventID::LIST_CTRL, BreakpointList::OnPopupClick)
	EVT_LIST_ITEM_RIGHT_CLICK(EventID::LIST_CTRL, BreakpointList::OnItemRightClick)
	EVT_MENU(EventID::AddBreakpoint, BreakpointList::OnPopupClick)
	EVT_MENU(EventID::RemoveBreakpoint, BreakpointList::OnPopupClick)
wxEND_EVENT_TABLE()

BreakpointList::BreakpointList(std::shared_ptr<Debugger> d, wxWindow* parent)
	: debugger(d)
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
	return wxListView::Create(parent, EventID::LIST_CTRL, wxDefaultPosition, wxSize(0,0),
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

void BreakpointList::SetItems()
{
	bpData.clear();

	for (auto i : debugger->GetBreakpoints())
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

void BreakpointList::OnItemRightClick(wxListEvent& evt)
{
	long item = evt.GetIndex();

	wxMenu popMenu;
	if(item != -1)
	{
		popMenu.Append(EventID::RemoveBreakpoint, _("&Remove Breakpoint"));
	}
	popMenu.Append(EventID::AddBreakpoint, _("&Add Breakpoint"));
	popMenu.SetClientData(reinterpret_cast<void*>(item));
	PopupMenu(&popMenu);
}

void BreakpointList::OnPopupClick(wxCommandEvent& evt)
{
	int evtID = evt.GetId();
	switch(evtID)
	{
		case EventID::AddBreakpoint:
		{
			wxTextValidator hexValidator(wxFILTER_INCLUDE_CHAR_LIST);
			hexValidator.SetCharIncludes("0123456789ABCDEF");

			wxTextEntryDialog dlg(this, "Enter address", "Add Breakpoint");
			dlg.SetTextValidator(hexValidator);

			if(dlg.ShowModal() == wxID_OK)
			{
				wxString addressStr = dlg.GetValue();

				if(!addressStr.IsEmpty())
				{
					uint16_t address = strtol(addressStr, NULL, 16);
					debugger->AddBreakpoint(address);
					SetItems();
				}
			}

		} break;
		case EventID::RemoveBreakpoint:
		{
			long item = reinterpret_cast<long>(static_cast<wxMenu*>(evt.GetEventObject())->GetClientData());
			uint16_t address = strtol(bpData[item].address, NULL, 16);
			debugger->RemoveBreakpoint(address);
			SetItems();
		} break;
		default:
			break;
	}
}