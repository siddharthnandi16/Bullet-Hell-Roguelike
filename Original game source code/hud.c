#include <pdcurses.h>
#include "gamedata.h"
#include "window.h"
#include "projectile.h"
#include "hud.h"
#include "dialogue.h"
#include <math.h>
WINDOW *hud_win = NULL;
//Function to initialise a globally accessible HUD
void init_hud(int offset_y, int offset_x)
{
    int hud_y = (offset_y >= 4) ? offset_y - 4 : 0;
    int hud_x = (offset_x >= 1) ? offset_x - 1 : 0;
    hud_win = newwin(HUD_HEIGHT, HUD_WIDTH,
                     hud_y , hud_x);
}
//Function to draw and update the HUD
void drawHUD(Player *player){
   // Moved to main since it may cause a memory leak here
   // init_hud(offset_y,offset_x);
   wbkgd(hud_win, COLOR_PAIR(10));
wattron(hud_win,COLOR_PAIR(10));
box(hud_win,0,0);
wattroff(hud_win,COLOR_PAIR(10));
wattron(hud_win,COLOR_PAIR(12));
mvwprintw(hud_win,1,1, "Lives:");
mvwprintw(hud_win,1,7, "%d",player->lives);
mvwprintw(hud_win,1,9, "Score:");
mvwprintw(hud_win,1,16, "%d",player->score);
mvwprintw(hud_win,1,20, "Weapon:");
//Currently has a minor visual bug where weapon name don't get erased fully upon swapping to a weapon with a shorter name
WeaponType *current_weapon = get_weapon_template(player->weapon_id);
char *weapon_name = current_weapon->display_name;
mvwprintw(hud_win, 1, 27, "%-16s", "");
mvwprintw(hud_win,1,27, "%s", weapon_name);
if (player->fire_rate < current_weapon->cooldown_frames){
    mvwprintw(hud_win, 1, 43, "%-13s", "");
    mvwprintw(hud_win,1,43, "LOADING");
}
else {
    wattroff(hud_win, COLOR_PAIR(12));
    wattron(hud_win, COLOR_PAIR(10));
    mvwprintw(hud_win, 1, 43, "%-13s", "");
mvwprintw(hud_win,1,43, "READY TO FIRE");
wrefresh(hud_win);
wattroff(hud_win, COLOR_PAIR(10));
}
wattron(hud_win, COLOR_PAIR(12));
mvwprintw(hud_win,1,57, "Speed:");
float speed = 0;
if (moving_diagonally == TRUE){
speed = sqrt((player->dx*player->dx) + (player->dy*player->dy));
}
else speed = player->dx;
mvwprintw(hud_win,1,64,"%.2f", speed);
mvwprintw(hud_win,1,70, "m/s");
wattroff(hud_win,COLOR_PAIR(12));
    switch(current_level){
        case 1:
        update_dialogue(dialogue_table_1);
        break;
        case 2:
        update_dialogue(dialogue_table_2);
        break;
        case 3:
        update_dialogue(dialogue_table_3);
        break;
        default:
        break;
    } 
wrefresh(hud_win);
}