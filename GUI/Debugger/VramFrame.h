#pragma once
#include "../SfmlCanvas.h"
#include "../../Bus.h"
#include <SFML/Graphics.hpp>
#include <vector>

class VramCanvas : public SfmlCanvas
{
public:
    VramCanvas(std::shared_ptr<Bus> b, wxWindow* parent);
    virtual ~VramCanvas();

private:
    virtual void OnUpdate();

	std::shared_ptr<Bus> bus;
	std::vector<sf::Texture*> textures;
	std::vector<sf::Sprite> sprites;

	int scale, posX, posY;
	uint16_t tileIndex; // Keeps track of which tile to draw next
};

class VramFrame : public wxFrame
{
public:
    VramFrame(std::shared_ptr<Bus> b, wxWindow* parent);
    ~VramFrame();

private:
    VramCanvas* canvas;
};