#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

const int skin_count = 4;

typedef enum{
    _RED,
    _WHITE,
    _GREEN,
    _PURPLE
}PlayerSkin;

typedef enum{
    CENTER,
    LEFT,
    RIGHT
}PlayerState;

class Player : public Entity{
    private:
        textureAnimation animation[4][3];
        PlayerSkin skin;
        PlayerState state;
        Rectangle collision_rec;
        bool dead;
        int hp;

        textureAnimation shield_anim;
        Rectangle shield_dest;
        bool shielded;
        bool shield_fading; 
        float shield_timer;
        int shield_frame;
        int shield_frame_counter;

        textureAnimation flame_anim;
        int flame_frame;
        int flame_frame_counter;
        
    public:
        Player();
        bool update();
        void draw();
        void switch_skin(int dir = 1);
        textureAnimation& get_current_anim();
        int get_hp();
        Rectangle get_collision_rec();
        void hurt();
        void heal();
        void shield();
        float get_shield_ratio();
        bool is_shielded();
        void die();
        void reset();
        ~Player();
};

#endif