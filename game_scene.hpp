#ifndef GAME_SCENE
#define GAME_SCENE

#include <raylib.h>
#include "player.hpp"
#include "enemies.hpp"
#include "background.hpp"

class GameScene{
    private:
        Player player;
        Rectangle player_collision_rec;
        Enemy enemies[7];
        textureAnimation hp_icon;
        textureAnimation shield_icon;
        Sound* sfx_hit;
        Sound* sfx_aid;
        Sound* sfx_lose;
        float elapsed_time;
        float time_limit;
        float spawn_interval;
        float spawn_interval_min;
        float spawn_timer;
        int aid_delay;
        int aid_delay_counter;
        int spawn_index;
        int enemies_size;
        bool game_over;
    public:
        GameScene();
        void update();
        void draw();
        bool is_over();
        bool is_won();
        void set_sfx(Sound* hit, Sound* aid, Sound* lose);
        textureAnimation& get_player_anim();
        void switch_player_skin(int dir);
        void reset();
        ~GameScene();
};



#endif