#include <iostream>
#include <vector>
#include <raylib.h>
#include "minigames/piano.h"

using namespace std;

int main()
{
    Color darkGreen = Color{20, 160, 133, 255};

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Boop Quest");

    SetTargetFPS(60);

    InitAudioDevice();

    vector<Sound> sounds = {
        LoadSound("res/sounds/getGood.wav"),
        LoadSound("res/sounds/getBad.wav"),
        LoadSound("res/sounds/spooky.wav"),
        LoadSound("res/sounds/getBoop.wav"),
        LoadSound("res/sounds/getLocation.wav"),
        LoadSound("res/sounds/funkyTown.wav"),
        LoadSound("res/sounds/beatGame.wav")};
    enum SFX
    {
        Good,
        Bad,
        Spooky,
        Boop,
        Location,
        FunkyTown,
        BeatGame
    };
    InitPiano(sounds, {10, screenHeight * 0.25, screenWidth - 20, (screenHeight * 0.75) - 10});

    while (WindowShouldClose() == false)
    {
        Vector2 position = GetMousePosition();
        if (GetTouchPointCount() > 0)
        {
            position = GetTouchPosition(0);
        }

        BeginDrawing();
        ClearBackground(darkGreen);

        DrawPiano(position);

        EndDrawing();
    }

    for (int i = sounds.size() - 1; i >= 0; i--)
    {
        UnloadSound(sounds[i]);
        sounds.pop_back();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
