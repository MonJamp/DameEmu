#include "MainFrame.h"


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(MenuID::LOAD, MainFrame::OnLoadROM)
	EVT_MENU(MenuID::RUN, MainFrame::OnRunEmu)
	EVT_MENU(MenuID::EXIT, MainFrame::OnExit)
	EVT_MENU(MenuID::SETTINGS, MainFrame::OnSettings)
	EVT_MENU(MenuID::DEBUGGER, MainFrame::OnDebugger)
	EVT_MENU(MenuID::ABOUT, MainFrame::OnAbout)
#ifdef _DEBUG
	EVT_MENU(MenuID::CHECK, MainFrame::OnCheckCart)
	EVT_MENU(MenuID::UNLOAD, MainFrame::OnUnloadROM)
#endif
wxEND_EVENT_TABLE()


MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "DameEmu", wxDefaultPosition, wxSize(600,400))
{
	dameEmu = std::make_unique<DameEmu>();

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

#ifdef _DEBUG
	wxMenu* debugMenu = new wxMenu();
	wxMenuItem* checkCartItem = new wxMenuItem(debugMenu, MenuID::CHECK, _("&Check Cartridge"));
	debugMenu->Append(checkCartItem);
	wxMenuItem* unloadItem = new wxMenuItem(debugMenu, MenuID::UNLOAD, _("&Reset dameEmu"));
	debugMenu->Append(unloadItem);
	menuBar->Append(debugMenu, _("&Debug"));
#endif

	SetMenuBar(menuBar);

	glPanel = new SfmlCanvas(this);
}

MainFrame::~MainFrame()
{

}

void MainFrame::OnLoadROM(wxCommandEvent& evt)
{
	wxFileDialog openFileDialog(
		this,
		_("Open GB file"),
		"",
		"",
		"GB files (*.gb)|*.gb",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	wxFileInputStream fileStream(openFileDialog.GetPath());
	if (!fileStream.IsOk()) {
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}

	wxString filename = openFileDialog.GetPath();
	dameEmu = std::make_unique<DameEmu>(std::string(filename.mb_str()));
	dameEmu->SetCanvas(glPanel);
}

void MainFrame::OnRunEmu(wxCommandEvent& evt)
{
	runItem->Enable(false);
	Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdleRun));
}

void MainFrame::OnIdleRun(wxIdleEvent& evt)
{
	dameEmu->Step();
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
		disasmFrame = new DisasmFrame(dameEmu->GetDebugger(), this);
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

#ifdef _DEBUG
void MainFrame::OnCheckCart(wxCommandEvent& evt)
{
	Cartridge cart = dameEmu->GetCart();
	if (cart.isValid())
	{
		wxMessageBox(cart.headerToString());
	}
	else
	{
		wxMessageBox("ROM is invalid");
	}
}

void MainFrame::OnUnloadROM(wxCommandEvent& evt)
{
	dameEmu.reset();
}
#endif
