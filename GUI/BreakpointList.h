#pragma once
#include "../Disassembler.h"
#include <cstdint>
#include <vector>
#include <wx/listctrl.h>


class BreakpointList : public wxListView
{
public:
	BreakpointList(wxWindow* parent);
	void SetItems(Disassembly breakpoints);

protected:
	bool Create(wxWindow* parent);
	wxString OnGetItemText(long item, long column) const;

private:
	enum class ColumnID
	{
		Address,
		Instruction,
		Enabled
	};

	struct BreakpointData
	{
		wxString address;
		wxString instruction;
		wxString enabled;
	};

private:
	void Init();

private:
	std::vector<BreakpointData> bpData;

};