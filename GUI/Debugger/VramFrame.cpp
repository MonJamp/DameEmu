#include "VramFrame.h"
#include <vector>


VramCanvas::VramCanvas(std::shared_ptr<Bus> b, wxWindow* parent)
	: SfmlCanvas(parent), bus(b)
{

}

VramCanvas::~VramCanvas()
{

}

void VramCanvas::OnUpdate()
{
    uint8_t pixels[8 * 8 * 4];
	std::vector<sf::Texture*> textures;
	std::vector<sf::Sprite> sprites;
	float scale = 3.f;
	float posX = 0.f;
	float posY = 0.f;

	std::array<uint8_t, 0x2000>* vram = bus->GetVRAM();

    for(int k = 0; k < 256; k++)
    {
        for(int i = 0; i < 8; i++)
        {
			//uint8_t upper = bus->Read(0x8000 + k * 16 + i * 2);
			//uint8_t lower = bus->Read(0x8000 + k * 16 + i * 2 + 1);

			uint8_t upper = vram->at(k * 16 + i * 2);
			uint8_t lower = vram->at(k * 16 + i * 2 + 1);

            for(int j = 0; j < 8; j++)
            {
                bool upperBit = upper & 128;
                bool lowerBit = lower & 128;
                uint32_t color = 0x00000000;

                if(upperBit && lowerBit)
                {
                    //Color 4
                    color = 0xFF000000;
                }
                else if(upperBit && !lowerBit)
                {
                    //Color 3
                    color = 0xFF545454;
                }
                else if(!upperBit && lowerBit)
                {
                    //Color 2
                    color = 0xFFA9A9A9;
                }
                else
                {
                    //Color 1
                    color = 0xFFFFFFFF;
                }
                
                
                pixels[i * 32 + j * 4] = color & 0xFF;
                pixels[i * 32 + j * 4 + 1] = (color & 0xFF00) >> 8;
                pixels[i * 32 + j * 4 + 2] = (color & 0xFF0000) >> 16;
                pixels[i * 32 + j * 4 + 3] = (color & 0xFF000000) >> 24;
                
                upper <<= 1;
                lower <<= 1;
            }
        }

		sf::Texture* texture = new sf::Texture;
		texture->create(8, 8);
		texture->update(pixels);
		textures.push_back(texture);

		sf::Sprite sprite;
		sprite.setTexture(*textures.back());
		sprite.setScale(scale, scale);
		sprite.setPosition(posX, posY);
		sprites.push_back(sprite);

		posX += 8 * scale;
		if (posX == 8 * scale * 16)
		{
			posX = 0.f;
			posY += 8 * scale;
		}
    }

    clear(sf::Color::Black);
    
	for (auto s : sprites)
	{
		draw(s);
	}

	while (!textures.empty())
	{
		sf::Texture* texture = textures.back();
		textures.pop_back();
		delete texture;
	}
}

VramFrame::VramFrame(std::shared_ptr<Bus> b, wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "VRAM Viewer", wxDefaultPosition, wxSize(400,400))
{
    canvas = new VramCanvas(b, this);
	canvas->SetSize(canvas->GetBestSize());
}

VramFrame::~VramFrame()
{

}
