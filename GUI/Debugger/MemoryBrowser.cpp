#include "MemoryBrowser.h"


wxBEGIN_EVENT_TABLE(MemoryBrowser, wxListView)
    EVT_SET_FOCUS(MemoryBrowser::OnFocus)
    EVT_KILL_FOCUS(MemoryBrowser::OnFocus)
wxEND_EVENT_TABLE()

MemoryBrowser::MemoryBrowser(std::shared_ptr<Bus> b, wxWindow* parent)
    : bus(b)
{
#ifdef __WINDOWS__
	// Disables vertical gap between columns
	wxSystemThemedControl::EnableSystemTheme(false);
#endif
    
    Create(parent);

    wxColor fontColor = wxColor(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOXTEXT));
    wxColor bgColor = wxColor(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));
    wxFont font = wxFont(9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    itemAttr = new wxItemAttr(fontColor, bgColor, font);

    wxListItem itemCol;
    // Empty
    itemCol.SetId(wxID_ANY);
    itemCol.SetWidth(0);
    InsertColumn(static_cast<long>(ColumnID::Empty), itemCol);
    // CategoryAddress
    itemCol.SetId(wxID_ANY);
    itemCol.SetWidth(75);
    InsertColumn(static_cast<long>(ColumnID::CategoryAddress), itemCol);
    // Values
    itemCol.SetId(wxID_ANY);
    itemCol.SetWidth(345);
    InsertColumn(static_cast<long>(ColumnID::Values), itemCol);
    // Ascii
    itemCol.SetId(wxID_ANY);
#ifdef __WINDOWS__
    itemCol.SetWidth(135);
#elif __WXGTK__
    itemCol.SetWidth(125);
#endif
    InsertColumn(static_cast<long>(ColumnID::Ascii), itemCol);

#ifdef __WINDOWS__
    SetMinSize(wxSize(75+345+135+20, 150));
#elif __WXGTK__
    SetMinSize(wxSize(75+340+125, 150));
#endif

    RefreshValues();
}

MemoryBrowser::~MemoryBrowser()
{
	delete itemAttr;
}

bool MemoryBrowser::Create(wxWindow* parent)
{
    return wxListView::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_VIRTUAL | wxLC_NO_HEADER);
}

wxString GetCategory(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x7FFF)
	{
		return "CART";
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		return "VRAM";
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		return "ERAM";
	}
	else if (address >= 0xC000 && address <= 0xDFFF)
	{
		return "WRAM";
	}
	else if (address >= 0xE000 && address <= 0xFDFF)
	{
		return "BLNK";
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		return " OAM";
	}
	else if (address >= 0xFEA0 && address <= 0xFEFF)
	{
		return "BLNK";
	}
    else if (address >= 0xFF00 && address <= 0xFF7F)
    {
        return "REGI";
    }
    else if (address >= 0xFF80 && address <= 0xFFFE)
    {
        return "HRAM";
    }
    else if (address == 0xFFFF)
    {
        return "REGI";
    }
    else
    {
        return "ERROR";
    }
    
}

wxString HexToAscii(uint8_t x)
{
    if(x < 0x20 || x == 0x7F)
    {
        return ".";
    }
    else
    {
        std::stringstream oss;
        oss << x;
        return wxString(oss.str());
    }
    
}

void MemoryBrowser::RefreshValues()
{
	Memory::Map& map = bus->GetMemoryDump();

    memData.clear();

    for(uint16_t i = 0; i < 0x8000; i += 0x10)
    {
        MemData data;
        data.category = GetCategory(i + 0x8000);
        data.address = intToHexString(i + 0x8000, 2);
        data.values = "";
        data.ascii = "";

        for(uint8_t j = 0; j < 0x10; j++)
        {
            uint8_t value = map.raw.at(i + j);
            data.values += intToHexString(value, 1) + " ";
            data.ascii += HexToAscii(value);
        }

        data.values.erase(data.values.size() - 1);

        memData.push_back(data);
    }

    SetItemCount(memData.size());
    Refresh();
}

void MemoryBrowser::OnFocus(wxFocusEvent& evt)
{
    if(!HasFocus())
    {
        itemAttr->SetTextColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
    }
    else
    {
        itemAttr->SetTextColour(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOXTEXT));
    }

    evt.Skip();
}

wxString MemoryBrowser::OnGetItemText(long item, long column) const
{
    if (static_cast<size_t>(item) > memData.size())
    {
        return "";
    }

    switch (static_cast<ColumnID>(column))
    {
    case ColumnID::CategoryAddress:     return memData[item].category
                                            + ":" + memData[item].address;
    case ColumnID::Values:              return memData[item].values;
    case ColumnID::Ascii:               return memData[item].ascii;
    default: return "";   
    }
}

wxItemAttr* MemoryBrowser::OnGetItemAttr(long item) const
{
    return itemAttr;
}