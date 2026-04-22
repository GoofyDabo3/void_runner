#include "scene_selector.hpp"

void DrawTextThick(const char* text, int x, int y, int size, Color color)
{
    DrawText(text, x+1, y,   size, color);
    DrawText(text, x-1, y,   size, color);
    DrawText(text, x,   y+1, size, color);
    DrawText(text, x,   y-1, size, color);
    DrawText(text, x,   y,   size, color); // main on top
}

Button::Button()
{
    current_state = 0;
    scale = 1.0f;
    rotation = 0.0f;
    origin = {0.0f, 0.0f};
}

void Button::load(const char* normal, const char* hover, const char* click, float x, float y, float btn_scale)
{
    initAnimation(states[0], normal);
    initAnimation(states[1], hover);
    initAnimation(states[2], click);
    scale = btn_scale;

    position = { x, y };

    dest_rec = {
        position.x,
        position.y,
        states[0].sourceRec.width * scale,
        states[0].sourceRec.height * scale
    };
}

bool Button::update()
{
    bool clicked = false;
    if(CheckCollisionPointRec(GetMousePosition(), dest_rec))
    {
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            clicked = true;
            current_state = 1; // back to hover after click
        }
        else if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            current_state = 2;
        }
        else
        {
            current_state = 1;
        }
    }
    else
    {
        current_state = 0;
    }
    return clicked;
}

void Button::draw()
{
    // DrawRectangleLinesEx(dest_rec, 2, GREEN);
    DrawTexturePro(states[current_state].texture, states[current_state].sourceRec, dest_rec, origin, rotation, WHITE);
}

Button::~Button()
{
    UnloadTexture(states[0].texture);
    UnloadTexture(states[1].texture);
    UnloadTexture(states[2].texture);
}

Scene_selector::Scene_selector()
{
    bgm = LoadMusicStream("resources/audio/bgm.ogg");
    SetMusicVolume(bgm, 0.3f);   // muffled default
    SetMusicPitch(bgm, 0.85f);   // slightly lower pitch = muffled feel
    PlayMusicStream(bgm);

    sfx_hit      = LoadSound("resources/audio/hit.mp3");
    sfx_aid      = LoadSound("resources/audio/aid.mp3");
    sfx_lose     = LoadSound("resources/audio/lose.mp3");
    sfx_win      = LoadSound("resources/audio/win.mp3");
    sfx_btn      = LoadSound("resources/audio/btn_back.mp3");
    sfx_btn_back = LoadSound("resources/audio/btn.mp3");
    gamescene.set_sfx(&sfx_hit, &sfx_aid, &sfx_lose);

    previous_scene = MAIN_MENU;
    current_scene = MAIN_MENU;
    quit = false;

    // 1. Define your dimensions
    float screen_width = static_cast<float>(GetScreenWidth()); 
    float gap = 25.0f;
    float btn_scale = 8.0f;
    float btn_w = 12.0f * btn_scale;
    float preview_scale = 10.0f; // bigger ship
    float preview_w = 16.0f * preview_scale;
    float preview_h = 16.0f * preview_scale;
    float skin_y = 600.0f;
    float arrow_w = 12.0f * btn_scale;
    float arrow_gap = 0.0f;

    float total_ui_width = (btn_w * 2.0f) + gap;
    float start_x = (screen_width / 2.0f) - (total_ui_width / 2.0f);

    btn_play.load("resources/ui/play_normal.png", "resources/ui/play_hover.png", "resources/ui/play_click.png", start_x, 450, btn_scale);
    btn_quit.load("resources/ui/quit_normal.png", "resources/ui/quit_hover.png", "resources/ui/quit_click.png", start_x + btn_w + gap, 450, btn_scale);

    btn_skin_left.load("resources/ui/left_normal.png", "resources/ui/left_hover.png", "resources/ui/left_click.png", screen_width/2.0f - preview_w/2.0f - arrow_w - arrow_gap, skin_y, btn_scale);
    btn_skin_right.load("resources/ui/right_normal.png", "resources/ui/right_hover.png", "resources/ui/right_click.png", screen_width/2.0f + preview_w/2.0f + arrow_gap, skin_y, btn_scale);

    btn_resume.load("resources/ui/play_normal.png", "resources/ui/play_hover.png", "resources/ui/play_click.png", start_x, 450, btn_scale);
    btn_menu.load("resources/ui/menu_normal.png", "resources/ui/menu_hover.png", "resources/ui/menu_click.png", start_x + btn_w + gap, 450, btn_scale);

    btn_restart.load("resources/ui/play_normal.png", "resources/ui/play_hover.png", "resources/ui/play_click.png", start_x, 450, btn_scale);
    btn_menu2.load("resources/ui/menu_normal.png", "resources/ui/menu_hover.png", "resources/ui/menu_click.png", start_x + btn_w + gap, 450, btn_scale);
}

void Scene_selector::update_scenes()
{
    UpdateMusicStream(bgm);

    switch (current_scene)
    {
        case MAIN_MENU:
        {
            SetMusicVolume(bgm, 0.3f);
            SetMusicPitch(bgm, 0.85f);
            background.draw();
            DrawTextThick("VOID RUNNER", 540/2 - MeasureText("VOID RUNNER", 55)/2, 150, 55, WHITE);            
            if(btn_play.update()) 
            { 
                PlaySound(sfx_btn); 
                gamescene.reset(); 
                current_scene = GAME; 
            }
            if(btn_quit.update() || IsKeyPressed(KEY_ESCAPE)) 
            { 
                PlaySound(sfx_btn_back); 
                quit = true; 
            }

            textureAnimation& current_anim = gamescene.get_player_anim();
            float preview_scale = 10.0f;
            float preview_w = current_anim.sourceRec.width * preview_scale;
            float preview_h = current_anim.sourceRec.height * preview_scale;
            float skin_y = 600.0f;
            DrawTexturePro(current_anim.texture, current_anim.sourceRec,
                {540.0f/2.0f - preview_w/2.0f, skin_y, preview_w, preview_h},
                {0,0}, 0, WHITE);

            if(btn_skin_left.update()){
                PlaySound(sfx_btn); 
                gamescene.switch_player_skin(-1);
            }
            if(btn_skin_right.update()){
                PlaySound(sfx_btn); 
                gamescene.switch_player_skin(1);
            }
            btn_play.draw();
            btn_quit.draw();

            btn_skin_left.draw();
            btn_skin_right.draw();
            break;
        }
        case GAME:
            SetMusicVolume(bgm, 0.7f);
            SetMusicPitch(bgm, 1.0f);
            if(gamescene.is_over())
            {
                if(gamescene.is_won())
                {
                    PlaySound(sfx_win);
                    current_scene = WIN;
                }
                else
                {
                    PlaySound(sfx_lose);
                    current_scene = OVER;
                }
            }
            background.update();
            gamescene.update();
            background.draw();
            gamescene.draw();
            
            if(IsKeyPressed(KEY_ESCAPE))
            {
                PlaySound(sfx_btn_back); 
                current_scene = PAUSE;
            }
            break;

        case PAUSE:
            SetMusicVolume(bgm, 0.3f);
            SetMusicPitch(bgm, 0.85f);
            background.draw();
            DrawTextThick("PAUSED", 540/2 - MeasureText("PAUSED", 55)/2, 150, 55, WHITE);
            if(btn_resume.update()) 
            {
                PlaySound(sfx_btn); 
                current_scene = GAME; 
            }
            if(btn_menu.update()) 
            {
                PlaySound(sfx_btn_back); 
                current_scene = MAIN_MENU; 
            }
            btn_resume.draw();
            btn_menu.draw();
            break;

        case OVER:
            SetMusicVolume(bgm, 0.3f);
            SetMusicPitch(bgm, 0.85f);
            background.draw();
            DrawTextThick("GAME OVER", 540/2 - MeasureText("GAME OVER", 55)/2, 150, 55, RED);
            if(btn_restart.update()) 
            {
                PlaySound(sfx_btn); 
                gamescene.reset(); 
                current_scene = GAME; 
            }
            if(btn_menu2.update()) 
            {
                PlaySound(sfx_btn_back); 
                current_scene = MAIN_MENU; 
            }
            btn_restart.draw();
            btn_menu2.draw();
            break;

        case WIN:
            SetMusicVolume(bgm, 0.3f);
            SetMusicPitch(bgm, 0.85f);
            background.draw();
            DrawTextThick("YOU WIN!", 540/2 - MeasureText("YOU WIN!", 55)/2, 150, 55, YELLOW);
            if(btn_restart.update())
            {
                PlaySound(sfx_btn); 
                gamescene.reset();
                current_scene = GAME;
            }
            if(btn_menu2.update())
            {
                PlaySound(sfx_btn_back); 
                current_scene = MAIN_MENU;
            }
            btn_restart.draw();
            btn_menu2.draw();
            break;

        default:
        
            break;
    }
    previous_scene = current_scene;
}

Scene_selector::~Scene_selector()
{
    StopMusicStream(bgm);
    UnloadMusicStream(bgm);
    UnloadSound(sfx_hit);
    UnloadSound(sfx_aid);
    UnloadSound(sfx_lose);
    UnloadSound(sfx_win);
    UnloadSound(sfx_btn);
    UnloadSound(sfx_btn_back);
}