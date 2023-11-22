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
    fill(_debounce.begin(), _debounce.end(), false);
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
