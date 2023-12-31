#include <iostream>
#include <vector>
#include <raylib.h>
#include "minigames/piano.h"

using namespace std;

int main()
{
    Color darkGreen = Color{20, 160, 133, 255};

    const float screenWidth = 800;
    const float screenHeight = 600;

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

    vector<Texture2D> textures = {
        LoadTexture("res/character/climb_10.png"),
        LoadTexture("res/character/damage_2.png"),
        LoadTexture("res/character/death_8.png"),
        LoadTexture("res/character/fall_2.png"),
        LoadTexture("res/character/idle_8.png"),
        LoadTexture("res/character/jump_2.png"),
        LoadTexture("res/character/land_2.png"),
        LoadTexture("res/character/pickup_ground_8.png"),
        LoadTexture("res/character/pickup_wall_10.png"),
        LoadTexture("res/character/punch_6.png"),
        LoadTexture("res/character/run_8.png"),
        LoadTexture("res/character/talk_7.png"),
        LoadTexture("res/character/use_13.png"),
        LoadTexture("res/character/wait_6.png"),
        LoadTexture("res/character/walk_8.png")};

    InitPiano(sounds, textures, {10, screenHeight * 0.25f, screenWidth - 20, (screenHeight * 0.75f) - 10});

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
        DrawSprite();

        EndDrawing();
    }

    for (int i = sounds.size() - 1; i >= 0; i--)
    {
        UnloadSound(sounds[i]);
        sounds.pop_back();
    }

    for (int i = textures.size() - 1; i >= 0; i--)
    {
        UnloadTexture(textures[i]);
        textures.pop_back();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
