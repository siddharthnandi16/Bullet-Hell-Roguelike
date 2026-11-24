#ifndef TITLESCREEN_H
#define TITLESCREEN_H
#include "gamedata.h"
#include "level.h"
GameMode drawTitleScreen(void);
void Draw_Char_Select(Player *player);
void Select_Level(Level_Data *Current_Level, int *current_level);
void Music_Room();
void Draw_End_Screen(WINDOW *win, int area_w, int area_h);
#endif