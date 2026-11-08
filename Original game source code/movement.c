#include <pdcurses.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>
#include "gamedata.h"
#include "movement.h"
#include "window.h"
#include "projectile.h"
#include "hud.h"
#include "dialogue.h"
#include "sound.h"
int quit = 0; //1= true, 0= false
int move_player(Player *player){
    int max_x, max_y;
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    float new_px = player->px, new_py = player->py;
    int choice = getch();
    new_px = fminf(fmaxf(new_px, 0), PLAYFIELD_W - 1); // -1 since columns are 0-indexed
new_py = fminf(fmaxf(new_py, 0), PLAYFIELD_H - 1);

    if (choice == 'q' && !(player->q_was_down)) {
        player->speed_mode_fast = !(player->speed_mode_fast);
    }
    player->q_was_down = (choice == 'q');

    if (player->speed_mode_fast) {
        player->dx = player->vx;
       player->dy = player->vy;
    } else {
        player->dx = player->vx / 2;
        player->dy = player->vy / 2;
    }
// Now only used for key resizing, old switch-based movement code is commented out
  /*  
  switch (choice) {
        case KEY_UP: new_py = player->py - player->dy; break;
        case KEY_DOWN: new_py = player->py + player->dy; break;
        case KEY_LEFT: new_px = player->px - player->dx; break;
        case KEY_RIGHT: new_px = player->px + player->dx; break;   
        break;
        default: break; // Position unchanged on no input
    }
   */
  //Game pausing functionality
  bool paused = FALSE;
    if (choice == 'p' && paused == FALSE){
        scrollok(stdscr, FALSE);
        nodelay(stdscr, FALSE);
        paused = TRUE;
        mvprintw(offset_y + PLAYFIELD_H/2, offset_x + PLAYFIELD_W/2, "PAUSED");
        mvprintw(offset_y + PLAYFIELD_H/2 + 2, offset_x + PLAYFIELD_W/2, "Press Q to return to title screen");
        wnoutrefresh(stdscr);
    }
    while (paused == TRUE){  
    nodelay(stdscr, FALSE);
     choice = getch();
    if (choice == 'q' && paused == TRUE){
        quit = 1;
        scrollok(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        paused = FALSE;
        erase();
     ma_sound_stop(&loaded_sounds[Level_1]);
    ma_sound_stop(&loaded_sounds[Level_2]);
    ma_sound_stop(&loaded_sounds[Level_3]);
        wnoutrefresh(stdscr);
    }
    if (choice == 'p'){
        scrollok(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        paused = FALSE;
        erase();
        wnoutrefresh(stdscr);
    }
    }
    
    if (choice == KEY_RESIZE){
     erase();
    resize_term(0, 0);
    getmaxyx(stdscr, max_x, max_y);
    update_playfield_offset(max_y, max_x);

    int hud_y = offset_y - 4;
    int hud_x = offset_x - 1;
    if (hud_y < 0) hud_y = 0;
    if (hud_x < 0) hud_x = 0;

    werase(hud_win);
    mvwin(hud_win, hud_y, hud_x);
    wresize(hud_win, HUD_HEIGHT, HUD_WIDTH);
    box(hud_win, 0, 0);
    wrefresh(hud_win);
    wnoutrefresh(stdscr);
    syncConsoleBufferToWindow();}
//New movement code, allowing for diagonal movement
bool upward_movement = (GetAsyncKeyState(VK_UP)& 0x8000) != 0;
bool downward_movement = (GetAsyncKeyState(VK_DOWN)& 0x8000) != 0;
bool rightward_movement = (GetAsyncKeyState(VK_RIGHT)& 0x8000) != 0;
bool leftward_movement = (GetAsyncKeyState(VK_LEFT)& 0x8000) != 0;
bool moving_horizontally = FALSE;
bool moving_vertically = FALSE;
if (upward_movement == TRUE || downward_movement == TRUE) moving_vertically = TRUE;
if (rightward_movement == TRUE || leftward_movement == TRUE) moving_horizontally = TRUE;
if (moving_horizontally && moving_vertically == TRUE) moving_diagonally = TRUE;
if (moving_diagonally == TRUE){
    player->dx = player->dx * 0.7071;
    player->dy = player->dy * 0.7071;
}
if (upward_movement == TRUE) new_py = player->py - player->dy;
if (downward_movement == TRUE) new_py = player->py + player->dy;
if (leftward_movement == TRUE) new_px = player->px - player->dx;
if (rightward_movement == TRUE) new_px = player->px + player->dx;
    new_px = fminf(fmaxf(new_px, 0), (float)PLAYFIELD_W-1);
    new_py = fminf(fmaxf(new_py, 0), (float)PLAYFIELD_H-1);
    player->px = new_px;
    player->py = new_py;
    return quit;
}

//Function that fires player weapons
#define NUM_WEAPON_SLOTS 9
void fire_player(Player *player) {
    static bool switch_key_was_down[NUM_WEAPON_SLOTS] = {false};
    int slot_keys[NUM_WEAPON_SLOTS] = {'1', '2', '3', '4', '5', '6', '7','8','9'};

    // Switches weapons once per click
    for (int i = 0; i < NUM_WEAPON_SLOTS; i++) {
        bool key_down = (GetAsyncKeyState(slot_keys[i]) & 0x8000) != 0;
        if (key_down && !switch_key_was_down[i]) {
            if (player->inventory[i] != EMPTY_ID) {
                player->weapon_id = player->inventory[i];
            }
            // Does nothing if the slot is empty
        }
        switch_key_was_down[i] = key_down;
    }

    // Firing - held, gated by cooldown
    player->fire_rate++;
    bool fire_key_down = (GetAsyncKeyState('Z') & 0x8000) != 0;
    if (fire_key_down) {
        const WeaponType *weapon = get_weapon_template(player->weapon_id);
        if (weapon != NULL && player->fire_rate >= weapon->cooldown_frames) {
            fire_weapon(weapon, player->px, player->py - 1, 90, TRUE);
            player->fire_rate = 0;
        }
    }
}