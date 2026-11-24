#ifndef LEVEL_H
#define LEVEL_H
#include "spawn.h"
#include "sound.h"
#include "dialogue.h"
#include "gamedata.h"
//Defintion of struct that stores level data
extern int old_screen_py, old_screen_px;
typedef struct Level_Data{
    spawn_entry *spawn_table;
    int spawn_count;
    sound_entry *sound_table;
    int sound_count;
    dialogue_entry *dialogue_table;
    int dialogue_count;
    struct Level_Data *next_level;
}Level_Data;
extern Level_Data *Current_Level;
extern Level_Data level_1;
extern Level_Data level_2;
extern Level_Data level_3;
extern Level_Data level_4;
char bgchar(int max_x, int max_y);
bool should_draw_cell(int max_y, int max_x);
int scrollanddraw(int *old_screen_px, int *old_screen_py);
void level(const Level_Data *level);
extern int rows_scrolled;
EnemyType pick_type_weighted_by_difficulty(long double difficulty);
EnemyBehavior pick_behavior_for_type(EnemyType type, long double difficulty);
EnemyConfig pick_enemy_config(long double difficulty);
void spawn_wave(long double difficulty);
int pick_music_track(long double difficulty);
void endless_level(void);
void init_levels(void);
void reset_level_tables(Level_Data *level);
extern Level_Data level_1;
extern unsigned long int spawn_tick;
void Check_Boss_Fight();
void New_Weapons(Player *player);
#endif