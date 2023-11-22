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

    std::vector<Sound> sounds = {
        LoadSound("res/sounds/getGood.wav"),
        LoadSound("res/sounds/getBad.wav"),
        LoadSound("res/sounds/spooky.wav"),
        LoadSound("res/sounds/getBoop.wav"),
        LoadSound("res/sounds/getLocation.wav"),
        LoadSound("res/sounds/funkyTown.wav"),
        LoadSound("res/sounds/beatGame.wav")};
    InitPiano(sounds);

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(darkGreen);
        DrawPiano({10, 10, screenWidth - 20, screenHeight * 0.75});
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
