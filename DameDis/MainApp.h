#pragma once
#include <wx/wx.h>

class MainApp : public wxApp
{
public:
	MainApp();
	~MainApp();

	virtual bool OnInit();
};

