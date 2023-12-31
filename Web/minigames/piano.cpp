#include <iostream>
#include "piano.h"

using namespace std;

vector<Sound> _sounds;
int _numKeys = -1;
vector<bool> _debounce;
Rectangle _keyRect;
Color _whiteKeyInactive = Color{160, 160, 160, 255};
Color _blackKeyInactive = Color{80, 80, 80, 255};
float _colWidth;

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

vector<Texture2D> _textures;
Texture2D _texture;
float _frameWidth = 0.0f;
float _frameHeight = 64.0f;
float _frameScaling = 2.5f;
int _currentFrame = 0;
int _framesCounter = 0;
int _numFrames = 0;
Rectangle _frameRec;
Rectangle _scaledRect;
Vector2 _frameOrigin;

enum Sprites
{
    Climb,
    Damage,
    Death,
    Fall,
    Idle,
    Jump,
    Land,
    PickupGround,
    PickupWall,
    Punch,
    Run,
    Talk,
    Use,
    Wait,
    Walk
};

int _frameCounts[15] = {10, 2, 8, 2, 8, 2, 2, 8, 10, 6, 8, 7, 13, 6, 8};

enum Directions
{
    None,
    Right,
    Left
};

Directions _direction = Directions::Right;

void SetSprite(int n, Directions direction = Directions::None)
{
    _texture = _textures[n];
    _numFrames = _frameCounts[n];
    _frameWidth = (float)(_texture.width / _numFrames);
    _currentFrame = 0;
    _scaledRect = {_scaledRect.x,
                   0,
                   _frameWidth * _frameScaling,
                   _frameHeight * _frameScaling};
    _frameOrigin = {GetScreenWidth() / -2.0f + _scaledRect.width / 2.0f, 0};

    if (direction != Directions::None)
    {
        _direction = direction;
    }

    _frameRec = {0, 0, _frameWidth, _frameHeight};
    if (_direction == Directions::Left)
    {
        _frameRec.width *= -1;
    }
}

void DrawSprite()
{
    bool active = false;
    bool up = IsKeyPressed(KEY_W);
    bool leftPressed = IsKeyPressed(KEY_A);
    bool leftDown = IsKeyDown(KEY_A);
    bool down = IsKeyDown(KEY_S);
    bool rightPressed = IsKeyPressed(KEY_D);
    bool rightDown = IsKeyDown(KEY_D);

    cout << _scaledRect.x << '\n' << flush;

    if (up)
    {
        SetSprite(Sprites::Jump);
    }
    if (leftPressed)
    {
        SetSprite(Sprites::Walk, Directions::Left);
    }
    if (leftDown)
    {
        if (_scaledRect.x > GetScreenWidth() / -2.0f + 50)
        {
            active = true;
            _scaledRect.x -= 2;
        }
    }
    if (down)
    {
    }
    if (rightPressed)
    {
        SetSprite(Sprites::Walk, Directions::Right);
    }
    if (rightDown)
    {
        if (_scaledRect.x < GetScreenWidth() / 2.0f - 50)
        {
            active = true;
            _scaledRect.x += 2;
        }
    }

    _framesCounter++;
    if (_framesCounter > 5)
    {
        _currentFrame++;
        if (_currentFrame >= _numFrames)
        {
            _currentFrame = 0;
            if (!active)
            {
                SetSprite(Sprites::Wait);
            }
        }
        _framesCounter = 0;
    }

    _frameRec.x = _frameWidth * _currentFrame;

    DrawTexturePro(_texture,
                   _frameRec,
                   _scaledRect,
                   _frameOrigin,
                   0,
                   WHITE);
}

void InitPiano(vector<Sound> sounds,
               vector<Texture2D> textures,
               Rectangle bounds)
{
    _sounds = sounds;
    _numKeys = _sounds.size();
    _debounce.resize(_numKeys);
    fill(_debounce.begin(), _debounce.end(), false);
    _keyRect = {bounds.x,
                bounds.y,
                bounds.width - 5,
                bounds.height};
    _colWidth = _keyRect.width / (float)_numKeys;
    _textures = textures;
    SetSprite(Sprites::Wait);
}

void DrawPiano(Vector2 position)
{
    int col = 0;
    bool leftClick = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    if (!leftClick)
    {
        fill(_debounce.begin(), _debounce.end(), false);
    }

    for (float i = _keyRect.x; i < _keyRect.width; i += _colWidth)
    {
        Rectangle keyInactive = {i,
                                 _keyRect.y,
                                 _colWidth,
                                 _keyRect.height};
        Rectangle keyPressed = {i,
                                _keyRect.y + 10,
                                _colWidth,
                                _keyRect.height - 20};

        if (CheckCollisionPointRec(position, keyInactive))
        {
            if (leftClick && !_debounce[col])
            {
                PlaySound(_sounds[col]);
                SetSprite(col);
                DrawRectangleRec(keyPressed,
                                 col++ % 2 == 0 ? BLACK : WHITE);
            }
            else
            {
                DrawRectangleRec(keyInactive,
                                 col++ % 2 == 0 ? BLACK : WHITE);
            }
        }
        else
        {
            _debounce[col] = false;
            DrawRectangleRec(keyInactive,
                             col++ % 2 == 0 ? _blackKeyInactive : _whiteKeyInactive);
        }
    }
}
