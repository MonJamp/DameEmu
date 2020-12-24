#pragma once
#include "SfmlCanvas.h"


class MainCanvas : public SfmlCanvas
{
public:
	MainCanvas(wxWindow* parent, uint8_t scale);
	~MainCanvas();

	void UpdateLine(int line, uint8_t pixels[160 * 4]);
private:
	virtual void OnUpdate();

	sf::Texture* texture;
	sf::Sprite sprite;
};