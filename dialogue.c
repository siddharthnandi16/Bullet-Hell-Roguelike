#include <pdcurses.h>
#include  "gamedata.h"
#include "dialogue.h"
#include "level.h"
#include "window.h"
#include "hud.h"
#define DIALOGUE_HEIGHT 1   
#define DIALOGUE_WIDTH 102
// Use 10 for green on black, 11 for red on black, 12 for amber on black
dialogue_entry dialogue_table_1[MAX_DIALOGUE]={
    [0] ={
.trigger = ROW, .auto_scroll=TRUE, .bold = TRUE, .color = 10, 
.fired = FALSE, .trigger_time = 0, .text = "Use the arrow keys to move your aircraft!",
    },
    [1] ={
.trigger = ROW, .auto_scroll=TRUE, .bold = TRUE, .color = 10, 
.fired = FALSE, .trigger_time = 90, .text = "Press Z to fire your weapon!",
    },
    [2] ={
.trigger = ROW, .auto_scroll=TRUE, .bold = TRUE, .color = 10, 
.fired = FALSE, .trigger_time = 180, .text = "Use 1,2 and 3 to switch between weapons!",
    },
    [3] ={
.trigger = ROW, .auto_scroll=TRUE, .bold = TRUE, .color = 10, 
.fired = FALSE, .trigger_time = 270, .text = "Press Q to switch between fast and slow movement.",
    },
    [4] ={
.trigger = ROW, .auto_scroll=TRUE, .bold = TRUE, .color = 12, 
.fired = FALSE, .trigger_time = 360, .text = "MISSION CONTROL:Shoot down any enemy aircraft you see.Beware the vast enemy fleet!",
    }   
};
const int dialogue_table_1_count = sizeof(dialogue_table_1)/sizeof(dialogue_table_1[0]);
dialogue_entry dialogue_table_2[MAX_DIALOGUE]={
    [0] ={
.trigger = ROW, .auto_scroll=TRUE, .bold = TRUE, .color = 10, 
.fired = FALSE, .trigger_time = 0, .text = "Press 4 to try out your new weapon!"
    }
};
const int dialogue_table_2_count = sizeof(dialogue_table_2)/sizeof(dialogue_table_2[0]);
dialogue_entry dialogue_table_3[MAX_DIALOGUE]={
    [0] ={
.trigger = ROW, .auto_scroll=TRUE, .bold = TRUE, .color = 10, 
.fired = FALSE, .trigger_time = 0, .text = "Press 5 to try out your new weapon!"
    }
};
const int dialogue_table_3_count = sizeof(dialogue_table_3)/sizeof(dialogue_table_3[0]);
//Commented out since it is now redundant
/* WINDOW *dialogue_win = NULL;
void init_dialogue_window(int offset_y, int offset_x){
    fprintf(stderr, "newwin(%d, %d, %d, %d)\n",
    DIALOGUE_HEIGHT, DIALOGUE_WIDTH,
    offset_y + PLAYFIELD_H + 1, offset_x - 1);
    fprintf(stderr, "dialogue_win failed to create! offset_y=%d offset_x=%d\n", offset_y, offset_x);
    dialogue_win = newwin(DIALOGUE_HEIGHT, DIALOGUE_WIDTH,
                           offset_y + PLAYFIELD_H + 1, offset_x - 1);
} */
void update_dialogue(dialogue_entry *dialogue_table){

    for (int i = 0; i < MAX_DIALOGUE; i++){
        if (dialogue_table[i].fired == TRUE || dialogue_table[i].text == NULL){
        continue; // Skips entries that are already fired or are blank
        }
        switch(dialogue_table[i].trigger){
            case ROW: 
            if (rows_scrolled >= dialogue_table[i].trigger_time){
                if (dialogue_table[i].bold == TRUE) wattron(hud_win, A_BOLD);
                wattron(hud_win, COLOR_PAIR(dialogue_table[i].color));
                mvwprintw(hud_win, 2, 1, "%-100s", "");
                dialogue_table[i].fired = TRUE;
                mvwprintw(hud_win, 2, 1, 
                    "%s", dialogue_table[i].text);
                wattroff(hud_win, COLOR_PAIR(dialogue_table[i].color));
                 if (dialogue_table[i].bold == TRUE) wattroff(hud_win, A_BOLD);
                wnoutrefresh(hud_win);
            }
            break;
            case TICK:
            if (tick >= dialogue_table[i].trigger_time){
                 if (dialogue_table[i].bold == TRUE) wattron(hud_win, A_BOLD);
                wattron(hud_win, COLOR_PAIR(dialogue_table[i].color));
                mvwprintw(hud_win, 2, 1, "%-100s", "");
                dialogue_table[i].fired = TRUE;
                mvwprintw(hud_win, 2, 1, 
                    "%s", dialogue_table[i].text);
                wattroff(hud_win, COLOR_PAIR(dialogue_table[i].color));
                if (dialogue_table[i].bold == TRUE) wattroff(hud_win, A_BOLD);
                wnoutrefresh(hud_win);
            }
            break;
        }
    }
}

