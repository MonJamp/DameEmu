#include "AppMain.h"
#include "MainFrame.h"


// Disable warning causes by wxWidget's implementation of WinMain
#pragma warning(push)
#pragma warning(disable: 28251)
wxIMPLEMENT_APP(AppMain);
#pragma warning(pop)

AppMain::AppMain()
{

}

AppMain::~AppMain()
{

}

bool AppMain::OnInit()
{
	MainFrame* mainFrame = new MainFrame();
	mainFrame->Show();
	return true;
}