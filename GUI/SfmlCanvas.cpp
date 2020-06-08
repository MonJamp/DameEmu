#include "SfmlCanvas.h"


BEGIN_EVENT_TABLE(SfmlCanvas, wxControl)
	EVT_PAINT(SfmlCanvas::OnPaint)
	EVT_IDLE(SfmlCanvas::OnIdle)
	EVT_ERASE_BACKGROUND(SfmlCanvas::OnEraseBackground)
	EVT_SIZE(SfmlCanvas::OnSize)
END_EVENT_TABLE()

SfmlCanvas::SfmlCanvas(wxWindow* parent)
	: wxControl(parent, wxID_ANY)
{
	sf::RenderWindow::create(GetHandle());
	clear(sf::Color::Black);
}

SfmlCanvas::~SfmlCanvas()
{

}

void SfmlCanvas::OnUpdate()
{
	
}

void SfmlCanvas::OnIdle(wxIdleEvent& evt)
{
	Refresh();
}

void SfmlCanvas::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC Dc(this);

	OnUpdate();

	display();
}

void SfmlCanvas::OnEraseBackground(wxEraseEvent& evt)
{

}

void SfmlCanvas::OnSize(wxSizeEvent& evt)
{
	setSize(sf::Vector2u(evt.GetSize().x, evt.GetSize().y));
	setView(sf::View(sf::FloatRect(0, 0, evt.GetSize().x, evt.GetSize().y)));
}