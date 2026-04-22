#include <raylib.h>
#include "scene_selector.hpp"

const int width = 540;
const int height = 960;

int main()
{
    InitWindow(width, height, "VOID RUNNER");
    InitAudioDevice();
    SetExitKey(0);
    SetTargetFPS(60);

    Scene_selector scene_selector;
    
    while(!scene_selector.quit && !WindowShouldClose())
    {
        BeginDrawing();
        
        ClearBackground(BLACK);

        scene_selector.update_scenes();
        
        EndDrawing();
    }

    CloseAudioDevice();

    return 0;
}