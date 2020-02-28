#include "DisasmFrame.h"
#include "wx/wfstream.h"

wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_MENU(wxID_OPEN, DisasmFrame::OnOpen)
	EVT_MENU(wxID_EXIT, DisasmFrame::OnExit)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame()
	: wxFrame(nullptr, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400))
{
	disasm = new Dissassembler();

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
	itemCol.SetId(0);
	itemCol.SetWidth(20);
	listDisasm->InsertColumn(0, itemCol);
	// Address
	itemCol.SetId(1);
	itemCol.SetWidth(45);
	listDisasm->InsertColumn(1, itemCol);
	// Mnemonic
	itemCol.SetId(2);
	itemCol.SetWidth(70);
	listDisasm->InsertColumn(2, itemCol);
	// Operand
	itemCol.SetId(3);
	itemCol.SetWidth(150);
	listDisasm->InsertColumn(3, itemCol);
}

void DisasmFrame::PopulateList()
{
	size_t numOfIns = disasm->GetNumOfInstructions();

	for (unsigned int i = 0; i < numOfIns; i++) {
		wxString address = disasm->GetAddress(i);
		wxString mnemonic = disasm->GetMnemonic(i);
		wxString operand = disasm->GetOperands(i);

		listDisasm->InsertItem(i, "");
		listDisasm->SetItem(i, 1, address);
		listDisasm->SetItem(i, 2, mnemonic);
		listDisasm->SetItem(i, 3, operand);

		wxYieldIfNeeded();
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
	disasm->LoadCartridge(filename);
	// TODO: Check validity of rom
	disasm->Disassemble();
	PopulateList();
}

void DisasmFrame::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
