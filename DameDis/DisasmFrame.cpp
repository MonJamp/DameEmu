#include "DisasmFrame.h"
#include <sstream>
#include <iomanip>
#include "wx/wfstream.h"

wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_MENU(wxID_OPEN, DisasmFrame::OnOpen)
	EVT_MENU(wxID_EXIT, DisasmFrame::OnExit)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame()
	: wxFrame(nullptr, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400))
{
	listDisasm = new wxListCtrl(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxLC_REPORT | wxLC_NO_HEADER
	);

	InitMenuBar();
	ResetDisassemblyList();
}

DisasmFrame::~DisasmFrame()
{
	delete disasm;
}

void DisasmFrame::InitMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	menuFile->Append(wxID_OPEN, "&Open ROM File\tCtrl-O");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	
	SetMenuBar(menuBar);
}

void DisasmFrame::ResetDisassemblyList()
{
	listIterator = 0;
	listDisasm->ClearAll();

	wxListItem itemCol;
	// Empty
	itemCol.SetId(static_cast<long>(ColumnID::Empty));
	itemCol.SetWidth(20);
	listDisasm->InsertColumn(0, itemCol);
	// Address
	itemCol.SetId(static_cast<long>(ColumnID::Address));
	itemCol.SetWidth(45);
	listDisasm->InsertColumn(1, itemCol);
	// Opcode
	itemCol.SetId(static_cast<long>(ColumnID::Opcode));
	itemCol.SetWidth(70);
	listDisasm->InsertColumn(2, itemCol);
	// Mnemonic
	itemCol.SetId(static_cast<long>(ColumnID::Mnemonic));
	itemCol.SetWidth(70);
	listDisasm->InsertColumn(3, itemCol);
	// Operand
	itemCol.SetId(static_cast<long>(ColumnID::Operand));
	itemCol.SetWidth(150);
	listDisasm->InsertColumn(4, itemCol);
	// Operand
	itemCol.SetId(static_cast<long>(ColumnID::Comment));
	itemCol.SetWidth(150);
	listDisasm->InsertColumn(5, itemCol);
}

// Reserves space in the list, fixes moving scroll bar
void DisasmFrame::ReserveListItems(unsigned int size)
{
	int x = 0;
	// TODO: Make class derived from listctrl for better performance
	// This function shows that listctrl is very slow when inserting items
	// After reserving list items then populating list, the item are
	// loaded much faster than before but the initial reserve stage
	// takes a while to finish
	for (unsigned int i = 0; i < size; i++) {
		listDisasm->InsertItem(i, "");
		listDisasm->SetItem(i, static_cast<long>(ColumnID::Comment), "Loading...");

		// Show progress
		// TODO: Placeholder for progress bar
		std::stringstream ss;
		char p[4] = {'\\', '|', '/', '-'};
		(i % 1000) ? x : x++;
		(x >= 4) ? x = 0 : x;
		ss << "Initializing list " << p[x % 4];
		listDisasm->SetItem(0, static_cast<long>(ColumnID::Operand), ss.str());
		ss.str("");
		ss << i << " of " << size;
		listDisasm->SetItem(0, static_cast<long>(ColumnID::Comment), ss.str());

		// HACK: Other events could result in list being corrupted
		// wxYield should not be used while inserting to list
		wxYieldIfNeeded();
	}

	listDisasm->SetItem(0, static_cast<long>(ColumnID::Operand), "Populating list...");
}

void DisasmFrame::PopulateList(wxIdleEvent& evt)
{
	// Perhaps it would be best to stall program until insertion finishes
	// Need to investigate performance options
	if (listIterator < disasm->GetNumOfInstructions()) {
		wxString address = disasm->GetAddress(listIterator);
		wxString opcode = disasm->GetOpcode(listIterator);
		wxString mnemonic = disasm->GetMnemonic(listIterator);
		wxString operand = disasm->GetOperands(listIterator);
		wxString comment = disasm->GetComment(listIterator);

		//listDisasm->InsertItem(listIterator, "");
		listDisasm->SetItem(listIterator, static_cast<long>(ColumnID::Address), address);
		listDisasm->SetItem(listIterator, static_cast<long>(ColumnID::Opcode), opcode);
		listDisasm->SetItem(listIterator, static_cast<long>(ColumnID::Mnemonic), mnemonic);
		listDisasm->SetItem(listIterator, static_cast<long>(ColumnID::Operand), operand);
		listDisasm->SetItem(listIterator, static_cast<long>(ColumnID::Comment), comment);

		listIterator++;
		evt.RequestMore();
	}
	else
	{
		Disconnect(wxEVT_IDLE, wxIdleEventHandler(DisasmFrame::PopulateList));
	}
}

void DisasmFrame::OnDisassemble(wxIdleEvent& evt)
{
	if (!disasm->isDisassembled())
	{
		disasm->Disassemble();
		evt.RequestMore();
	}
	else
	{
		Disconnect(wxEVT_IDLE, wxIdleEventHandler(DisasmFrame::OnDisassemble));
		ResetDisassemblyList();
		ReserveListItems(disasm->GetNumOfInstructions());
		Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(DisasmFrame::PopulateList));
	}
}

void DisasmFrame::OnOpen(wxCommandEvent& evt)
{
	wxFileDialog fileDialog(
		this,
		"Open GB File",
		wxEmptyString,
		wxEmptyString,
		"GB ROM (*.gb) | *.gb",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	wxFileInputStream fileStream(fileDialog.GetPath());
	if (!fileStream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", fileDialog.GetPath());
		return;
	}

	wxString filename = fileDialog.GetPath();
	if (disasm != nullptr)
		delete disasm;
	disasm = new Dissassembler();
	disasm->LoadCartridge(filename);
	// TODO: Check validity of rom
	Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(DisasmFrame::OnDisassemble));
}

void DisasmFrame::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
