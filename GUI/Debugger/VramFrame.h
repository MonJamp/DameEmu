#pragma once
#include "../SfmlCanvas.h"
#include "../../Bus.h"
#include <SFML/Graphics.hpp>

class VramCanvas : public SfmlCanvas
{
public:
    VramCanvas(std::shared_ptr<Bus> b, wxWindow* parent);
    virtual ~VramCanvas();

private:
    virtual void OnUpdate();

	std::shared_ptr<Bus> bus;
};

class VramFrame : public wxFrame
{
public:
    VramFrame(std::shared_ptr<Bus> b, wxWindow* parent);
    ~VramFrame();

private:
    VramCanvas* canvas;
};