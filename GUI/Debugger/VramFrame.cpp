#include "VramFrame.h"


VramCanvas::VramCanvas(std::shared_ptr<Bus> b, wxWindow* parent)
	: SfmlCanvas(parent), bus(b)
{
	scale = 3;
	posX = 0;
	posY = 0;
	tileIndex = 0;

	SetMinSize(wxSize(scale * 8 * 16, scale * 8 * 16));

	uint8_t white_pixels[8 * 8 * 4];
	for (uint8_t i = 0; i < 8; i++)
	{
		for (uint8_t j = 0; j < 8; j++)
		{
			white_pixels[i * 32 + j * 4] = 0xFF;
			white_pixels[i * 32 + j * 4 + 1] = 0xFF;
			white_pixels[i * 32 + j * 4 + 2] = 0xFF;
			white_pixels[i * 32 + j * 4 + 3] = 0xFF;
		}
	}

	for (uint16_t i = 0; i < 256; i++)
	{
		// Init textures with white pixels
		sf::Texture* texture = new sf::Texture();
		texture->create(8, 8);
		texture->update(white_pixels);
		textures.push_back(texture);

		sf::Sprite sprite;
		sprite.setTexture(*textures.back());
		sprite.setScale((float)scale, (float)scale);
		sprite.setPosition((float)posX, (float)posY);
		sprites.push_back(sprite);

		posX += 8 * scale;
		if (posX == 8 * scale * 16)
		{
			posX = 0;
			posY += 8 * scale;
		}
	}
}

VramCanvas::~VramCanvas()
{
	while (!textures.empty())
	{
		sf::Texture* texture = textures.back();
		textures.pop_back();
		delete texture;
	}
}

// OnUpdate draws one tile from the VRAM at a time
void VramCanvas::OnUpdate()
{
    uint8_t pixels[8 * 8 * 4];
	std::array<uint8_t, 0x2000>* vram = bus->GetVRAM();

    for(int i = 0; i < 8; i++)
    {
		uint8_t upper = vram->at(tileIndex * 16 + i * 2);
		uint8_t lower = vram->at(tileIndex * 16 + i * 2 + 1);

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
				color = 0xFFA9A9A9;
            }
            else if(!upperBit && lowerBit)
            {
                //Color 2
				color = 0xFF545454;
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

	textures[tileIndex]->update(pixels);

	tileIndex++;
	if (tileIndex > 255)
	{
		tileIndex = 0;
	}

    clear(sf::Color::Black);
    
	for (auto s : sprites)
	{
		draw(s);
	}
}

VramFrame::VramFrame(std::shared_ptr<Bus> b, wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "VRAM Viewer", wxDefaultPosition, wxSize(400,400))
{
    canvas = new VramCanvas(b, this);
	canvas->SetSize(canvas->GetBestSize());
	Fit();
}

VramFrame::~VramFrame()
{

}
