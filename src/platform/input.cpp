#include "platform/input.h"

Direction read_direction() {
    Direction d{0, 0, false};
    if (IsKeyDown(KEY_LEFT))  d.dx -= 1;
    if (IsKeyDown(KEY_RIGHT)) d.dx += 1;
    if (IsKeyDown(KEY_UP))    d.dy -= 1;
    if (IsKeyDown(KEY_DOWN))  d.dy += 1;
    d.moving_diagonally = (d.dx != 0 && d.dy != 0);
    return d;
}