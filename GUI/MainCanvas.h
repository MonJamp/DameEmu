#pragma once
#include "SfmlCanvas.h"
#include <array>


class MainCanvas : public SfmlCanvas
{
public:
	MainCanvas(wxWindow* parent, uint8_t scale);
	~MainCanvas();

	void UpdateLine(int line, std::array<uint8_t, 160 * 4> pixels);
private:
	virtual void OnUpdate();

	sf::Texture* texture;
	sf::Sprite sprite;
};