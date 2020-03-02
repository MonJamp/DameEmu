#include "MainApp.h"
#include "MainFrame.h"

// Disable warning causes by wxWidget's implementation of WinMain
#pragma warning(push)
#pragma warning(disable: 28251)
wxIMPLEMENT_APP(MainApp);
#pragma warning(pop)

MainApp::MainApp()
{

}

MainApp::~MainApp()
{

}

bool MainApp::OnInit()
{
	MainFrame* frame = new MainFrame();
	frame->Show();
	return true;
}
