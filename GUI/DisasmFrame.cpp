#include "DisasmFrame.h"
#include "wx/wfstream.h"


wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_MENU(wxID_EXIT, DisasmFrame::OnExit)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame(std::shared_ptr<Disassembler>& disasm, wxWindow* parent)
	: wxFrame(parent, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400))
{
	InitMenuBar();

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	disasmList = new DisasmListCtrl(this);
	regPanel = new RegPanel(this);

	hbox->Add(disasmList, wxSizerFlags(3).Expand());
	hbox->Add(regPanel, wxSizerFlags(1).Expand());
	SetSizer(hbox);

	// TODO: Show progress bar
	disasm->Disassemble();
	disasmList->StoreDisassembly(disasm->GetDisassembly());
	disasmList->StoreJumpTable(disasm->jumpTable);
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

void DisasmFrame::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
