#include "MainFrame.h"
#include "wx/wfstream.h"
#include "wx/aboutdlg.h"


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(MenuID::LOAD, MainFrame::OnLoadROM)
	EVT_MENU(MenuID::RUN, MainFrame::OnRunEmu)
	EVT_MENU(MenuID::EXIT, MainFrame::OnExit)
	EVT_MENU(MenuID::SETTINGS, MainFrame::OnSettings)
	EVT_MENU(MenuID::DEBUGGER, MainFrame::OnDebugger)
	EVT_MENU(MenuID::ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()


MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "DameEmu", wxDefaultPosition, wxSize(600,400))
{
	menuBar = new wxMenuBar();

	systemMenu = new wxMenu();
	openItem = new wxMenuItem(systemMenu, MenuID::LOAD, _("&Load ROM"));
	systemMenu->Append(openItem);
	runItem = new wxMenuItem(systemMenu, MenuID::RUN, _("&Run"));
	systemMenu->Append(runItem);
	systemMenu->AppendSeparator();
	exitItem = new wxMenuItem(systemMenu, MenuID::EXIT, _("E&xit"));
	systemMenu->Append(exitItem);
	menuBar->Append(systemMenu, _("&System"));

	emulationMenu = new wxMenu();
	settingsItem = new wxMenuItem(emulationMenu, MenuID::SETTINGS, _("&Settings"));
	emulationMenu->Append(settingsItem);
	debuggerItem = new wxMenuItem(emulationMenu, MenuID::DEBUGGER, _("&Debugger"));
	emulationMenu->Append(debuggerItem);
	menuBar->Append(emulationMenu, _("&Emulation"));

	miscMenu = new wxMenu();
	aboutItem = new wxMenuItem(miscMenu, MenuID::ABOUT, _("&About"));
	miscMenu->Append(aboutItem);
	menuBar->Append(miscMenu, _("&Misc"));

	SetMenuBar(menuBar);
}

MainFrame::~MainFrame()
{
	delete dameEmu;
}

void MainFrame::OnLoadROM(wxCommandEvent& evt)
{
	wxFileDialog openFileDialog(
		this,
		_("Open GB file"),
		"",
		"",
		"GB files (*.gb) | *.gb",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk()) {
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}

	rom_dir = openFileDialog.GetPath();
	wxMessageBox(rom_dir);
}

void MainFrame::OnRunEmu(wxCommandEvent& evt)
{
	runItem->Enable(false);
	dameEmu = new DameEmu(rom_dir);
	Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdleRun));
}

void MainFrame::OnIdleRun(wxIdleEvent& evt)
{
	dameEmu->Run();
}

void MainFrame::OnExit(wxCommandEvent& evt)
{
	Close(true);
}

void MainFrame::OnSettings(wxCommandEvent& evt)
{
	//TODO: Implement settings
	wxMessageBox("TODO");
}

void MainFrame::OnDebugger(wxCommandEvent& evt)
{
	if (wxWindow::FindWindowByName("DisasmFrame"))
	{
		disasmFrame->Raise();
	}
	else
	{
		disasmFrame = new DisasmFrame(this);
		disasmFrame->SetName("DisasmFrame");
		disasmFrame->Show();
	}
}

void MainFrame::OnAbout(wxCommandEvent& evt)
{
	wxAboutDialogInfo info;
	info.SetName(_("DameEmu"));
	info.SetWebSite("https://github.com/MonJamp/DameEmu");
	wxAboutBox(info, this);
}