#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "gamedata.h"
#include <stdbool.h>
extern int quit;
extern bool moving_diagonally;
int move_player(Player *player);
void fire_player(Player *player);
#endif