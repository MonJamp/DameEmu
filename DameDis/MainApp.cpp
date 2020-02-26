#include "MainApp.h"
#include "DisasmFrame.h"

wxIMPLEMENT_APP(MainApp);

MainApp::MainApp()
{

}

MainApp::~MainApp()
{

}

bool MainApp::OnInit()
{
	DisasmFrame* frame = new DisasmFrame();
	frame->Show();
	return true;
}
