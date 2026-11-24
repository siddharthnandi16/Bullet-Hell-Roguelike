#ifndef HUD_H
#define HUD_H
#define HUD_HEIGHT 4 
#define HUD_WIDTH 102
void drawHUD(Player *player);
void init_hud(int offset_y, int offset_x);
extern WINDOW *hud_win;
#endif