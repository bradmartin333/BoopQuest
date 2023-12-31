#include "piano.h"

using namespace std;

int _numKeys = -1;
vector<Sound> _sounds;

vector<bool> _debounce;
Rectangle _bounds;
Color _whiteKeyInactive = Color{160, 160, 160, 255};
Color _blackKeyInactive = Color{80, 80, 80, 255};

vector<Texture2D> _textures;
Texture2D _texture;
float _frameWidth = 0.0f;
float _frameHeight = 64.0f;
int _currentFrame = 0;
float _frameScaling = 2.5f;
Rectangle _frameRec = {0, 0, _frameWidth, _frameHeight};
Rectangle _scaledRect = {0, 0, _frameWidth *_frameScaling, _frameHeight *_frameScaling};
Vector2 _frameOrigin = {800.0f / -2.0f + _scaledRect.width / 2.0f, -_scaledRect.height / (_frameScaling * _frameScaling)};
int _framesCounter = 0;
int _numFrames = 0;

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

void SetSprite(int n)
{
    _texture = _textures[n];
    _numFrames = _frameCounts[n];
    _frameWidth = (float)(_texture.width / _numFrames);
    _currentFrame = 0;
    _frameRec = {0, 0, _frameWidth, _frameHeight};
    _scaledRect = {0, 0, _frameWidth * _frameScaling, _frameHeight * _frameScaling};
    _frameOrigin = {800.0f / -2.0f + _scaledRect.width / 2.0f, 0};
}

void DrawSprite()
{
    _framesCounter++;
    if (_framesCounter > 5)
    {
        _currentFrame++;
        if (_currentFrame >= _numFrames)
        {
            _currentFrame = 0;
            SetSprite(Sprites::Wait);
        }
        _framesCounter = 0;
    }
    _frameRec.x = _frameWidth * _currentFrame;
    DrawTexturePro(_texture, _frameRec, _scaledRect, _frameOrigin, 0, WHITE);
}

void InitPiano(vector<Sound> sounds, vector<Texture2D> textures, Rectangle bounds)
{
    _bounds = bounds;
    _sounds = sounds;
    _textures = textures;
    _numKeys = _sounds.size();
    _debounce.resize(_numKeys);
    fill(_debounce.begin(), _debounce.end(), false);
    SetSprite(Sprites::Wait);
}

void DrawPiano(Vector2 position)
{
    bool touched = GetTouchPointCount() > 0;
    bool leftClick = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    if (!touched && !leftClick)
    {
        fill(_debounce.begin(), _debounce.end(), false);
    }

    if (_numKeys == -1)
    {
        _numKeys = 10;
    }

    Rectangle r = {_bounds.x, _bounds.y, _bounds.width - 5, _bounds.height};
    float colWid = r.width / (float)_numKeys;
    int col = 0;

    for (float i = r.x; i < r.width; i += colWid)
    {
        Rectangle keyInactive = {i, r.y, colWid, r.height};
        Rectangle keyPressed = {i, r.y + 10, colWid, r.height - 20};

        if (CheckCollisionPointRec(position, keyInactive))
        {
            if ((touched || leftClick) && !_debounce[col])
            {
                if (IsSoundPlaying(_sounds[col]))
                {
                    _debounce[col] = false;
                }
                else
                {
                    PlaySound(_sounds[col]);
                    SetSprite(col);
                    _debounce[col] = true;
                }

                DrawRectangleRec(keyPressed, col++ % 2 == 0 ? BLACK : WHITE);
            }
            else
            {
                DrawRectangleRec(keyInactive, col++ % 2 == 0 ? BLACK : WHITE);
            }
        }
        else
        {
            _debounce[col] = false;
            DrawRectangleRec(keyInactive, col++ % 2 == 0 ? _blackKeyInactive : _whiteKeyInactive);
        }
    }
}
