#include <stdio.h>
#include <stdlib.h>
#include <pdcurses.h>
#include <string.h>
#include "miniaudio.h"
#include "titlescreen.h"
#include "gamedata.h"
#include "sound.h"
#include "window.h"
#include "hud.h"
#include "level.h"
#include "highscores.h"
#define NUM_CHAR_OPTIONS (sizeof(char_options) / sizeof(char_options[0]))
#define NUM_LEVEL_OPTIONS (sizeof(Level_options) / sizeof(Level_options[0]))
#define NUM_MUSIC_OPTIONS (sizeof(Song_options) / sizeof(Song_options[0]))
GameMode drawTitleScreen(void){ 
   PlaySoundEffect(&loaded_sounds[Titlescreen_MUSIC]);
    const char *menu_options[] = {
    "Story Mode",
    "Endless Mode",
    "Level Select",
    "High Scores",
    "Music Room",
    "Quit"
};
 #define NUM_MENU_OPTIONS (sizeof(menu_options) / sizeof(menu_options[0]))
    erase();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    const char *titlescreen[] = {
    " _____                       ______ _ _       _   ",
    "/  ___|                      | ___ (_) |     | |  ",
    "\\ `--. _ __   __ _  ___ ___  | |_/ /_| | ___ | |_ ",
    " `--. \\ '_ \\ / _` |/ __/ _ \\ |  __/| | |/ _ \\| __|",
    "/\\__/ / |_) | (_| | (_|  __/ | |   | | | (_) | |_ ",
    "\\____/| .__/ \\__,_|\\___\\___| \\_|   |_|_|\\___/ \\__|",
    "      | |                                         ",
    "      |_|                                         "
};
int num_lines=8;
int max_x, max_y;
getmaxyx(stdscr, max_y, max_x);
int art_width = strlen(titlescreen[0]); // All lines are of same width, used to make it centred
int start_y = PLAYFIELD_H / 4;                // Prints the title screen near the top
int start_x = (PLAYFIELD_W - art_width) / 2;


//mvprintw(start_y + 12, start_x + 6, "Press any key to start the game");
int selected = 0;
int game_starting = 0; //1= true, 0=false
keypad(stdscr, TRUE);

while(1) {
    nodelay(stdscr, TRUE);
    handle_mute_toggle();
    erase();  // Clear screen each frame
    
 for (int i = 0; i < num_lines; i++) {
    attron(COLOR_PAIR(1)); // Contains black background and green foreground
    mvprintw(offset_y + i, offset_x + start_x, "%s", titlescreen[i]);
}
    // Draw menu options
    int menu_y = offset_y + 12;
    for (int i = 0; i < NUM_MENU_OPTIONS; i++) {
        if (i == selected) {
            attron(A_REVERSE);
        }
        mvprintw(menu_y + i, offset_x + start_x, "%s", menu_options[i]);
        if (i == selected) {
            attroff(A_REVERSE);
        }
    }
    refresh();
    
    int ch = getch();
    switch(ch) {
        case KEY_RESIZE:
        erase();
        resize_term(0, 0);
        getmaxyx(stdscr, max_y, max_x);
        update_playfield_offset(max_x, max_y);
        refresh();
        syncConsoleBufferToWindow();
        break;
        case 60419:  // Up
            selected = (selected - 1 + NUM_MENU_OPTIONS) % NUM_MENU_OPTIONS;
            break;
        case 60418:  // Down
            selected = (selected + 1) % NUM_MENU_OPTIONS;
            break;
        case '\n':  // Press enter to confirm selection
        delwin(hud_win);
        getmaxyx(stdscr, max_y, max_x);
        update_playfield_offset(max_x, max_y);
        init_hud(offset_y, offset_x);
        ma_sound_stop(&loaded_sounds[Titlescreen_MUSIC]);
        game_starting = 1;
            return (GameMode)selected;
        default:
            break;
    }
    if (game_starting == 1){
        break;
    }
}
napms(8);
return (GameMode)selected;
}
const char *char_options[] = {
    "Fighter Jet",
    "Flying Fortress",
    "Experimental Fighter",
  
};

void Draw_Char_Select(Player *player){
    ma_sound_start(&loaded_sounds[Titlescreen_MUSIC]);
    int character_selected = 0; //1 for true, 0 for false
    nodelay(stdscr, FALSE);
    erase();
    int char_selected = 0;
    int ch, max_x, max_y;
    while(1){
 for (int i = 0; i < NUM_CHAR_OPTIONS; i++) {
        if (i == char_selected) {
            attron(A_REVERSE);
        }
        mvprintw(offset_y + i, offset_x + PLAYFIELD_W/3, "%s", char_options[i]);
        if (i == char_selected) {
            attroff(A_REVERSE);
        }
    }
    refresh();
    ch = getch();
    switch(ch) {
        case KEY_RESIZE:
        erase();
        resize_term(0, 0);
        getmaxyx(stdscr, max_y, max_x);
        update_playfield_offset(max_x, max_y);
        refresh();
        syncConsoleBufferToWindow();
        break;
        case 60419:  // Up
            char_selected = (char_selected - 1 + NUM_CHAR_OPTIONS) % NUM_CHAR_OPTIONS;
            break;
        case 60418:  // Down
            char_selected = (char_selected + 1) % NUM_CHAR_OPTIONS;
            break;
        case '\n':  // Press enter to confirm selection
        character_selected = 1;
        default:
        break;
    }
     if (character_selected == 1){
        ma_sound_stop(&loaded_sounds[Titlescreen_MUSIC]);
        nodelay(stdscr, TRUE);
    switch(char_selected){
        case 0: //Fighter jet
        *player = fighter_jet;
        break;
        case 1: //Flying fortress
        *player = flying_fortress;
        break;
        case 2: //Experimental fighter
        *player = experimental_fighter;
        break;
        //Debug player commented out
       // case 3: 
       // break;
        default:
        break;
    }
    erase();
     break;
    }
    }
    napms(8);
}
const char *Level_options[] = {
    "Level 1",
    "Level 2",
    "Level 3"
};
//Function to select a level
void Select_Level(Level_Data *Current_Level, int *current_level){
    ma_sound_start(&loaded_sounds[Titlescreen_MUSIC]);
    int ch, max_x, max_y, level_selected = 0;
    bool level_selection_done = FALSE;
    nodelay(stdscr, FALSE);
    while(level_selection_done == FALSE){
erase();
for (int i = 0; i < NUM_LEVEL_OPTIONS; i++) {
        if (i == level_selected) {
            attron(A_REVERSE);
        }
        mvprintw(offset_y + i, offset_x + PLAYFIELD_W/3, "%s", Level_options[i]);
        if (i == level_selected) {
            attroff(A_REVERSE);
        }
    }
    ch = getch();
    switch(ch) {
        case KEY_RESIZE:
        erase();
        resize_term(0, 0);
        getmaxyx(stdscr, max_y, max_x);
        update_playfield_offset(max_x, max_y);
        refresh();
        syncConsoleBufferToWindow();
        break;
        case 60419:  // Up
            level_selected = (level_selected - 1 + NUM_LEVEL_OPTIONS) % NUM_LEVEL_OPTIONS;
            break;
        case 60418:  // Down
            level_selected = (level_selected + 1) % NUM_LEVEL_OPTIONS;
            break;
        case '\n':  // Press enter to confirm selection
        delwin(hud_win);
        getmaxyx(stdscr, max_y, max_x);
        update_playfield_offset(max_x, max_y);
        init_hud(offset_y, offset_x);
        ma_sound_stop(&loaded_sounds[Titlescreen_MUSIC]);
        level_selection_done = TRUE;
        break;
        default:
        break;
    }
    if (level_selection_done == TRUE){
        nodelay(stdscr, TRUE);
        switch(level_selected){
        case 0: //Level 1
        *Current_Level = level_1;
        *current_level = 1;
        break;
        case 1: //Level 2
        *Current_Level = level_2;
        *current_level = 2;
        break;
        case 2: //Level 3, commented out due to being unimplemented
       *Current_Level = level_3;
       *current_level = 3;
        break;
        default:
        break;
    }
    }
    }
    
}
const char *Song_options[] = {
    "Titlescreen - Map",
    "Level 1 - Mars",
    "Level 2 - Venus",
    "Level 3 - Boss Fight",
    "Exit Music Room" //Here corresponds to case 4
};
// Function to play selected music
void Music_Room(){
int ch, max_x, max_y, song_selected = 0;
    bool music_room_active = TRUE;
    nodelay(stdscr, TRUE);
    int current_soundtrack = Titlescreen_MUSIC;
    erase();
    while(music_room_active == TRUE){
for (int i = 0; i < NUM_MUSIC_OPTIONS; i++) {
        if (i == song_selected) {
            attron(A_REVERSE);
        }
        mvprintw(offset_y + i, offset_x + PLAYFIELD_W/3, "%s", Song_options[i]);
        if (i == song_selected) {
            attroff(A_REVERSE);
        }
    }
    ch = getch();
    switch(ch) {
        case KEY_RESIZE:
        erase();
        resize_term(0, 0);
        getmaxyx(stdscr, max_y, max_x);
        update_playfield_offset(max_x, max_y);
        refresh();
        syncConsoleBufferToWindow();
        break;
        case 60419:  // Up
            song_selected = (song_selected - 1 + NUM_MUSIC_OPTIONS) % NUM_MUSIC_OPTIONS;
            break;
        case 60418:  // Down
            song_selected = (song_selected + 1) % NUM_MUSIC_OPTIONS;
            break;
        default:
        break;
    }
    switch(song_selected){
        case Titlescreen_MUSIC:
        ma_sound_stop(&loaded_sounds[current_soundtrack]);
        current_soundtrack = Titlescreen_MUSIC;
        ma_sound_start(&loaded_sounds[current_soundtrack]);
        break;
        case Level_1:
        ma_sound_stop(&loaded_sounds[current_soundtrack]);
        current_soundtrack = Level_1;
        ma_sound_start(&loaded_sounds[current_soundtrack]);
        break;
        case Level_2:
        ma_sound_stop(&loaded_sounds[current_soundtrack]);
        current_soundtrack = Level_2;
        ma_sound_start(&loaded_sounds[current_soundtrack]);
        break;
        case Level_3:
        ma_sound_stop(&loaded_sounds[current_soundtrack]);
        current_soundtrack = Level_3;
        ma_sound_start(&loaded_sounds[current_soundtrack]);
        break;
        case 4: //Quit music room
        ma_sound_stop(&loaded_sounds[current_soundtrack]);
        music_room_active = FALSE;
        break;
        default:
        break;
    }
    }
}

//Function to print congratulations upon completing the game
const char *congrats_art[] = {
    "  ____                            _         _       _   _                 _ ",
    " / ___|___  _ __   __ _ _ __ __ _| |_ _   _| | __ _| |_(_) ___  _ __  ___| |",
    "| |   / _ \\| '_ \\ / _` | '__/ _` | __| | | | |/ _` | __| |/ _ \\| '_ \\/ __| |",
    "| |__| (_) | | | | (_| | | | (_| | |_| |_| | | (_| | |_| | (_) | | | \\__ \\_|",
    " \\____\\___/|_| |_|\\__, |_|  \\__,_|\\__|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_|___(_)",
    "                  |___/                                                     "
};
#define CONGRATS_ROWS (sizeof(congrats_art) / sizeof(congrats_art[0]))
//Win= window where you want it to be printed, area_w and area_h are the width and height of that window respectively
void Draw_End_Screen(WINDOW *win, int area_w, int area_h)
{
    char choice = 'a';
    while(choice != '\n'){
        erase();
   
int art_h = CONGRATS_ROWS;
    int art_w = (int)strlen(congrats_art[0]);

    int start_y = (offset_y + area_h - art_h) / 2;
    int start_x = (offset_x + area_w - art_w) / 2;

    wattron(win, COLOR_PAIR(10));
    for (int i = 0; i < art_h; i++) {
        mvwaddstr(win, start_y + i, start_x, congrats_art[i]);
    }
    wattroff(win, COLOR_PAIR(10));

    const char *line1 = "You have beaten SpacePilot! Thanks for playing.";
    const char *line2 = "Press Enter to continue.";

    mvwaddstr(win, start_y + art_h + 1, (area_w - (int)strlen(line1)) / 2, line1);
    mvwaddstr(win, start_y + art_h + 3, (area_w - (int)strlen(line2)) / 2, line2);

    wnoutrefresh(win);
    doupdate();
    nodelay(stdscr, FALSE);
    choice = getch();
    }
    if(choice = '\n') nodelay(stdscr, TRUE);
    
}

