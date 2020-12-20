#include "MemoryBrowser.h"

MemoryBrowser::MemoryBrowser(std::shared_ptr<Bus> b, wxWindow* parent)
    : bus(b)
{
#ifdef __WINDOWS__
	// Disables vertical gap between columns
	wxSystemThemedControl::EnableSystemTheme(false);
#endif
    
    Create(parent);

    wxColor fontColor = wxColor(*wxBLACK);
    wxColor bgColor = wxColor(*wxWHITE);
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
    itemCol.SetWidth(340);
    InsertColumn(static_cast<long>(ColumnID::Values), itemCol);
    // Ascii
    itemCol.SetId(wxID_ANY);
    itemCol.SetWidth(125);
    InsertColumn(static_cast<long>(ColumnID::Ascii), itemCol);

    SetMinSize(wxSize(75+340+125, 100));

    RefreshValues();
}

MemoryBrowser::~MemoryBrowser()
{

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
    std::array<uint8_t, 0x8000> dump = bus->GetMemoryDump();

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
            uint8_t value = dump.at(i + j);
            data.values += intToHexString(value, 1) + " ";
            data.ascii += HexToAscii(value);
        }

        data.values.erase(data.values.size() - 1);

        memData.push_back(data);
    }

    SetItemCount(memData.size());
    Refresh();
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