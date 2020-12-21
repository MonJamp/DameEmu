#include "VramFrame.h"


VramCanvas::VramCanvas(wxWindow* parent) : SfmlCanvas(parent)
{

}

VramCanvas::~VramCanvas()
{

}

void VramCanvas::OnUpdate()
{
    uint8_t vram[] = {
        0x00, 0x00, 0x18, 0x18, 0x38, 0x38, 0x18, 0x18,
        0x18, 0x18, 0x18, 0x18, 0x3C, 0x3C, 0x00, 0x00};
    
    uint8_t pixel[8 * 8 * 4];


    for(int k = 0; k < sizeof(vram); k++)
    {
        for(int i = 0; i < 8; i++)
        {
            uint8_t upper = vram[k * 16 + i * 2];
            uint8_t lower = vram[k * 16 + i * 2 + 1];

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
                
                /*
                pixel[i * 32 + j * 4] = color & 0xFF000000;
                pixel[i * 32 + j * 4 + 1] = color & 0xFF0000;
                pixel[i * 32 + j * 4 + 2] = color & 0xFF00;
                pixel[i * 32 + j * 4 + 3] = color & 0xFF;
                */

                pixel[i * 32 + j * 4] = 255;
                pixel[i * 32 + j * 4 + 1] = 255;
                pixel[i * 32 + j * 4 + 2] = 255;
                pixel[i * 32 + j * 4 + 3] = 255;

                upper <<= 1;
                lower <<= 1;
            }
        }
    }

    std::array<uint8_t, 8*8*4> pixel2;
    std::fill(pixel2.begin(), pixel2.end(), 0xFF);

    sf::Texture texture;
    texture.create(8, 8);
    texture.update(pixel);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(5.f, 5.f);
    sprite.setPosition(20.f, 20.f);

    clear(sf::Color::Black);
    draw(sprite);
}

VramFrame::VramFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, "VRAM Viewer", wxDefaultPosition, wxSize(400,400))
{
    canvas = new VramCanvas(this);
    canvas->PostSizeEvent();
}

VramFrame::~VramFrame()
{

}
