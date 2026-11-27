#ifndef SRC_PLATFORM_INPUT_H_
#define SRC_PLATFORM_INPUT_H_

#include "raylib.h"
struct Direction{
    int dx;
    int dy;
    bool moving_diagonally;
};
Direction read_direction();
#endif  // SRC_PLATFORM_INPUT_H_
