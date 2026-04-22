#include "enemies.hpp"

Enemy::Enemy()
{
    speed = GetRandomValue(4, 6);
    origin = {
        0.0f,
        0.0f
    };
    rotation = 0.0f;
    scale = 8.0f;
    type = SMALL;
    speed = 7;
    hitbox_shrink = 8.0f;
    cooldown = 2.0f;
    attack_time = -cooldown;
    active = false;

    initAnimation(animation[SMALL],"resources/enemies/asteroid_small.png");
    initAnimation(animation[BIG],"resources/enemies/asteroid_big.png");
    initAnimation(animation[HEALTH],"resources/misc/health_potion.png");
    initAnimation(animation[SHIELD],"resources/misc/shield_potion.png");

    position = {
        static_cast<float>(GetScreenWidth()),
        0
    };

    dest_rec = {
        position.x,
        position.y - animation[type].sourceRec.height * scale,
        animation[type].sourceRec.width * scale, 
        animation[type].sourceRec.height * scale
    };
}

void Enemy::update()
{
    if(active)
    {
        position.y += speed;
        if(position.y - dest_rec.height >= GetScreenHeight())
        {
            active = false;
        }
    }
}

void Enemy::animate()
{
    dest_rec = {
        position.x,
        position.y - animation[type].sourceRec.height * scale,
        animation[type].sourceRec.width * scale, 
        animation[type].sourceRec.height * scale
    };

    // DrawRectangleLinesEx(get_collision_rec(), 2, RED);
    DrawTexturePro(animation[type].texture, animation[type].sourceRec, dest_rec, origin, rotation, WHITE);
}

Rectangle Enemy::get_collision_rec()
{
    return {
        dest_rec.x + hitbox_shrink,
        dest_rec.y + hitbox_shrink * 3,
        dest_rec.width - hitbox_shrink * 2,
        dest_rec.height - static_cast<float>(hitbox_shrink * 3.5)
    };
}

void Enemy::attack(Player &player)
{
    current_time = GetTime();
    if(current_time - attack_time >= cooldown)
    {
        attack_time = current_time;
        switch (type)
        {
        case HEALTH:
            player.heal();
            break;

        case SHIELD:
            player.shield();
            break;
        
        default:
            player.hurt();
            break;
        }
    }
}

void Enemy::reset(Type reset_type)
{

    if(reset_type == AID)
    {
        type = static_cast<EnemyType>(GetRandomValue(0, 1));
    }
    else if(reset_type == OBSTACLE)
    {
        type = static_cast<EnemyType>(GetRandomValue(2, 3));
    }
    position = {
        static_cast<float>(GetRandomValue(0, 500)),
        static_cast<float>(-70)
    };
    speed = GetRandomValue(6, 8);
    active = false;
}

bool Enemy::is_inactive()
{
    return !active;
}

void Enemy::activate()
{
    active = true;
}

Enemy::~Enemy()
{
    UnloadTexture(this->animation[SMALL].texture);
    UnloadTexture(this->animation[BIG].texture);
    UnloadTexture(this->animation[HEALTH].texture);
    UnloadTexture(this->animation[SHIELD].texture);
}