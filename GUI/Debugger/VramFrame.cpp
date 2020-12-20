#include "VramFrame.h"


VramCanvas::VramCanvas(wxWindow* parent) : SfmlCanvas(parent)
{

}

VramCanvas::~VramCanvas()
{

}

void VramCanvas::OnUpdate()
{
    clear(sf::Color::White);
}

VramFrame::VramFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "VRAM Viewer", wxDefaultPosition, wxSize(400,400))
{
    canvas = new VramCanvas(this);
}

VramFrame::~VramFrame()
{

}
