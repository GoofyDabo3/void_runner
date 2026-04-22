#include <raylib.h>
#include "background.hpp"


void Background::initLayer(int i, const char *file, Vector2 pos)
{
    layer[i].texture = LoadTexture(file);
    layer[i].sourceRec = {0.0f, 0.0f, static_cast<float>(layer[i].texture.width), static_cast<float>(layer[i].texture.height)};
    layer[i].position = pos;
}

Background::Background()
{
    speed = 5;
    scale = 4.2f;
    screenheight = 960;
    layer_gap = 4;
    origin = {0.0f, 0.0f};
    rotation = 0.0f;

    initLayer(0, "resources/backgrounds/background_1.png", {0, 0});
    initLayer(1, "resources/backgrounds/background_1.png", {0, -(layer[0].texture.height * scale) + layer_gap});
}

void Background::update()
{
    for(int i = 0; i < 2; i++)
    {
        layer[i].position.y += speed;
        if(layer[i].position.y >= screenheight)
        {
            other_tile = 1 - i;
            layer[i].position.y = layer[other_tile].position.y - (layer[other_tile].texture.height * scale) + layer_gap;
        }
    }
}
   

void Background::draw()
{
    
    for(int i = 0; i < 2; i++)
    {
        dest_rec = {
            layer[i].position.x,
            layer[i].position.y,
            layer[i].sourceRec.width * scale, 
            layer[i].sourceRec.height * scale
        };
        DrawTexturePro(layer[i].texture, layer[i].sourceRec, dest_rec, origin, rotation, WHITE);
    }
}

Background::~Background()
{
    for(int i = 0; i < 2; i++)
    {
        UnloadTexture(layer[i].texture);
    }
}