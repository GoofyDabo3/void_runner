#include "game_scene.hpp"

GameScene::GameScene()
{
    game_over = false;
    elapsed_time = 0.0f;
    time_limit = 60.0f;
    spawn_index = 0;
    enemies_size = 7;
    spawn_interval = 0.8f;
    spawn_interval_min = 0.2f;
    spawn_timer = 0.0f;
    aid_delay = 7;
    aid_delay_counter = 0;
    enemies[spawn_index].reset(OBSTACLE);
    enemies[spawn_index].activate();
    ++spawn_index;

    hp_icon.texture = LoadTexture("resources/misc/health_icon.png");
    hp_icon.sourceRec = {0, 0, static_cast<float>(hp_icon.texture.width), static_cast<float>(hp_icon.texture.height)};
    shield_icon.texture = LoadTexture("resources/misc/shield_icon.png");
    shield_icon.sourceRec = {0, 0, static_cast<float>(shield_icon.texture.width), static_cast<float>(shield_icon.texture.height)};
}

void GameScene::update()
{
    if(player.update())
    {
        game_over = true;
        return;
    }
    player_collision_rec = player.get_collision_rec();
    for(int i = 0; i < enemies_size; i++)
    {
        if(!enemies[i].is_inactive() && CheckCollisionRecs(player_collision_rec, enemies[i].get_collision_rec()))
        {
            EnemyType type = enemies[i].get_type();
            if(type == HEALTH || type == SHIELD) PlaySound(*sfx_aid);
            else if(!player.is_shielded()) PlaySound(*sfx_hit);

            enemies[i].attack(player);
            if(type == HEALTH || type == SHIELD)
            {
                enemies[i].reset(OBSTACLE);
            }
            else if(!player.is_shielded())
            {
                enemies[i].reset(OBSTACLE);
            }
        }
        enemies[i].update();
    }
    if(!game_over)
    {
        elapsed_time += GetFrameTime();
        if(elapsed_time >= time_limit)
        {
            game_over = true;
        }
    }
    spawn_timer += GetFrameTime();
    if(spawn_timer >= spawn_interval)
    {
        ++aid_delay_counter;
        aid_delay_counter %= aid_delay;

        if(enemies[spawn_index].is_inactive())
        {
            if(aid_delay_counter == 0)
            {
                enemies[spawn_index].reset(AID);
            }
            else
            {
                enemies[spawn_index].reset(OBSTACLE);
            }
            
            enemies[spawn_index].activate();
            ++spawn_index;
            spawn_index %= enemies_size;
        }
        spawn_timer = 0;

        spawn_interval -= elapsed_time / 30.0f;
        if(spawn_interval < spawn_interval_min)
            spawn_interval = spawn_interval_min;
    }
}

void GameScene::draw()
{
    for(int i = 0; i < enemies_size; i++)
    {
        if(!enemies[i].is_inactive())
            enemies[i].animate();
    }
    player.draw();

    float icon_scale = 3.0f;
    float icon_size = 8.0f * icon_scale;
    float bar_w = 160.0f;
    float bar_h = 18.0f;
    float hud_x = 20.0f;
    float hud_y = 20.0f;
    float row_gap = 30.0f;

    DrawTexturePro(hp_icon.texture, hp_icon.sourceRec,
        {hud_x, hud_y, icon_size, icon_size}, {0,0}, 0, WHITE);

    DrawRectangle(hud_x + icon_size + 6, hud_y + icon_size/2 - bar_h/2, bar_w, bar_h, {40, 10, 10, 200});

    float hp_ratio = player.get_hp() / 100.0f;
    Color hp_color = {
        255,
        static_cast<unsigned char>(80 * hp_ratio),
        0, 255
    };
    DrawRectangle(hud_x + icon_size + 6, hud_y + icon_size/2 - bar_h/2, bar_w * hp_ratio, bar_h, hp_color);
    
    DrawTexturePro(shield_icon.texture, shield_icon.sourceRec,
        {hud_x, hud_y + row_gap, icon_size, icon_size}, {0,0}, 0, WHITE);

    DrawRectangle(hud_x + icon_size + 6, hud_y + row_gap + icon_size/2 - bar_h/2, bar_w, bar_h, {10, 20, 40, 200});

    float shield_ratio = player.get_shield_ratio();
    DrawRectangle(hud_x + icon_size + 6, hud_y + row_gap + icon_size/2 - bar_h/2, bar_w * shield_ratio, bar_h, {0, 210, 255, 255});

    const char* timer_text = TextFormat("%.0f", time_limit - elapsed_time);
    int timer_size = 50;
    DrawText(timer_text, 540/2 - MeasureText(timer_text, timer_size)/2, 20, timer_size, WHITE);
}

bool GameScene::is_over()
{
    return game_over;
}

bool GameScene::is_won()
{
    return game_over && elapsed_time >= time_limit;
}

void GameScene::set_sfx(Sound* hit, Sound* aid, Sound* lose)
{
    sfx_hit = hit;
    sfx_aid = aid;
    sfx_lose = lose;
}

textureAnimation& GameScene::get_player_anim()
{
    return player.get_current_anim();
}

void GameScene::switch_player_skin(int dir)
{
    // dir: 1 = next, -1 = prev
    player.switch_skin(dir);
}

void GameScene::reset()
{
    game_over = false;
    
    elapsed_time = 0.0f;
    spawn_interval = 0.8f;
    spawn_timer = 0.0f;
    spawn_index = 0;
    aid_delay_counter = 0;

    for(int i = 0; i < enemies_size; i++)
    {
        enemies[i].reset(OBSTACLE);
    }

    enemies[spawn_index].reset(OBSTACLE);
    enemies[spawn_index].activate();
    ++spawn_index;
    player.reset();
}

GameScene::~GameScene()
{
    UnloadTexture(hp_icon.texture);
    UnloadTexture(shield_icon.texture);
}