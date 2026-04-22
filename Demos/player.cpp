#include "player.hpp"


    
Player::Player()
{
    playerPosition = {
        static_cast<float>(GetScreenWidth() / 4),
        static_cast<float>(GetScreenHeight() - 300) 
    };
    delaycounter = 0;
    currentframe = 0;
    playerSpeed = 3;
    origin = {
        0.0f,
        0.0f
    };
    rotation = 0.0f;
    scale = 4;

    initAnimation(animation[RUN],"resources/Pink_Run.png", 6);
    initAnimation(animation[IDLE],"resources/Pink_Idle.png", 4);
    initAnimation(animation[WALK],"resources/Pink_Walk.png", 6);
}

void Player::initAnimation(textureAnimation& anim, const char* file, int frames)
{
    anim.texture = LoadTexture(file);
    anim.framecount = frames;
    anim.framewidth = anim.texture.width / anim.framecount;
    anim.frameheight = anim.texture.height;
    anim.sourceRec = {0.0f, 0.0f, anim.framewidth, anim.frameheight};
}

void Player::update()
{
    state = IDLE;
    if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT))
    {
        state = WALK;
        if(IsKeyDown(KEY_LEFT_SHIFT))
        {
            state = RUN;
        }
    }
    

    if(IsKeyDown(KEY_RIGHT)){
        animation[state].sourceRec.width = animation[state].framewidth;
        playerPosition.x+= playerSpeed;
        if(state == RUN)
        {
            playerPosition.x+= playerSpeed;
        }
        animation[IDLE].sourceRec.width = animation[IDLE].framewidth;
    }else if(IsKeyDown(KEY_LEFT)){
        animation[state].sourceRec.width = -animation[state].framewidth;
        playerPosition.x-= playerSpeed;
        if(state == RUN)
        {
            playerPosition.x-= playerSpeed;
        }

        animation[IDLE].sourceRec.width = -animation[IDLE].framewidth;
    }

    dest_rec = {
        playerPosition.x,
        playerPosition.y,
        animation[state].sourceRec.width * scale, 
        animation[state].sourceRec.height * scale
    };
}

void Player::animate()
{
    ++delaycounter;
    if(delaycounter >= DELAYSPEED)
    {
        delaycounter = 0;
        ++currentframe;
        currentframe%= animation[state].framecount;
        animation[state].sourceRec.x = animation[state].framewidth * currentframe;
    }

    DrawTexturePro(animation[state].texture, animation[state].sourceRec, dest_rec, origin, rotation, WHITE);
}

void Player::toggle_center()
{
    playerPosition = {
        static_cast<float>(GetScreenWidth() / 4),
        static_cast<float>(GetScreenHeight() - 300) 
    };
}

Player::~Player()
{
    UnloadTexture(this->animation[RUN].texture);
    UnloadTexture(this->animation[IDLE].texture);
    UnloadTexture(this->animation[WALK].texture);
}