#include <raylib.h>

#define MAXSPEED 30
#define MINSPEED 1
#define MAXRADIUS 150
#define MINRADIUS 20

class Ball{
    public:
        int x, y, r, speed;
        Color color;

        void draw(){
            DrawCircle(x, y, r, color);
        }

        void update(){
            if(IsKeyDown(KEY_RIGHT) && x < GetScreenWidth() - r) x+= speed;
            if(IsKeyDown(KEY_LEFT) && x > r) x-= speed;
            if(IsKeyDown(KEY_DOWN) && y < GetScreenHeight() - r) y+= speed;
            if(IsKeyDown(KEY_UP) && y > r) y-= speed;

            if(IsKeyPressed(KEY_I) && speed < MAXSPEED) speed++;
            if(IsKeyPressed(KEY_K) && speed > MINSPEED) speed--;
            if(IsKeyDown(KEY_O) && r < MAXRADIUS) r++;
            if(IsKeyDown(KEY_L) && r > MINRADIUS) r--;
        }
};

int main(void)
{
    const int width = 1280, height = 720;

    Ball ball;
    ball.speed = 7;
    ball.x = width / 2; ball.y = height / 2;
    ball.r = 70;
    ball.color = RED;

    InitWindow(width, height, "CACHETTE");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);

        ball.draw();
        ball.update();
        DrawText(TextFormat("SPEED: %d RADIUS: %d", ball.speed, ball.r), 10, 10, 25, BLACK);

        EndDrawing();
    }

    return 0;
}