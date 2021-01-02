#include "MainCanvas.h"


MainCanvas::MainCanvas(wxWindow* parent, uint8_t scale)
	: SfmlCanvas(parent)
{
	SetMinSize(wxSize(scale * 160, scale * 144));

	uint8_t white_pixels[160 * 144 * 4];
	for (uint8_t i = 0; i < 160; i++)
	{
		for (uint8_t j = 0; j < 144; j++)
		{
			white_pixels[i * 576 + j * 4] = 0xFF;
			white_pixels[i * 576 + j * 4 + 1] = 0xFF;
			white_pixels[i * 576 + j * 4 + 2] = 0xFF;
			white_pixels[i * 576 + j * 4 + 3] = 0xFF;
		}
	}

	texture = new sf::Texture();
	texture->create(160, 144 + 1);
	texture->update(white_pixels);

	sprite.setTexture(*texture);
	sprite.setScale(3.f, 3.f);
	sprite.setPosition(0.f, 0.f);
}

MainCanvas::~MainCanvas()
{
	delete texture;
}

void MainCanvas::OnUpdate()
{
	clear(sf::Color::Black);
	draw(sprite);
}

void MainCanvas::UpdateLine(int line, std::array<uint8_t, 160 * 4> pixels)
{
	texture->update(pixels.data(), 160, 1, 0, line);
}

std::array<uint8_t, 160 * 4> MainCanvas::GetLine(int line)
{
	std::array<uint8_t, 160 * 4> pixels;
	sf::Image image = texture->copyToImage();
	const sf::Uint8* pixelsFull = image.getPixelsPtr();

	assert((line * 160 * 4 + (pixels.size() / 4) * 4 + 3) < (image.getSize().x * image.getSize().y * 4));

	for (unsigned int i = 0; i < (pixels.size() / 4); i++)
	{
		pixels[i * 4] = pixelsFull[line * 160 * 4 + i * 4];
		pixels[i * 4 + 1] = pixelsFull[line * 160 * 4 + i * 4 + 1];
		pixels[i * 4 + 2] = pixelsFull[line * 160 * 4 + i * 4 + 2];
		pixels[i * 4 + 3] = pixelsFull[line * 160 * 4 + i * 4 + 3];
	}

	return pixels;
}