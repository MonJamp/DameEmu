#include "SfmlCanvas.h"

#ifdef __WXGTK__
    #include <gtk/gtk.h>
	#include <gdk/gdkx.h>
#endif


BEGIN_EVENT_TABLE(SfmlCanvas, wxControl)
	EVT_PAINT(SfmlCanvas::OnPaint)
	EVT_IDLE(SfmlCanvas::OnIdle)
	EVT_ERASE_BACKGROUND(SfmlCanvas::OnEraseBackground)
	EVT_SIZE(SfmlCanvas::OnSize)
END_EVENT_TABLE()

SfmlCanvas::SfmlCanvas(wxWindow* parent)
	: wxControl(parent, wxID_ANY)
{
#ifdef __WXGTK__
	// Get handle of X11 window
	gtk_widget_realize(m_wxwindow);
	gtk_widget_set_double_buffered(m_wxwindow, false);
	GdkWindow* Win = gtk_widget_get_window(m_wxwindow);
	XFlush(GDK_WINDOW_XDISPLAY(Win));
	sf::RenderWindow::create(gdk_x11_window_get_xid(Win));
#else
	sf::RenderWindow::create(GetHandle());
#endif
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
