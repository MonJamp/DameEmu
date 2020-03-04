#include "AppMain.h"

wxIMPLEMENT_APP(AppMain);

AppMain::AppMain()
{

}

AppMain::~AppMain()
{

}

bool AppMain::OnInit()
{
	mainFrame = new MainFrame();
	mainFrame->Show();
	return true;
}