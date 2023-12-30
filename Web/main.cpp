#include <iostream>
#include <vector>
#include <raylib.h>
#include "minigames/piano.h"

#define NUM_FRAMES_PER_LINE 6
#define NUM_LINES 1

using namespace std;

int main()
{
    Color darkGreen = Color{20, 160, 133, 255};

    const float screenWidth = 800;
    const float screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Boop Quest");

    SetTargetFPS(60);

    InitAudioDevice();

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

    InitPiano(sounds, {10, screenHeight * 0.25, screenWidth - 20, (screenHeight * 0.75) - 10});

    // SPRITE TESTING
    // float frameWidth = (float)(characterWaiting.width / NUM_FRAMES_PER_LINE);
    // float frameHeight = (float)(characterWaiting.height / NUM_LINES);
    // int currentFrame = 0;
    // int currentLine = 0;
    // Rectangle frameRec = {0, 0, frameWidth, frameHeight};
    // float frameScaling = 2.5f;
    // Rectangle scaledRect = {0, 0, frameWidth * frameScaling, frameHeight * frameScaling};
    // Vector2 frameOrigin = {screenWidth / -2.0f + scaledRect.width / 2.0f, -scaledRect.height / (frameScaling * frameScaling)};
    // int framesCounter = 0;
    // //

    while (WindowShouldClose() == false)
    {
        // SPRITE TESTING
        // framesCounter++;
        // if (framesCounter > 5)
        // {
        //     currentFrame++;
        //     if (currentFrame >= NUM_FRAMES_PER_LINE)
        //     {
        //         currentFrame = 0;
        //         currentLine++;
        //         if (currentLine >= NUM_LINES)
        //         {
        //             currentLine = 0;
        //         }
        //     }
        //     framesCounter = 0;
        // }
        // frameRec.x = frameWidth * currentFrame;
        // frameRec.y = frameHeight * currentLine;

        Vector2 position = GetMousePosition();
        if (GetTouchPointCount() > 0)
        {
            position = GetTouchPosition(0);
        }

        BeginDrawing();
        ClearBackground(darkGreen);

        DrawPiano(position);
        //DrawTexturePro(characterWaiting, frameRec, scaledRect, frameOrigin, 0, WHITE);

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
