#include <pdcurses.h>
#include "gamedata.h"
// This file stores data on players, enemies and bullets
/* struct Player{
    float px, py; //Stores the player's position
    float vx, vy; // Stores the player's standard movement speed
    float dx, dy; //Stores the player's current movement direction and speed
    int alive; //Stores whether the player is alive or dead, 1=alive, 0=dead
    int lives; //Stores the number of lives of the player
    bool hasLasers, hasBombs; // Stores whether the player has access to special weapons
    int fire_rate; //Determines rate of fire
    char symbol;
    bool speed_mode_fast ; //Making this false halves player movement
    bool q_was_down; //Checks whether the speed-toggle was held down last frame and prevents it from activating if it was
  WeaponID weapon_id;
    WeaponID inventory[100]; //Size maximised for debug purposes will be reduced to 10 later 
}; */
// Commented out since it conflicts with the typedef in gamedata.h
//Bool that stores whether the player is moving diagonally
bool moving_diagonally = FALSE;
struct Player player = {
    .px = PLAYFIELD_W/2, .py = (PLAYFIELD_H)/3*2,
    .vx = 2, .vy = 2,
    .dx = 2, .dy = 2,
    .alive = 1,
    .lives = 8,
    .hasLasers = true, .hasBombs = true,
    .fire_rate = 20,
    .symbol = '@',
    .speed_mode_fast = true,
    .q_was_down = false,
    .weapon_id = AUTOPISTOL_ID,
    .inventory = {AUTOPISTOL_ID, MACHINEGUN_ID, LASRIFLE_PLAYER_ID, 
        BOMB_PLAYER_ID, PLASMARIFLE_PLAYER_ID, MISSILE_PLAYER_ID,PLASMA_STORM_ID, 
     GRAND_CANNON_ID, MINIGUN_ID},
    // For debug purposes, all implemented weapons are given to the player 
    .invuln_frames = 0,
    .score =0,
    .class = PLAYER_DEBUG
};
// Fast but with weaker weapons
struct Player fighter_jet = {
    .px = PLAYFIELD_W/2, .py = (PLAYFIELD_H)/3*2,
    .vx = 3, .vy = 3,
    .dx = 3, .dy = 3,
    .alive = 1,
    .lives = 8,
    .hasLasers = true, .hasBombs = true,
    .fire_rate = 20,
    .symbol = '@',
    .speed_mode_fast = true,
    .q_was_down = false,
    .weapon_id = MACHINEGUN_ID,
    .inventory = {MACHINEGUN_ID, LASRIFLE_PLAYER_ID, SHOTGUN_ID}, 
    .invuln_frames = 0,
    .score =0,
    .class = PLAYER_FIGHTER
};
//Slow but has access to many strong weapons
struct Player flying_fortress= {
    .px = PLAYFIELD_W/2, .py = (PLAYFIELD_H)/3*2,
    .vx = 1, .vy = 1,
    .dx = 1, .dy = 1,
    .alive = 1,
    .lives = 8,
    .hasLasers = true, .hasBombs = true,
    .fire_rate = 20,
    .symbol = '@',
    .speed_mode_fast = true,
    .q_was_down = false,
    .weapon_id = BOMB_PLAYER_ID,
    .inventory = {BOMB_PLAYER_ID, MACHINEGUN_ID, MISSILE_PLAYER_ID}, 
    .invuln_frames = 0,
    .score =0,
    .class = PLAYER_FORTRESS
};
//Supposed to have experimental and high-tech weapons like the plasma rifle, EMP and chain lightning
struct Player experimental_fighter= {
    .px = PLAYFIELD_W/2, .py = (PLAYFIELD_H)/3*2,
    .vx = 2, .vy = 2,
    .dx = 2, .dy = 2,
    .alive = 1,
    .lives = 8,
    .hasLasers = true, .hasBombs = true,
    .fire_rate = 20,
    .symbol = '@',
    .speed_mode_fast = true,
    .q_was_down = false,
    .weapon_id = PLASMACANNON_ID,
    .inventory = {PLASMACANNON_ID, SPIRAL_CANNON_ID}, 
    .invuln_frames = 0,
    .score =0,
    .class = PLAYER_EXPERIMENTAL
};
//Backup version of player struct. ALWAYS KEEP IT THE SAME AS THE PLAYER STRUCT
struct Player player_backup = {
    .px = PLAYFIELD_W/2, .py = (PLAYFIELD_H/3) * 2,
    .vx = 2, .vy = 2,
    .dx = 2, .dy = 2,
    .alive = 1,
    .lives = 8,
    .hasLasers = true, .hasBombs = true,
    .fire_rate = 20,
    .symbol = '@',
    .speed_mode_fast = true,
    .q_was_down = false,
    .weapon_id = AUTOPISTOL_ID,
    .inventory = {AUTOPISTOL_ID, MACHINEGUN_ID, LASRIFLE_PLAYER_ID, 
        BOMB_PLAYER_ID, PLASMARIFLE_PLAYER_ID, MISSILE_PLAYER_ID,PLASMA_STORM_ID, 
     GRAND_CANNON_ID, MINIGUN_ID},
    // For debug purpose, all implemented weapons are given to the player 
    .invuln_frames = 0,
    .score = 0,
    .class = PLAYER_DEBUG
};
bool Level_Complete = FALSE;
BossState state = BOSS_NORMAL;
int boss_state_timer = 0;                
 bool boss_invulnerable = FALSE;  
 int unsigned long tick = 0; 
 int current_level = 1;   
 char continue_choice = 'a';
 bool continued = FALSE;
 bool boss_fight_ongoing = FALSE;
 GameMode game_mode = 0;
  
