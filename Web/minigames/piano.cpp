#include "piano.h"

using namespace std;

int _numKeys = -1;
vector<Sound> _sounds;
vector<bool> _debounce;
Rectangle _bounds;
Color _whiteKeyInactive = Color{160, 160, 160, 255};
Color _blackKeyInactive = Color{80, 80, 80, 255};

void InitPiano(vector<Sound> sounds, Rectangle bounds)
{
    _bounds = bounds;
    _sounds = sounds;
    _numKeys = _sounds.size();
    _debounce.resize(_numKeys);
    std::fill(_debounce.begin(), _debounce.end(), false);
}

void DrawPiano(vector<Vector2> positions)
{
    if (_numKeys == -1)
    {
        _numKeys = 10;
    }

    Rectangle r = {_bounds.x, _bounds.y, _bounds.width - 5, _bounds.height};
    float colWid = r.width / (float)_numKeys;
    int col = 0;
    vector<Sound> soundsToPlay;

    for (float i = r.x; i < r.width; i += colWid)
    {
        Rectangle keyInactive = {i, r.y, colWid, r.height};
        Rectangle keyPressed = {i, r.y + 10, colWid, r.height - 20};

        for (int i = 0; i < positions.size(); i++)
        {
            Vector2 p = positions[i];
            if (CheckCollisionPointRec(p, keyInactive))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !_debounce[col])
                {
                    _debounce[col] = true;
                    if (!IsSoundPlaying(_sounds[col]))
                        soundsToPlay.push_back(_sounds[col]);
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

    for (int i = 0; i < soundsToPlay.size(); i++)
    {
        PlaySound(soundsToPlay[i]);
    }
}
