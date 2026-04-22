#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <raylib.h>
#include "entity.hpp"

typedef struct{
    Texture2D texture;
    Rectangle sourceRec;
    Vector2 position;
}Layer;

class Background : public Entity{
    private:
        Layer layer[2];
        int screenheight;
        int layer_gap;
        int other_tile;
        void initLayer(int i, const char *file, Vector2 pos);
    public:
        Background();
        void update();
        void draw();
        ~Background();
};

#endif