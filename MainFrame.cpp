#include "MainFrame.h"
#include "wx/wfstream.h"


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
	: wxFrame(nullptr, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400)),
	  disasmList(this)
{
	InitMenuBar();
}

MainFrame::~MainFrame()
{
	
}

void MainFrame::InitMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	menuFile->Append(wxID_OPEN, "&Open ROM File\tCtrl-O");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	
	SetMenuBar(menuBar);
}

void MainFrame::OnOpen(wxCommandEvent& evt)
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

	std::unique_ptr<Disassembler> disasm(new Disassembler);
	// TODO: Check validity of rom
	disasm->LoadCartridge(std::string(filename.mb_str()));
	// TODO: Show progress bar
	disasm->Disassemble();
	disasmList.StoreDisassembly(disasm->GetDisassembly());
}

void MainFrame::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
