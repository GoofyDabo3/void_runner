#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <raylib.h>

const int DELAYSPEED = 7;

typedef enum State{IDLE, RUN, WALK} State;

typedef struct{
    Texture2D texture;
    int framecount;
    float framewidth;
    float frameheight;
    Rectangle sourceRec;
}textureAnimation;

class Player{
    private:
        textureAnimation animation[3];
        State state;
        Vector2 playerPosition;
        int delaycounter;
        int currentframe;
        int playerSpeed;
        Vector2 origin;
        float rotation;
        float scale;
        Rectangle dest_rec;
    
    public:
        Player();
        void initAnimation(textureAnimation& anim, const char* file, int frames);
        void update();
        void animate();
        void toggle_center();
        ~Player();
};

#endif