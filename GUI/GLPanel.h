#pragma once
#include <wx/wx.h>
#include <SFML/Graphics.hpp>


class GLPanel : public wxControl, public sf::RenderWindow
{
public:
	GLPanel(wxWindow* parent);
	
	virtual ~GLPanel();

private:
	virtual void OnUpdate();
	void OnIdle(wxIdleEvent& evt);
	void OnPaint(wxPaintEvent& evt);
	void OnEraseBackground(wxEraseEvent& evt);
	void OnSize(wxSizeEvent& evt);

	DECLARE_EVENT_TABLE()
};