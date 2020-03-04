#pragma once
#include "wx_header.h"


class AppMain : public wxApp
{
public:
	AppMain();
	~AppMain();

	virtual bool OnInit();
};