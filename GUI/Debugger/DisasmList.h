#pragma once
#include "../../Disassembler.h"
#include "../wx_header.h"


wxDECLARE_EVENT(TOGGLE_BREAKPOINT, wxCommandEvent);

class DisasmList : public wxListView
{
public:
	struct InsData
	{
		wxString address;
		wxString opcode;
		wxString mnemonic;
		wxString operand;
		wxString comment;
	};

	enum EventID
	{
		LIST_CTRL,
		ToggleBreakpoint,
	};

public:
	DisasmList(std::shared_ptr<Debugger> d, wxWindow* parent);
	~DisasmList();

	void RefreshValues();
	void ShowCurrentAddress();
	void ShowAddress(uint16_t a);

protected:
	bool Create(wxWindow* parent);
	wxString OnGetItemText(long item, long column) const;
	wxItemAttr* OnGetItemAttr(long item) const;

private:
	std::vector<InsData> disasmData;
	std::shared_ptr<Debugger> debugger;
	AddressTable addressToIndex; // Address is key, index is value

private:
	void OnItemRightClick(wxListEvent& evt);
	void OnItemDoubleClick(wxListEvent& evt);
	void OnItemAction(wxCommandEvent& evt);

	void OnToggleBreakpoint(long item);

	wxDECLARE_EVENT_TABLE();

private:
	enum class ColumnID
	{
		Empty,
		Address,
		Opcode,
		Mnemonic,
		Operand,
		Comment
	};

private:
	wxItemAttr* itemAttr;
};