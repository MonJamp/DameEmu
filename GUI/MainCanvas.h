#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <array>


using FrameBuffer = std::array<uint8_t, 160 * 144 * 4>;

class MainCanvas
{
public:
	static MainCanvas& getInstance()
	{
		static MainCanvas instance;
		return instance;
	}

	static void Init();
	static void UpdateLine(int line, std::array<uint8_t, 160 * 4> pixels);
	static void UpdateCanvas(FrameBuffer* framebuffer);
	static void Paint();
	static bool IsVblank();
	static sf::Sprite& GetSprite() { return getInstance().sprite; }

private:
	MainCanvas() {}
	~MainCanvas() { delete getInstance().texture; }

	sf::Texture* texture = new sf::Texture();
	sf::Sprite sprite = sf::Sprite();
	bool is_vblank = false;
};