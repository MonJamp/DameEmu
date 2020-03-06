#include "DisasmFrame.h"


wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_MENU(wxID_EXIT, DisasmFrame::OnExit)
	EVT_BUTTON(ButtonID::Step, DisasmFrame::OnStep)
wxEND_EVENT_TABLE()


DisasmListCtrl::DisasmListCtrl(wxWindow* parent)
	: wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_VIRTUAL | wxLC_NO_HEADER)
{
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
}

void DisasmListCtrl::StoreDisassembly(std::shared_ptr<Disassembly> disasm)
{
	disasmData.clear();

	for (auto i : *disasm)
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
}

void DisasmListCtrl::StoreAddressTable(AddressTable at)
{
	addressTable = at;
}

void DisasmListCtrl::ShowAddress(uint16_t a)
{
	// Deselect previous item
	SetItemState(selectedItem, 0, wxLIST_STATE_SELECTED);
	// Select new item
	EnsureVisible(addressTable[a]);
	SetItemState(addressTable[a], wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	selectedItem = addressTable[a];
}

wxString DisasmListCtrl::OnGetItemText(long item, long column) const
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

DisasmFrame::DisasmFrame(std::shared_ptr<Debugger>& d, wxWindow* parent)
	: wxFrame(parent, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400)),
	debugger(d)
{
	InitMenuBar();
	InitWidgets();
	
	disasmList->StoreDisassembly(debugger->GetDisassembly());
	disasmList->StoreAddressTable(debugger->GetAddressTable());
	disasmList->ShowAddress(debugger->cpuState.pc);
	regPanel->UpdateValues(debugger->cpuState);
}

DisasmFrame::~DisasmFrame()
{
	
}

void DisasmFrame::InitMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	
	SetMenuBar(menuBar);
}

void DisasmFrame::InitWidgets()
{
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	btnPanel = new ButtonPanel(this);
	vbox->Add(btnPanel, wxSizerFlags(0));

	disasmList = new DisasmListCtrl(this);
	hbox->Add(disasmList, wxSizerFlags(3).Expand());
	regPanel = new RegPanel(this);
	hbox->Add(regPanel, wxSizerFlags(1).Expand());

	vbox->Add(hbox, wxSizerFlags(1).Expand());
	SetSizer(vbox);
}

void DisasmFrame::OnStep(wxCommandEvent& evt)
{
	debugger->Step();
	regPanel->UpdateValues(debugger->cpuState);
	disasmList->ShowAddress(debugger->cpuState.pc);
}

void DisasmFrame::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
