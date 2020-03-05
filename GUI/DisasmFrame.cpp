#include "DisasmFrame.h"
#include "wx/wfstream.h"


wxBEGIN_EVENT_TABLE(DisasmFrame, wxFrame)
	EVT_MENU(wxID_EXIT, DisasmFrame::OnExit)
	EVT_BUTTON(ButtonID::Step, DisasmFrame::OnStep)
wxEND_EVENT_TABLE()

DisasmFrame::DisasmFrame(wxWindow* parent)
	: wxFrame(parent, wxID_ANY, "DameDis", wxDefaultPosition, wxSize(600, 400))
{
	InitMenuBar();

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

void DisasmFrame::OnStep(wxCommandEvent& evt)
{
	// TODO connect this action with emulation
	wxMessageBox("TODO");
}

void DisasmFrame::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
