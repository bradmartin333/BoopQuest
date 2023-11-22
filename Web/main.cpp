#include <iostream>
#include <raylib.h>
#include "minigames/piano.h"

using namespace std;

int main()
{

    Color Dark_Green = Color{20, 160, 133, 255};

    const int screenWidth = 800;
    const int screenHeight = 600;
    int ball_x = 100;
    int ball_y = 100;
    int ball_speed_x = 5;
    int ball_speed_y = 5;
    int ball_radius = 15;

    InitWindow(screenWidth, screenHeight, "Boop Quest");
    SetTargetFPS(60);

    InitAudioDevice();

    Music music = LoadMusicStream("res/sounds/western.mp3");
    PlayMusicStream(music);

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(music);

        BeginDrawing();
        ClearBackground(Dark_Green);
        ball_x += ball_speed_x;
        ball_y += ball_speed_y;

        if (ball_x + ball_radius >= screenWidth || ball_x - ball_radius <= 0)
        {
            ball_speed_x *= -1;
        }

        if (ball_y + ball_radius >= screenHeight || ball_y - ball_radius <= 0)
        {
            ball_speed_y *= -1;
        }

        DrawCircle(ball_x, ball_y, ball_radius, WHITE);
        test(screenWidth / 2, screenHeight / 2);
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
