#include "DisasmFrame.h"
#include <sstream>
#include <iomanip>
#include "wx/wfstream.h"

wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_MENU(wxID_OPEN, DisasmFrame::OnOpen)
	EVT_MENU(wxID_EXIT, DisasmFrame::OnExit)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame()
	: wxFrame(nullptr, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400)),
	  disasmList(this)
{
	InitMenuBar();
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
	// TODO: Check validity of rom
	disasm->LoadCartridge(filename);
	// TODO: Show progress bar
	disasm->Disassemble();
	disasmList.StoreDisassembly(disasm);
}

void DisasmFrame::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
