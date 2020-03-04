#pragma once

#include "MainFrame.h"
#include "wx_header.h"

class AppMain : public wxApp
{
public:
	AppMain();
	~AppMain();

	virtual bool OnInit();

private:
	MainFrame* mainFrame = nullptr;
};