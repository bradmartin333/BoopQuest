#include "piano.h"

int _numKeys = -1;
std::vector<Sound> _sounds;
Color _whiteKeyInactive = Color{160, 160, 160, 255};
Color _blackKeyInactive = Color{80, 80, 80, 255};

void InitPiano(std::vector<Sound> sounds) {
    _sounds = sounds;
    _numKeys = _sounds.size();
}

void DrawPiano(Rectangle bounds)
{
    if (_numKeys == -1) {
        _numKeys = 10;
    }

    Rectangle r = {bounds.x, bounds.y, bounds.width - 5, bounds.height};
    float colWid = r.width / (float)_numKeys;

    int col = 0;
    for (float i = r.x; i < r.width; i += colWid)
    {
        Vector2 mouse = GetMousePosition();

        Rectangle keyInactive = {i, r.y, colWid, r.height};
        Rectangle keyPressed = {i, r.y + 10, colWid, r.height - 20};

        if (CheckCollisionPointRec(mouse, keyInactive))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                PlaySound(_sounds[col]);
                DrawRectangleRec(keyPressed, col++ % 2 == 0 ? BLACK : WHITE);
            }
            else
            {
                DrawRectangleRec(keyInactive, col++ % 2 == 0 ? BLACK : WHITE);
            }
        }
        else
        {
            DrawRectangleRec(keyInactive, col++ % 2 == 0 ? _blackKeyInactive : _whiteKeyInactive);
        }
    }
}
