#pragma once
#include "../Disassembler.h"
#include <cstdint>
#include <vector>
#include <wx/listctrl.h>


class BreakpointList : public wxListView
{
public:
	enum EventID
	{
		LIST_CTRL,
		AddBreakpoint,
		RemoveBreakpoint
	};

public:
	BreakpointList(std::shared_ptr<Debugger> d, wxWindow* parent);
	void SetItems();

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

	void OnItemRightClick(wxListEvent& evt);
	void OnPopupClick(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
	std::shared_ptr<Debugger> debugger;
	std::vector<BreakpointData> bpData;

};