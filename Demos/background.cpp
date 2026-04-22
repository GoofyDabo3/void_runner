#include <raylib.h>
/* #include "player.hpp"
#include "scene_selector.hpp" */


const int width = 1920;
const int height = 1080;

typedef struct Layer
{
    Texture2D texture;
    Vector2 position;
    float speed;
}Layer;


class Background{
    private:
        Layer sky;
        Layer layer[7][2];
        int i = 0, j = 0;
        void initLayer(int i, int j, const char *file, Vector2 pos, float speed)
        {
            layer[i][j].texture = LoadTexture(file);
            layer[i][j].position = {pos.x, pos.y};
            layer[i][j].speed = speed;
        }
    public:
        Background()
        {
            sky.texture = LoadTexture("resources/background_3/layer_1.png");
            sky.position = {0, 0};
            sky.speed = 0;

            initLayer(0, 0, "resources/background_3/layer_2.png", {0, 0}, 0.5);
            initLayer(0, 1, "resources/background_3/layer_2.png", {1920, 0},0.5);

            initLayer(1, 0, "resources/background_3/layer_3.png", {0, 0}, 1);
            initLayer(1, 1, "resources/background_3/layer_3.png", {1920, 0}, 1);

            initLayer(2, 0, "resources/background_3/layer_4.png", {0, 0}, 1.5);
            initLayer(2, 1, "resources/background_3/layer_4.png", {1920, 0}, 1.5);

            initLayer(3, 0, "resources/background_3/layer_5.png", {0, 0}, 2);
            initLayer(3, 1, "resources/background_3/layer_5.png", {1920, 0}, 2);

            initLayer(4, 0, "resources/background_3/layer_6.png", {0, 0}, 2.5);
            initLayer(4, 1, "resources/background_3/layer_6.png", {1920, 0}, 2.5);

            initLayer(5, 0, "resources/background_3/layer_7.png", {0, 0}, 3);
            initLayer(5, 1, "resources/background_3/layer_7.png", {1920, 0}, 3);

            initLayer(6, 0, "resources/background_3/layer_8.png", {0, 0}, 3.5);
            initLayer(6, 1, "resources/background_3/layer_8.png", {1920, 0}, 3.5);
        }

        void animate()
        {

            DrawTextureV(sky.texture, sky.position, WHITE);
            for(int i = 0; i < 7; i++)
            {
                for(int j = 0; j < 2; j++)
                {
                    layer[i][j].position.x -= layer[i][j].speed;
                    if(layer[i][j].position.x <= -width) layer[i][j].position.x = width;
                    DrawTextureV(layer[i][j].texture, layer[i][j].position, WHITE);
                }
            }
        }

        ~Background()
        {
            for(int i = 0; i < 7; i++)
            {
                for(int j = 0; j < 2; j++)
                {
                    UnloadTexture(layer[i][j].texture);
                }
            }
        }
};

int main()
{
    
    SetTargetFPS(60);
    InitWindow(width, height, "pink monster animation");
    if(!IsWindowFullscreen())
    {
        ToggleFullscreen();
    }

    // Player player;
    Background background;
    
    while(!WindowShouldClose())
    {
        BeginDrawing();

        // scene_selector.select_scene();

        // player.update();

        ClearBackground(DARKGRAY);

        background.animate();
        // player.animate();
        
        EndDrawing();
    }

    return 0;
}