#ifndef SCENE_SELECTOR_HPP
#define SCENE_SELECTOR_HPP

#include <raylib.h>
#include <vector>
#include "background.hpp"
#include "game_scene.hpp"

typedef enum Scene_enum{
    MAIN_MENU, 
    PAUSE, 
    GAME,
    OVER,
    WIN
}Scene_enum;

void DrawTextThick(const char* text, int x, int y, int size, Color color);

class Button : public Entity {
    private:
        textureAnimation states[3];  // 0=normal, 1=hover, 2=click
        int current_state;
    public:
        Button();
        void load(const char* normal, const char* hover, const char* click, float x_offset, float y, float btn_scale);
        bool update();
        void draw();
        ~Button();
};  

class Scene_selector{
    private:
        Music bgm;
        Sound sfx_hit;
        Sound sfx_aid;
        Sound sfx_lose;
        Sound sfx_win;
        Sound sfx_btn;
        Sound sfx_btn_back;
        Scene_enum current_scene;
        Scene_enum previous_scene;
        GameScene gamescene;
        Background background;
        // main menu
        Button btn_play;
        Button btn_skin_left;
        Button btn_skin_right;
        Button btn_quit;
        // pause
        Button btn_resume;
        Button btn_menu;
        // game over / win
        Button btn_restart;
        Button btn_menu2;
    public:
        bool quit;
        Scene_selector();
        void update_scenes();
        ~Scene_selector();
};

class Scene{
    private:
        std::vector<Rectangle> buttons;
        Image backgorund;
};

// bool DrawButton(Rectangle rect, const char* text);

#endif