#include "entity.hpp"
    
Entity::Entity()
{

}

void Entity::initAnimation(textureAnimation& anim, const char* file)
{
    anim.texture = LoadTexture(file);
    anim.sourceRec = {0.0f, 0.0f, static_cast<float>(anim.texture.width), static_cast<float>(anim.texture.height)};
}

Rectangle Entity::get_collision_rec()
{
    return {
        dest_rec.x + hitbox_shrink,
        dest_rec.y + hitbox_shrink,
        dest_rec.width - hitbox_shrink * 2,
        dest_rec.height - hitbox_shrink
    };
}

void Entity::toggle_ground()
{
    position.y = GROUND;
}

Entity::~Entity()
{
    
}