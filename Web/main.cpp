#include <iostream>
#include <vector>
#include <raylib.h>
#include "minigames/piano.h"

using namespace std;

#define MAX_TOUCH_POINTS 10

int main()
{
    Color darkGreen = Color{20, 160, 133, 255};

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Boop Quest");

    vector<Vector2> positions;

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
    enum SFX {
        Good, Bad, Spooky, Boop, Location, FunkyTown, BeatGame
    };
    InitPiano(sounds, {10, screenHeight * 0.25, screenWidth - 20, (screenHeight * 0.75) - 10});

    while (WindowShouldClose() == false)
    {
        // Get mouse and all touch positions
        positions.clear();
        positions.push_back(GetMousePosition());
        int tCount = GetTouchPointCount();
        if(tCount > MAX_TOUCH_POINTS) tCount = MAX_TOUCH_POINTS;
        for (int i = 0; i < tCount; ++i) positions.push_back(GetTouchPosition(i));

        BeginDrawing();
        ClearBackground(darkGreen);

        DrawPiano(positions);

        EndDrawing();
    }

    for (int i = 0; i < sounds.size(); i++)
    {
        UnloadSound(sounds[i]);
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
