#pragma once
#include "../SfmlCanvas.h"
#include <SFML/Graphics.hpp>

class VramCanvas : public SfmlCanvas
{
public:
    VramCanvas(wxWindow* parent);
    virtual ~VramCanvas();

private:
    virtual void OnUpdate();
};

class VramFrame : public wxFrame
{
public:
    VramFrame(wxWindow* parent);
    ~VramFrame();

private:
    VramCanvas* canvas;
};