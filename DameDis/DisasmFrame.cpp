#include "DisasmFrame.h"
#include "wx/wfstream.h"

wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_MENU(wxID_OPEN, DisasmFrame::OnOpen)
	EVT_MENU(wxID_EXIT, DisasmFrame::OnExit)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame()
	: wxFrame(nullptr, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400))
{
	InitMenuBar();
	InitDisassemblyList();
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

void DisasmFrame::InitDisassemblyList()
{
	listDisasm = new wxListCtrl(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxLC_REPORT | wxLC_NO_HEADER
	);

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
}

void DisasmFrame::PopulateList()
{
	// HACK: This whole function will probably need to be rewritten
	listDisasm->DeleteAllItems();

	size_t numOfIns = disasm->GetNumOfInstructions();

	for (unsigned int i = 0; i < numOfIns; i++) {
		wxString address = disasm->GetAddress(i);
		wxString opcode = disasm->GetOpcode(i);
		wxString mnemonic = disasm->GetMnemonic(i);
		wxString operand = disasm->GetOperands(i);

		listDisasm->InsertItem(i, "");
		listDisasm->SetItem(i, static_cast<long>(ColumnID::Address), address);
		listDisasm->SetItem(i, static_cast<long>(ColumnID::Opcode), opcode);
		listDisasm->SetItem(i, static_cast<long>(ColumnID::Mnemonic), mnemonic);
		listDisasm->SetItem(i, static_cast<long>(ColumnID::Operand), operand);

		// If program closes while in loop, exceptions are thrown
		// TODO: Safely close program even if list is still being populated
		wxYieldIfNeeded();
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
		PopulateList();
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
