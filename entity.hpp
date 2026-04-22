#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <raylib.h>
#include <raymath.h>

const int DELAYSPEED = 6;
const int GROUND = 860;


typedef struct{
    Texture2D texture;
    Rectangle sourceRec;
}textureAnimation;

class Entity{
    protected:
        Vector2 position;
        int speed;
        float rotation;
        float scale;
        Vector2 origin;
        Rectangle dest_rec;
        float hitbox_shrink;
    
    public:
        Entity();
        void initAnimation(textureAnimation& anim, const char* file);
        Rectangle get_collision_rec();
        void toggle_ground();
        ~Entity();
};

#endif