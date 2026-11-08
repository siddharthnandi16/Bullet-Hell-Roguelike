#include <pdcurses.h>
#include <stdlib.h>
#include <time.h>
#include "gamedata.h"
#include "level.h"
#include "spawn.h"
#include "sound.h"
#include "dialogue.h"
#include "enemy.h"
#include "window.h"
extern int unsigned long tick;
extern int spawn_table_count_1;
int rows_scrolled;
float density = 0.2;
extern int max_x, max_y;
//Currently unimplemented functions to draw a background with a given density
char bgchar(int max_x, int max_y){
    const char chars[] = ".,'$^*()";
    int n = sizeof(chars) - 1;
    return chars[rand() % n];
}

bool should_draw_cell(int max_y, int max_x){
    return (rand() / (double)RAND_MAX) < density;
}
//Scrolls the game and renders the player. Also returns the number of rows scrolled
 int old_screen_px = -1;
 int old_screen_py = -1;
int scrollanddraw(int *old_screen_px, int *old_screen_py){
    if (*old_screen_px != -1) {
        mvaddch(*old_screen_py, *old_screen_px, ' ');
    }

    scrollok(stdscr, TRUE);
    if (tick % 5 == 0 && boss_fight_ongoing != TRUE){
     wscrl(stdscr, -1);
    rows_scrolled++;
    }

    int screen_px = offset_x + (int)player.px;
    int screen_py = offset_y + (int)player.py;

    if (player.invuln_frames > 0){attron(A_REVERSE);}
    attron(COLOR_PAIR(1));
    mvaddch(screen_py, screen_px, player.symbol);
    attroff(COLOR_PAIR(1));
    if (player.invuln_frames > 0){attroff(A_REVERSE);}

    *old_screen_px = screen_px;
    *old_screen_py = screen_py;

    wnoutrefresh(stdscr);
    return rows_scrolled;
}
//Sound count and sound table currently do nothing and should be ignored
//Functionality for them is planned at a later date
Level_Data level_4={
.spawn_table = spawn_table_4,
.spawn_count = 1,
    .sound_table = sound_table_1,
    .sound_count = 0,          
    .dialogue_table = dialogue_table_1,
    .dialogue_count = 0,
    .next_level = NULL
};
Level_Data level_3={
.spawn_table = spawn_table_3,
.spawn_count = 1,
    .sound_table = sound_table_1,
    .sound_count = 0,          
    .dialogue_table = dialogue_table_3,
    .dialogue_count = 0,
    .next_level = NULL
};
//Data for levels 1 and 2
Level_Data level_2={
.spawn_table = spawn_table_2,
.spawn_count = 1,
    .sound_table = sound_table_1,
    .sound_count = 0,          
    .dialogue_table = dialogue_table_2,
    .dialogue_count = 0,
    .next_level = &level_3
};
Level_Data level_1={
.spawn_table = spawn_table_1,
.spawn_count = 1,
    .sound_table = sound_table_1,
    .sound_count = 0,          
    .dialogue_table = dialogue_table_1,
    .dialogue_count = 0,
    .next_level = &level_2
};
void init_levels(void) {
    level_1.spawn_count    = spawn_table_1_count;
    level_2.spawn_count    = spawn_table_2_count;
    level_3.spawn_count    = spawn_table_3_count;
    level_1.dialogue_count = dialogue_table_1_count;
    level_2.dialogue_count = dialogue_table_2_count;
    level_3.dialogue_count = dialogue_table_3_count;
}
//Variable that stores the current level
 Level_Data *Current_Level = &level_1;
//Function that handles level progression (enemy spawning)
void level(const Level_Data *level){
    for (int i = 0; i < level->spawn_count; i++) {
        if (level->spawn_table[i].fired){
        continue; // Skips entries that are already fired
        }
        switch (level->spawn_table[i].trigger) {
            case TICK:
                if (tick >= level->spawn_table[i].trigger_time) {

                    spawn_enemy(level->spawn_table[i].type, level->spawn_table[i].behavior, 
                        level->spawn_table[i].aimed, level->spawn_table[i].px,
                         level->spawn_table[i].py,level->spawn_table[i].strafe);
                    level->spawn_table[i].fired = TRUE;
                    
                }
                break;
            case ROW:
                if (rows_scrolled >= level->spawn_table[i].trigger_time) {
                    spawn_enemy(level->spawn_table[i].type, level->spawn_table[i].behavior, 
                                level->spawn_table[i].aimed,
                                level->spawn_table[i].px, level->spawn_table[i].py,
                                level->spawn_table[i].strafe);
                    level->spawn_table[i].fired = TRUE;
                    //Debug code to check whether entities are spawning correctly
                    //fprintf(stderr, "Entry %d fired, trigger_time=%d\n", i, level->spawn_table[i].trigger_time);
                    //Debug code to check if enemy characteristics are being correctly assinged
                    //fprintf(stderr, "Entry horizontal position is %d , shape=%d\n", enemies[i].px, enemies[i].shape );
                }
                break;
        }
    }
    // Section for moving to next level, if one exists
    if (Level_Complete == TRUE && level->next_level != NULL){
    fprintf(stderr, "Level completed.");
    werase(stdscr);
    for (int p= 0;p < MAX_ENEMIES; p++){
        enemies[p].state = DEAD;
    }
    fprintf(stderr, "%d", Current_Level, "\n");
    Current_Level = level->next_level;
    fprintf(stderr, "%d", Current_Level, "\n");
    reset_level_tables(Current_Level);
    Level_Complete = FALSE;   
    state = BOSS_NORMAL;
    boss_state_timer = 0;
    boss_invulnerable = FALSE; 
    tick = 0;
    rows_scrolled = 0;
    }
}
//Function to reset spawn_tables after beating a level
void reset_level_tables(Level_Data *level){
    for (int i = 0; i < level->spawn_count; i++){
        level->spawn_table[i].fired = FALSE;
    }
}
static long double difficulty = 0;
//Function which picks a type for each enemy
EnemyType pick_type_weighted_by_difficulty(long double difficulty) {
    // Clamp difficulty to 0.0-1.0 range
    if (difficulty < 0.0f) difficulty = 0.0f;
    if (difficulty > 1.0f) difficulty = 1.0f;
    
    // GRUNT: 75% -> 40%
    int grunt_weight = 75 - (int)(difficulty * 35);
    
    // RAPIDFIRE: 10% -> 30%
    int rapidfire_weight = 10 + (int)(difficulty * 20);
    
    // Special enemies: split remaining equally
    int special_remaining = 100 - grunt_weight - rapidfire_weight;
    int laser_weight = special_remaining / 3;
    int bomber_weight = special_remaining / 3;
    int hunter_weight = special_remaining - laser_weight - bomber_weight;  // handles rounding
    
    // Roll 0-99 and check weights
    int roll = rand() % 100;
    
    if (roll < grunt_weight) return GRUNT;
    roll -= grunt_weight;
    
    if (roll < rapidfire_weight) return RAPIDFIRE;
    roll -= rapidfire_weight;
    
    if (roll < laser_weight) return LASER_ENEMY;
    roll -= laser_weight;
    
    if (roll < bomber_weight) return BOMBER;
    
    return HUNTER;
}
//Function to choose enemy behaviour given type
EnemyBehavior pick_behavior_for_type(EnemyType type, long double difficulty) {
    // HUNTER type always hunts, regardless of difficulty
    if (type == HUNTER) {
        return HUNT_PLAYER;
    }
    
    // Clamp difficulty
    if (difficulty < 0.0f) difficulty = 0.0f;
    if (difficulty > 1.0f) difficulty = 1.0f;
    
    // STATIC: 30% -> 0%
    int static_weight = 30 - (int)(difficulty * 50);
    
    // Remaining percentage split equally among three behaviors
    int dynamic_remaining = 100 - static_weight;
    int strafe_h_weight = dynamic_remaining / 3;
    int strafe_v_weight = dynamic_remaining / 3;
    int hunt_weight = dynamic_remaining - strafe_h_weight - strafe_v_weight;  // handles rounding
    
    int roll = rand() % 100;
    
    if (roll < static_weight) return STATIC;
    roll -= static_weight;
    
    if (roll < strafe_h_weight) return STRAFE_HORIZONTAL;
    roll -= strafe_h_weight;
    
    if (roll < strafe_v_weight) return STRAFE_VERTICAL;
    
    return HUNT_PLAYER;
}
bool pick_aimed_for_difficulty(long double difficulty) {
    if (difficulty < 0.0f) difficulty = 0.0f;
    if (difficulty > 1.0f) difficulty = 1.0f;

    int aimed_chance = 20 + (int)(difficulty * 15); // 20% -> 35%
    int roll = rand() % 100;

    return (roll < aimed_chance);
}
//Function to set enemy configs
EnemyConfig pick_enemy_config(long double difficulty) {
    EnemyConfig config = {0};
    EnemyType chosen_type = GRUNT;
    EnemyBehavior behavior = STATIC;
    config.type = pick_type_weighted_by_difficulty(difficulty);
    config.behavior = pick_behavior_for_type(config.type, difficulty);\
     config.aimed = pick_aimed_for_difficulty(difficulty);
 config.strafe = 8 + (rand() % 4);  // 8 + (0 to 5) = 8 to 12
    return config;
}
//Function to spawn enemies in waves
#define SPAWN_ZONE_TOP 5 // enemies spawn in top 5 rows only
#define SPAWN_ZONE_WIDTH (PLAYFIELD_W - 2)  
void spawn_wave(long double difficulty) {
 float wave_size = 6 + (difficulty * 10);
    for (int i = 0; i < (int)wave_size; i++) {
       // fprintf(stderr, "  Iteration %d/%d\n", i+1, wave_size);
        EnemyConfig config = pick_enemy_config(difficulty);
        float px = (rand() % (PLAYFIELD_W - 2)) + 1;  
        float py = rand() % SPAWN_ZONE_TOP;
        spawn_enemy(config.type, config.behavior, config.aimed,px, py, config.strafe);
       // fprintf(stderr, "    Spawning %d at (%.0f, %.0f)\n", config.type, px, py);
    }
}
//Function to select music based on difficulty, currently a stub
int pick_music_track(long double difficulty) {
}
//Function to generate an endless procedurally generated level
int unsigned long spawn_tick = 0;
void endless_level(void){
    difficulty = 0;
    if (difficulty < 5){
if (spawn_tick % 300==0) difficulty += 0.05;
    }  
if (spawn_tick % 300 ==0 || spawn_tick==0){
spawn_wave(difficulty);
pick_music_track(difficulty);
}
spawn_tick++;
}
//Function to check whether a boss fight is currently ongoing and set the appropriate flag
void Check_Boss_Fight(){
    boss_fight_ongoing = FALSE;
    for (int i = 0; i < MAX_ENEMIES; i++){
        if(enemies[i].is_boss_part > 0 && enemies[i].state == ALIVE){
            boss_fight_ongoing = TRUE;
            break;
        }
    }
}
//Function to grant the player new weapons upon completing a stage
void New_Weapons(Player *player){
    if(current_level >= 2){
        switch(player->class){
            case PLAYER_FIGHTER:
            player->inventory[3] = SPIRAL_CANNON_ID;
            break;
            case PLAYER_FORTRESS:
            player->inventory[3] = MISSILE_STORM_ID;
            break;
            case PLAYER_EXPERIMENTAL:
            player->inventory[3] = PLASMARIFLE_PLAYER_ID;
            default: 
            break;
        }
    }
    if(current_level >= 3){
        switch(player->class){
            case PLAYER_FIGHTER:
            player->inventory[4] = MINIGUN_ID;
            break;
            case PLAYER_FORTRESS:
            player->inventory[4] = GRAND_CANNON_ID;
            break;
            case PLAYER_EXPERIMENTAL:
            player->inventory[4] = PLASMA_STORM_ID;
            default: 
            break;
        }
    }
}