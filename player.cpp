#include "player.hpp"

Player::Player()
{
    dead = false;
    hp = 100;
    skin = _RED;
    state = CENTER;
    speed = 8;
    rotation = 0.0f;
    scale = 8;
    hitbox_shrink = 5.0f;

    initAnimation(animation[_RED][CENTER],"resources/ships/red_center.png");
    initAnimation(animation[_RED][LEFT],"resources/ships/red_left.png");
    initAnimation(animation[_RED][RIGHT],"resources/ships/red_right.png");

    initAnimation(animation[_WHITE][CENTER],"resources/ships/white_center.png");
    initAnimation(animation[_WHITE][LEFT],"resources/ships/white_left.png");
    initAnimation(animation[_WHITE][RIGHT],"resources/ships/white_right.png");

    initAnimation(animation[_GREEN][CENTER],"resources/ships/green_center.png");
    initAnimation(animation[_GREEN][LEFT],"resources/ships/green_left.png");
    initAnimation(animation[_GREEN][RIGHT],"resources/ships/green_right.png");

    initAnimation(animation[_PURPLE][CENTER],"resources/ships/purple_center.png");
    initAnimation(animation[_PURPLE][LEFT],"resources/ships/purple_left.png");
    initAnimation(animation[_PURPLE][RIGHT],"resources/ships/purple_right.png");

    position = {
        static_cast<float>(GetScreenWidth() / 2 - animation[skin][state].sourceRec.width / 2),
        GROUND
    };

    origin = {
        0.0f,
        0.0f
    };

    dest_rec = {
        position.x,
        position.y,
        animation[skin][state].sourceRec.width * scale,
        animation[skin][state].sourceRec.height * scale
    };

    shield_anim.texture = LoadTexture("resources/misc/shield_anim.png");
    shield_anim.sourceRec = {   
        0.0f, 
        0.0f, 
        static_cast<float>(shield_anim.texture.width / 4), 
        static_cast<float>(shield_anim.texture.height) 
    };
    shielded = false;
    shield_fading = false;
    shield_timer = 0.0f;
    shield_frame = 0;
    shield_frame_counter = 0;

    flame_anim.texture = LoadTexture("resources/misc/rocket.png");
    flame_anim.sourceRec = {   
        0.0f, 
        0.0f, 
        static_cast<float>(flame_anim.texture.width / 4), 
        static_cast<float>(flame_anim.texture.height) 
    };
    flame_frame = 0;
    flame_frame_counter = 0;
}

bool Player::update()
{
    collision_rec = get_collision_rec();

    if(!dead)
    {
        if(IsKeyDown(KEY_RIGHT) && collision_rec.x < GetScreenWidth() - collision_rec.width)
        {
            state = RIGHT;
            position.x += speed;
        }
        else if(IsKeyDown(KEY_LEFT) && collision_rec.x > 0)
        {
            state = LEFT;
            position.x -= speed;
        }
        else   
        {
            state = CENTER;
        }
    }

    /* if(IsKeyPressed(KEY_X))
    {
        switch_skin();
    }

    if(IsKeyPressed(KEY_S))
    {
        shield();
    } */

    if(shielded)
    {
        shield_timer -= GetFrameTime();
        shield_frame_counter++;

        if(!shield_fading)
        {
            if(shield_frame_counter >= DELAYSPEED && shield_frame < 3)
            {
                shield_frame++;
                shield_anim.sourceRec.x = shield_frame * 16.0f;
                shield_frame_counter = 0;
            }

            if(shield_timer <= 0.0f)
            {
                shield_fading = true;
                shield_frame_counter = 0;
            }
        }
        else
        {
            if(shield_frame_counter >= DELAYSPEED)
            {
                if(shield_frame > 0)
                {
                    shield_frame--;
                    shield_anim.sourceRec.x = shield_frame * shield_anim.sourceRec.width;
                }
                else
                {
                    shielded = false;
                    shield_fading = false;
                }
                shield_frame_counter = 0;
            }
        }
    }

    flame_frame_counter++;
    if(flame_frame_counter >= DELAYSPEED)
    {
        flame_frame = (flame_frame + 1) % 4;
        flame_anim.sourceRec.x = flame_frame * 8.0f;
        flame_frame_counter = 0;
    }

    return dead;
}

void Player::draw()
{   
    dest_rec = {
        position.x,
        position.y,
        animation[skin][state].sourceRec.width * scale, 
        animation[skin][state].sourceRec.height * scale
    };

    Rectangle flame_dest = {
        dest_rec.x - dest_rec.width / 2 ,
        dest_rec.y + dest_rec.height / 3,
        flame_anim.sourceRec.width * scale,
        flame_anim.sourceRec.height * scale
    };
    DrawTexturePro(flame_anim.texture, flame_anim.sourceRec, flame_dest, {0.0f, 0.0f}, 0.0f, WHITE);
    

    // DrawRectangleLinesEx(get_collision_rec(), 2, RED);
    DrawTexturePro(animation[skin][state].texture, animation[skin][state].sourceRec, dest_rec, origin, rotation, WHITE);

    if(shielded)
    {
        shield_dest = {
            dest_rec.x + dest_rec.width / 2 - (shield_anim.sourceRec.width * scale) / 2,
            dest_rec.y + dest_rec.height / 2 - (shield_anim.sourceRec.height * scale) / 2,
            shield_anim.sourceRec.width * scale,
            shield_anim.sourceRec.height * scale
        };
        DrawTexturePro(shield_anim.texture, shield_anim.sourceRec, shield_dest, origin, rotation, WHITE);
    }

}

void Player::switch_skin(int dir)
{
    int next = (static_cast<int>(skin) + dir + skin_count) % skin_count;
    skin = static_cast<PlayerSkin>(next);
}

textureAnimation& Player::get_current_anim()
{
    return animation[skin][CENTER];
}

int Player::get_hp()
{
    return hp;
}

Rectangle Player::get_collision_rec()
{
    return {
        dest_rec.x - dest_rec.width / 2 + hitbox_shrink,
        dest_rec.y - dest_rec.height / 2 + hitbox_shrink,
        static_cast<float>(dest_rec.width - hitbox_shrink * 1.7),
        dest_rec.height - hitbox_shrink * 2
    };
}

void Player::hurt()
{
    if(shielded) return;
    hp -= 20;
    if(hp <= 0) die();
}

void Player::heal()
{
    hp += 20;
    if(hp > 100) hp = 100;
}

void Player::shield()
{
    if(!shielded)
    {
        shield_frame = 0;
        shield_frame_counter = 0;
        shield_anim.sourceRec.x = 0.0f;
        shield_fading = false;
    }
    else if(shield_fading)
    {
        shield_fading = false;
    }
    shield_timer = 4.0f;
    shielded = true;
}

float Player::get_shield_ratio()
{
    if(!shielded) return 0.0f;
    return shield_timer / 4.0f;  // 4.0f is max shield duration
}

bool Player::is_shielded()
{
    return shielded;
}

void Player::die()
{
    if(dead) return;

    dead = true;
    hp = 0;
}

void Player::reset()
{
    dead = false;
    state = CENTER;
    hp = 100;
    toggle_ground();

    dest_rec = {
        position.x,
        position.y,
        animation[skin][state].sourceRec.width * scale,
        animation[skin][state].sourceRec.height * scale
    };
    origin.x = dest_rec.width / 2;
    origin.y = dest_rec.height / 2;

    shielded = false;
    shield_fading = false;
    shield_timer = 0.0f;
    shield_frame = 0;
    shield_frame_counter = 0;
    shield_anim.sourceRec.x = 0.0f;

    flame_frame = 0;
    flame_frame_counter = 0;
    flame_anim.sourceRec.x = 0.0f;
}

Player::~Player()
{
    UnloadTexture(this->animation[_RED][CENTER].texture);
    UnloadTexture(this->animation[_RED][LEFT].texture);
    UnloadTexture(this->animation[_RED][RIGHT].texture);

    UnloadTexture(this->animation[_WHITE][CENTER].texture);
    UnloadTexture(this->animation[_WHITE][LEFT].texture);
    UnloadTexture(this->animation[_WHITE][RIGHT].texture);

    UnloadTexture(this->animation[_GREEN][CENTER].texture);
    UnloadTexture(this->animation[_GREEN][LEFT].texture);
    UnloadTexture(this->animation[_GREEN][RIGHT].texture);

    UnloadTexture(this->animation[_PURPLE][CENTER].texture);
    UnloadTexture(this->animation[_PURPLE][LEFT].texture);
    UnloadTexture(this->animation[_PURPLE][RIGHT].texture);

    UnloadTexture(shield_anim.texture);

    UnloadTexture(flame_anim.texture);
}