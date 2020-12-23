#pragma once
#include "../../Disassembler.h"
#include "../wx_header.h"


class MemoryBrowser : public wxListView
{
public:
    MemoryBrowser(std::shared_ptr<Bus> b, wxWindow* parent);
    ~MemoryBrowser();

    void OnFocus(wxFocusEvent& evt);

protected:
    bool Create(wxWindow* parent);
    wxString OnGetItemText(long item, long column) const;
    wxItemAttr* OnGetItemAttr(long itme) const;

    wxDECLARE_EVENT_TABLE();

private:
	Memory::Map& map;

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