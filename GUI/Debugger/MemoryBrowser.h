#pragma once
#include "../../Disassembler.h"
#include <wx/listctrl.h>


class MemoryBrowser : public wxListView
{
public:
    struct MemData
    {
        wxString category;
        wxString address;
        wxString values;
        wxString ascii;
    };

public:
    MemoryBrowser(std::shared_ptr<Bus> b, wxWindow* parent);
    ~MemoryBrowser();

    void RefreshValues();

protected:
    bool Create(wxWindow* parent);
    wxString OnGetItemText(long item, long column) const;
    wxItemAttr* OnGetItemAttr(long itme) const;

private:
    std::vector<MemData> memData;
    std::shared_ptr<Bus> bus;

private:
    enum class ColumnID
    {
        Empty,
        CategoryAddress,
        Values,
        Ascii
    };

private:
	wxItemAttr* itemAttr;
};