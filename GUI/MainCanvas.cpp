#include "MainCanvas.h"


void MainCanvas::Init()
{
	sf::Texture& texture = *getInstance().texture;
	sf::Sprite& sprite = getInstance().sprite;
	bool& is_vblank = getInstance().is_vblank;

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

	texture.create(160, 144);
	texture.update(white_pixels);

	sprite.setTexture(texture);
	sprite.setScale({ 3.f, 3.f });

	is_vblank = false;
}

void MainCanvas::UpdateLine(int line, std::array<uint8_t, 160 * 4> pixels)
{
	sf::Texture& texture = *getInstance().texture;
	texture.update(pixels.data(), 160, 1, 0, line);
}

void MainCanvas::UpdateCanvas(FrameBuffer* framebuffer)
{
	sf::Texture& texture = *getInstance().texture;
	texture.update(framebuffer->data());
}

void MainCanvas::Paint()
{
	bool& shouldDraw = getInstance().is_vblank;
	shouldDraw = true;
}

bool MainCanvas::IsVblank()
{
	bool& is_vblank = getInstance().is_vblank;

	if (is_vblank)
	{
		is_vblank = false;
		return true;
	}
	else
	{
		return false;
	}
}
