#pragma once
#include "wx_header.h"
#include <SFML/Graphics.hpp>


class SfmlCanvas : public wxControl, public sf::RenderWindow
{
public:
	SfmlCanvas(wxWindow* parent);
	
	virtual ~SfmlCanvas();

private:
	virtual void OnUpdate();
	void OnIdle(wxIdleEvent& evt);
	void OnPaint(wxPaintEvent& evt);
	void OnEraseBackground(wxEraseEvent& evt);
	void OnSize(wxSizeEvent& evt);

public:
	void PaintNow();

	DECLARE_EVENT_TABLE()
};