#include "GLPanel.h"


BEGIN_EVENT_TABLE(GLPanel, wxControl)
	EVT_PAINT(GLPanel::OnPaint)
	EVT_IDLE(GLPanel::OnIdle)
	EVT_ERASE_BACKGROUND(GLPanel::OnEraseBackground)
	EVT_SIZE(GLPanel::OnSize)
END_EVENT_TABLE()

GLPanel::GLPanel(wxWindow* parent)
	: wxControl(parent, wxID_ANY)
{
	sf::RenderWindow::create(GetHandle());
	clear(sf::Color::Black);
}

GLPanel::~GLPanel()
{

}

void GLPanel::OnUpdate()
{
	
}

void GLPanel::OnIdle(wxIdleEvent& evt)
{
	Refresh();
}

void GLPanel::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC Dc(this);

	OnUpdate();

	display();
}

void GLPanel::OnEraseBackground(wxEraseEvent& evt)
{

}

void GLPanel::OnSize(wxSizeEvent& evt)
{
	setSize(sf::Vector2u(evt.GetSize().x, evt.GetSize().y));
	setView(sf::View(sf::FloatRect(0, 0, evt.GetSize().x, evt.GetSize().y)));
}