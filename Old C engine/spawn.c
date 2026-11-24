#include <pdcurses.h>
#include  "gamedata.h"
#include "spawn.h"
//Game runs at 30 ticks per second and scrolls at 5 rows per second
//After editing spawn table, ALWAYS set spawn_count in level.c to the number of enemies in that table
//Old version of level 1, commented out due to being unbalanced
/*spawn_entry spawn_table_1[]={
    //Total time until reaching boss of level 1 = 1200
    [0] ={
.trigger = ROW, .type = CARRIER_BOSS, .behavior = CARRIER_SPECIAL, .fired = FALSE, .px = 50, .py = 6, 
.strafe = 0, .trigger_time=1200, .aimed = TRUE
    },
    [1] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 50, .py = 4, 
.strafe = 0, .trigger_time=90, .aimed = FALSE
    },
    [2] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 70, .py = 4, 
.strafe = 0, .trigger_time=90, .aimed = FALSE
    },
    [3] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 80, .py = 4, 
.strafe = 0, .trigger_time=90, .aimed = FALSE
    },
    [4] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 40, .py = 4, 
.strafe = 0, .trigger_time=90, .aimed = FALSE
    },
    [5] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 30, .py = 4, 
.strafe = 0, .trigger_time=90, .aimed = FALSE
    },
    [6] ={
.trigger = ROW, .type = GRUNT, .behavior = STRAFE_HORIZONTAL, .fired = FALSE, .px = 30, .py = 3, 
.strafe = 10, .trigger_time=120, .aimed = FALSE
    },
    [7] ={
.trigger = ROW, .type = GRUNT, .behavior = STRAFE_HORIZONTAL, .fired = FALSE, .px = 50, .py = 3, 
.strafe = 10, .trigger_time=120, .aimed = FALSE
    },
    [8] ={
.trigger = ROW, .type = GRUNT, .behavior = HUNT_PLAYER_FAR, .fired = FALSE, .px = 10, .py = 1, 
.strafe = 10, .trigger_time=150, .aimed = FALSE
    },
    [9] ={
.trigger = ROW, .type = GRUNT, .behavior = HUNT_PLAYER_FAR, .fired = FALSE, .px = 90, .py = 3, 
.strafe = 10, .trigger_time=150, .aimed = FALSE
    },
    [10] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 5, .py = 5, 
.strafe = 10, .trigger_time=210, .aimed = TRUE
    },
    [11] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 95, .py = 5, 
.strafe = 10, .trigger_time=210, .aimed = TRUE
    },
    [12] ={
.trigger = ROW, .type = GRUNT, .behavior = STRAFE_HORIZONTAL, .fired = FALSE, .px = 75, .py = 3, 
.strafe = 25, .trigger_time=270, .aimed = TRUE
    },
    [13] ={
.trigger = ROW, .type = GRUNT, .behavior = STRAFE_HORIZONTAL, .fired = FALSE, .px = 25, .py = 3, 
.strafe = 25, .trigger_time=270, .aimed = TRUE
    },
    [14] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 95, .py = 10, 
.strafe = 10, .trigger_time=280, .aimed = FALSE
    },
    [15] ={
.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE, .px = 5, .py = 10, 
.strafe = 10, .trigger_time=280, .aimed = FALSE
    },
    [16] ={
.trigger = ROW, .type = GRUNT, .behavior = STRAFE_VERTICAL, .fired = FALSE, .px = 85, .py = 10, 
.strafe = 11, .trigger_time=290, .aimed = FALSE
    },
    [17] ={
.trigger = ROW, .type = GRUNT, .behavior = STRAFE_VERTICAL, .fired = FALSE, .px = 15, .py = 10, 
.strafe = 10, .trigger_time=290, .aimed = FALSE
    },
    [18] ={
.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 30, .py = 3, 
.strafe = 20, .trigger_time=350, .aimed = FALSE
    },
    [19] ={
.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 20, .py = 3, 
.strafe = 20, .trigger_time=350, .aimed = FALSE
    },
    [20] ={
.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 10, .py = 3, 
.strafe = 20, .trigger_time=350, .aimed = FALSE
    },
    [21] ={
.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 1, .py = 3, 
.strafe = 20, .trigger_time=350, .aimed = FALSE
    },
    [22] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = STATIC, .fired = FALSE, .px = 40, .py = 3, 
.strafe = 20, .trigger_time=410, .aimed = TRUE
    },
    [23] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = STATIC, .fired = FALSE, .px = 60, .py = 3, 
.strafe = 20, .trigger_time=410, .aimed = TRUE
    },
    [24] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 10, .py = 2, 
.strafe = 20, .trigger_time=490, .aimed = FALSE
    },
    [25] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 6, .py = 2, 
.strafe = 20, .trigger_time=490, .aimed = FALSE
    },
    [26] ={
.trigger = ROW, .type = BOMBER, .behavior = STATIC, .fired = FALSE, .px = 55, .py = 3, 
.strafe = 20, .trigger_time=550, .aimed = FALSE
    },
    [27] ={
.trigger = ROW, .type = BOMBER, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 3, .py = 1, 
.strafe = 20, .trigger_time=600, .aimed = FALSE
    },
    [28] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 1, .py = 1, 
.strafe = 20, .trigger_time=600, .aimed = FALSE
    },
    [29] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 2, .py = 1, 
.strafe = 20, .trigger_time=600, .aimed = FALSE
    },
    [30] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 4, .py = 1, 
.strafe = 20, .trigger_time=600, .aimed = FALSE
    },
    [31] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 5, .py = 1, 
.strafe = 20, .trigger_time=600, .aimed = FALSE
    },
    [32] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEVERTICALLY, .fired = FALSE, .px = 10, .py = 1, 
.strafe = 20, .trigger_time=660, .aimed = TRUE
    },
    [33] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEVERTICALLY, .fired = FALSE, .px = 90, .py = 1, 
.strafe = 20, .trigger_time=660, .aimed = TRUE
    },
    [34] ={
.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE, .px = 5, .py = 1, 
.strafe = 20, .trigger_time=660, .aimed = TRUE
    },
}; */
spawn_entry spawn_table_1[]={

// ================= PHASE 1: Movement & basic dodging (rows 20-140) =================
// Static, unaimed grunts only. Teaches "bullets exist" with zero real pressure.

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 20, .py = 2, .strafe = 0, .trigger_time = 20, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 35, .py = 2, .strafe = 0, .trigger_time = 45, .aimed = FALSE},

// First moving enemy - horizontal sweep, still no aim
{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 5, .py = 3, .strafe = 0, .trigger_time = 75, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 15, .py = 2, .strafe = 0, .trigger_time = 100, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 30, .py = 0, .strafe = 0, .trigger_time = 100, .aimed = FALSE},

// ================= PHASE 2: Aimed fire introduced (rows 170-280) =================

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 25, .py = 2, .strafe = 0, .trigger_time = 170, .aimed = TRUE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 10, .py = 2, .strafe = 0, .trigger_time = 195, .aimed = TRUE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 45, .py = 3, .strafe = 0, .trigger_time = 220, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 5, .py = 4, .strafe = 0, .trigger_time = 220, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 30, .py = 2, .strafe = 0, .trigger_time = 250, .aimed = TRUE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 15, .py = 0, .strafe = 0, .trigger_time = 280, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 35, .py = 0, .strafe = 0, .trigger_time = 280, .aimed = FALSE},

// ================= PHASE 3: Rapidfire introduced (rows 330-450) =================

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STATIC, .fired = FALSE,
 .px = 20, .py = 2, .strafe = 0, .trigger_time = 330, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 10, .py = 2, .strafe = 0, .trigger_time = 330, .aimed = TRUE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 5, .py = 3, .strafe = 0, .trigger_time = 365, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 40, .py = 0, .strafe = 0, .trigger_time = 365, .aimed = FALSE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .fired = FALSE,
 .px = 25, .py = 2, .strafe = 10, .trigger_time = 400, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 15, .py = 2, .strafe = 0, .trigger_time = 400, .aimed = TRUE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 35, .py = 2, .strafe = 0, .trigger_time = 400, .aimed = TRUE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .fired = FALSE,
 .px = 10, .py = 1, .strafe = 8, .trigger_time = 450, .aimed = FALSE},

// ================= PHASE 4: First bomber + mixed pressure (rows 500-620) =================

{.trigger = ROW, .type = BOMBER, .behavior = STATIC, .fired = FALSE,
 .px = 25, .py = 2, .strafe = 0, .trigger_time = 500, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 5, .py = 3, .strafe = 0, .trigger_time = 500, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 45, .py = 4, .strafe = 0, .trigger_time = 530, .aimed = FALSE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STATIC, .fired = FALSE,
 .px = 15, .py = 2, .strafe = 0, .trigger_time = 560, .aimed = TRUE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STATIC, .fired = FALSE,
 .px = 30, .py = 2, .strafe = 0, .trigger_time = 560, .aimed = TRUE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 20, .py = 0, .strafe = 0, .trigger_time = 590, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 30, .py = 0, .strafe = 0, .trigger_time = 590, .aimed = FALSE},

{.trigger = ROW, .type = BOMBER, .behavior = STRAFE_HORIZONTAL, .fired = FALSE,
 .px = 20, .py = 2, .strafe = 10, .trigger_time = 620, .aimed = FALSE},

// ================= PHASE 5: Full remix - everything combined (rows 680-820) =================

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 10, .py = 2, .strafe = 0, .trigger_time = 680, .aimed = TRUE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 5, .py = 3, .strafe = 0, .trigger_time = 680, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 40, .py = 0, .strafe = 0, .trigger_time = 710, .aimed = FALSE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .fired = FALSE,
 .px = 25, .py = 2, .strafe = 10, .trigger_time = 740, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 15, .py = 2, .strafe = 0, .trigger_time = 740, .aimed = TRUE},

{.trigger = ROW, .type = BOMBER, .behavior = STATIC, .fired = FALSE,
 .px = 30, .py = 2, .strafe = 0, .trigger_time = 770, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 45, .py = 4, .strafe = 0, .trigger_time = 800, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 5, .py = 4, .strafe = 0, .trigger_time = 800, .aimed = FALSE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STATIC, .fired = FALSE,
 .px = 20, .py = 2, .strafe = 0, .trigger_time = 820, .aimed = TRUE},

// ================= PHASE 6: Final gauntlet before the boss (rows 880-1020) =================

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .fired = FALSE,
 .px = 15, .py = 1, .strafe = 8, .trigger_time = 880, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 25, .py = 2, .strafe = 0, .trigger_time = 880, .aimed = TRUE},

{.trigger = ROW, .type = GRUNT, .behavior = STATIC, .fired = FALSE,
 .px = 35, .py = 2, .strafe = 0, .trigger_time = 880, .aimed = TRUE},

{.trigger = ROW, .type = BOMBER, .behavior = STRAFE_VERTICAL, .fired = FALSE,
 .px = 25, .py = 1, .strafe = 8, .trigger_time = 920, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 10, .py = 0, .strafe = 0, .trigger_time = 950, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEVERTICALLY, .fired = FALSE,
 .px = 40, .py = 0, .strafe = 0, .trigger_time = 950, .aimed = FALSE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STATIC, .fired = FALSE,
 .px = 15, .py = 2, .strafe = 0, .trigger_time = 980, .aimed = TRUE},

{.trigger = ROW, .type = RAPIDFIRE, .behavior = STATIC, .fired = FALSE,
 .px = 30, .py = 2, .strafe = 0, .trigger_time = 980, .aimed = TRUE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 5, .py = 3, .strafe = 0, .trigger_time = 1020, .aimed = FALSE},

{.trigger = ROW, .type = GRUNT, .behavior = MOVEHORIZONTALLY, .fired = FALSE,
 .px = 45, .py = 3, .strafe = 0, .trigger_time = 1020, .aimed = FALSE},

// ================= Breather before the boss (rows 1020-1200) =================
// Deliberately empty - lets the player catch their breath and clear the field
// before the trigger_time = 1200 carrier boss spawn below.

// ================= BOSS =================

{.trigger = ROW, .type = CARRIER_BOSS, .behavior = CARRIER_SPECIAL, .fired = FALSE,
 .px = 50, .py = 6, .strafe = 0, .trigger_time = 1200, .aimed = TRUE},

};
const int spawn_table_1_count = sizeof(spawn_table_1) / sizeof(spawn_table_1[0]);
spawn_entry spawn_table_2[] = {
    // --- Rows 0-700: sparse intro ---
    //Test battleship spawn
   // { .trigger = ROW, .trigger_time = 0, .type = BATTLESHIP_BOSS, .behavior = BATTLESHIP_SPECIAL, .aimed = TRUE, .px = 50, .py = 6, .strafe = 20, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 30,  .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 8,  .py = 0, .strafe = 15, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 70,  .type = RAPIDFIRE,  .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 78, .py = 0, .strafe = 10, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 45, .py = 0, .strafe = 18, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 150, .type = LASER_ENEMY,.behavior = STATIC,            .aimed = TRUE,  .px = 25, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 190, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 62, .py = 0, .strafe = 14, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 230, .type = BOMBER,     .behavior = STATIC,            .aimed = FALSE, .px = 90, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 270, .type = GRUNT,      .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 15, .py = 0, .strafe = 12, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 310, .type = HUNTER,     .behavior = HUNT_PLAYER,       .aimed = FALSE, .px = 50, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 350, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 70, .py = 0, .strafe = 16, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 390, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 5,  .py = 0, .strafe = 10, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 430, .type = LASER_ENEMY,.behavior = STATIC,            .aimed = TRUE,  .px = 82, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 470, .type = RAPIDFIRE,  .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 38, .py = 0, .strafe = 11, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 510, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 95, .py = 0, .strafe = 12, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 550, .type = BOMBER,     .behavior = STATIC,            .aimed = FALSE, .px = 30, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 590, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 55, .py = 0, .strafe = 18, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 630, .type = HUNTER,     .behavior = HUNT_PLAYER,       .aimed = FALSE, .px = 12, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 670, .type = GRUNT,      .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 85, .py = 0, .strafe = 14, .fired = FALSE },

    // --- Row 750: Jet miniboss ---
    { .trigger = ROW, .trigger_time = 750, .type = JET_BOSS, .behavior = JET_SPECIAL, .aimed = TRUE, .px = 50, .py = 2, .strafe = 35, .fired = FALSE },

    // --- Rows 800-1450: mid-density ---
    { .trigger = ROW, .trigger_time = 820,  .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 10, .py = 0, .strafe = 15, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850,  .type = GRUNT,      .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 88, .py = 0, .strafe = 12, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 880,  .type = LASER_ENEMY,.behavior = STATIC,            .aimed = TRUE,  .px = 48, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 910,  .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 68, .py = 0, .strafe = 14, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 940,  .type = BOMBER,     .behavior = STATIC,            .aimed = FALSE, .px = 22, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 970,  .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 40, .py = 0, .strafe = 16, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1000, .type = HUNTER,     .behavior = HUNT_PLAYER,       .aimed = FALSE, .px = 75, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1030, .type = RAPIDFIRE,  .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 13, .py = 0, .strafe = 11, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1060, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 96, .py = 0, .strafe = 10, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1090, .type = LASER_ENEMY,.behavior = STATIC,            .aimed = TRUE,  .px = 60, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1120, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 33, .py = 0, .strafe = 17, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1150, .type = GRUNT,      .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 52, .py = 0, .strafe = 12, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1180, .type = BOMBER,     .behavior = STATIC,            .aimed = FALSE, .px = 18, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1210, .type = HUNTER,     .behavior = HUNT_PLAYER,       .aimed = FALSE, .px = 87, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1240, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 42, .py = 0, .strafe = 14, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1270, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 6,  .py = 0, .strafe = 15, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1300, .type = LASER_ENEMY,.behavior = STATIC,            .aimed = TRUE,  .px = 72, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1330, .type = RAPIDFIRE,  .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 28, .py = 0, .strafe = 11, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1360, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 64, .py = 0, .strafe = 16, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1390, .type = HUNTER,     .behavior = HUNT_PLAYER,       .aimed = FALSE, .px = 36, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1420, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 92, .py = 0, .strafe = 18, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1450, .type = GRUNT,      .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 58, .py = 0, .strafe = 10, .fired = FALSE },

    // --- Row 1500: Frigates ---
    { .trigger = ROW, .trigger_time = 1500, .type = FRIGATE1, .behavior = FRIGATE1_SPECIAL, .aimed = FALSE, .px = 35, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1500, .type = FRIGATE2, .behavior = FRIGATE2_SPECIAL, .aimed = FALSE, .px = 65, .py = 2, .strafe = 0, .fired = FALSE },

    // --- Rows 1550-2050: dense final gauntlet ---
    { .trigger = ROW, .trigger_time = 1570, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 14, .py = 0, .strafe = 18, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1600, .type = LASER_ENEMY,.behavior = STATIC,            .aimed = TRUE,  .px = 78, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1630, .type = HUNTER,     .behavior = HUNT_PLAYER,       .aimed = FALSE, .px = 46, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1660, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 8,  .py = 0, .strafe = 15, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1690, .type = RAPIDFIRE,  .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 85, .py = 0, .strafe = 12, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1720, .type = BOMBER,     .behavior = STATIC,            .aimed = FALSE, .px = 50, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1750, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 22, .py = 0, .strafe = 16, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1780, .type = HUNTER,     .behavior = HUNT_PLAYER,       .aimed = FALSE, .px = 70, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 35, .py = 0, .strafe = 17, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1840, .type = LASER_ENEMY,.behavior = STATIC,            .aimed = TRUE,  .px = 60, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1870, .type = GRUNT,      .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 10, .py = 0, .strafe = 11, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1900, .type = RAPIDFIRE,  .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 90, .py = 0, .strafe = 15, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1930, .type = HUNTER,     .behavior = HUNT_PLAYER,       .aimed = FALSE, .px = 42, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1960, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 18, .py = 0, .strafe = 14, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1990, .type = RAPIDFIRE,  .behavior = STRAFE_VERTICAL,   .aimed = FALSE, .px = 80, .py = 0, .strafe = 12, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2020, .type = LASER_ENEMY,.behavior = STATIC,            .aimed = TRUE,  .px = 55, .py = 0, .strafe = 0,  .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2050, .type = GRUNT,      .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 30, .py = 0, .strafe = 18, .fired = FALSE },

    // --- Row 2100: Battleship boss (placeholder, not yet implemented) ---
    { .trigger = ROW, .trigger_time = 1800, .type = BATTLESHIP_BOSS, .behavior = BATTLESHIP_SPECIAL, .aimed = TRUE, .px = 50, .py = 6, .strafe = 20, .fired = FALSE },
};
const int spawn_table_2_count = sizeof(spawn_table_2)/sizeof(spawn_table_2[0]);
// Stage 3 spawn table: 100x40 playfield, 3000 rows
// Bosses/minibosses go at rows 1000, 2000 and 3000 - placeholders below.
// Swap the placeholder comment for a real spawn_entry once the EnemyType exists
// (e.g. BATTLESHIP_BOSS at 3000; pick minibosses for 1000/2000 from your notes:
// Orbital Defense Station / Alien Hive / Gravity Well Generator / Drone Swarm
// Mothership / Corvette Pair / Pulse Cannon / Interceptor).
spawn_entry spawn_table_3[] = {
    //Test mothership spawn
    //{ .trigger = ROW, .trigger_time = 0, .type = MOTHERSHIP, .behavior = MOTHERSHIP_SPECIAL, .aimed = FALSE, .px = 50, .py = 6, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 10, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 12, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 10, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 14, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 10, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 16, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 10, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 18, .py = 0.0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 10, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 20, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 10, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 22, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 10, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 24, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 42, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 42, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 42, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 46, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 46, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 46, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 50, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 50, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 110, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 50, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 22, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 25, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 28, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 31, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 34, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 200, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 24, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 200, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 28, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 200, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 32, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 80, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 80, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 80, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 84, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 84, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 84, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 88, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 88, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 280, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 88, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 63, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 66, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 72, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 75, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 370, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 65, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 370, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 69, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 370, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 73, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 450, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 20, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 450, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 23, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 450, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 26, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 450, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 29, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 450, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 32, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 450, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 22, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 450, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 26, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 450, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 30, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 530, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 73, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 530, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 76, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 530, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 79, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 530, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 82, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 530, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 85, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 530, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 75, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 530, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 79, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 530, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 83, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 610, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 34, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 610, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 36, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 610, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 38, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 610, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 40, .py = 0.0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 610, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 42, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 610, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 44, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 610, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 46, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 710, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 57, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 712, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 32, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 713, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 24, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 714, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 61, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 715, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 93, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 716, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 79, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 717, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 58, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 718, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 39, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 719, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 47, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 720, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 4, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 36, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 39, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 42, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 45, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 48, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 38, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 42, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 850, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 46, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 930, .type = GRUNT, .behavior = STATIC, .aimed = FALSE, .px = 17, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 960, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 42, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 960, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 45, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 960, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 48, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 960, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 51, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 960, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 54, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 960, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 44, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 960, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 48, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 960, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 52, .py = 3, .strafe = 6, .fired = FALSE },
    // ---- STAGE 3 MINIBOSS 1: 2 special jet and three corvettes
    { .trigger = ROW, .trigger_time = 1000, .type = JET_BOSS, .behavior = JET_SPECIAL, .aimed = TRUE, .px = 60, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1000, .type = CORVETTE, .behavior = STRAFE_HORIZONTAL, .aimed = TRUE, .px = 50, .py = 5, .strafe = 80, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1000, .type = CORVETTE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 60, .py = 5, .strafe = 80, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1000, .type = CORVETTE_BOMBER, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 40, .py = 5, .strafe = 80, .fired = FALSE },
    // End of boss wave 1
    { .trigger = ROW, .trigger_time = 1010, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1010, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1010, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1010, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 73, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1010, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 73, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1010, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 73, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1010, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 77, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1010, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 77, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1010, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 77, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1100, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 57, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1100, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 59, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1100, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 61, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1100, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 63, .py = 0.0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1100, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 65, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1100, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 67, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1100, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 46, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 49, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 52, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 55, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1200, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 58, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1200, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 48, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1200, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 52, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1200, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 56, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 57, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 57, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 57, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 61, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 61, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 61, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 65, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 65, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1280, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 65, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 17, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 19, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1370, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 21, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1370, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 23, .py = 0.0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1370, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 25, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 27, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1370, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 29, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1470, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 38, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1470, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 41, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1470, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 44, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1470, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 47, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1470, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 50, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1470, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 40, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1470, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 44, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1470, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 48, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 21, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 21, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 21, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 25, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 25, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 25, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 29, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 29, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1550, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 29, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1640, .type = GRUNT, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 52, .py = 0, .strafe = 7, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1670, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 89, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1672, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 38, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1673, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 81, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1674, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 93, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1675, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 85, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1676, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 91, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1677, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 25, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1678, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 86, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1679, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 72, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1680, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 13, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 31, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 31, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 31, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 35, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 35, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 35, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 39, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 39, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1810, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 39, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1900, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 43, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1900, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 45, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1900, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 47, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1900, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 49, .py = 0.0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1900, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 51, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1900, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 53, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 1900, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 55, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    // ---- STAGE 3 MINIBOSS 2: One flying fortress, one jet, two corvettes
    { .trigger = ROW, .trigger_time = 2000, .type = FLYFORT_BOSS, .behavior = FLYFORT_SPECIAL, .aimed = FALSE, .px = 50, .py = 2, .strafe = 80, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2000, .type = CORVETTE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 60, .py = 5, .strafe = 80, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2000, .type = CORVETTE_BOMBER, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 40, .py = 5, .strafe = 80, .fired = FALSE },
    //End of 2nd boss wave
    { .trigger = ROW, .trigger_time = 2010, .type = GRUNT, .behavior = STATIC, .aimed = FALSE, .px = 91, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2040, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 49, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2040, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 52, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2040, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 55, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2040, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 58, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2040, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 61, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2040, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 51, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2040, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 55, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2040, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 59, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2120, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 17, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2120, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 19, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2120, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 21, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2120, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 23, .py = 0.0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2120, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 25, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2120, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 27, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2120, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 29, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2220, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 49, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2220, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 52, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2220, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 55, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2220, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 58, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2220, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 61, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2220, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 51, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2220, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 55, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2220, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 59, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2300, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 72, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2300, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 75, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2300, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 78, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2300, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 81, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2300, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 84, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2300, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 74, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2300, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 78, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2300, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 82, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2380, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 67, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2380, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2380, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 71, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2380, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 73, .py = 0.0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2380, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 75, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2380, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 77, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2380, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 79, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2480, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 26, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2480, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 29, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2480, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 32, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2480, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 35, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2480, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 38, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2480, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 28, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2480, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 32, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2480, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 36, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2560, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 77, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2560, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 80, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2560, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 83, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2560, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 86, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2560, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 89, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2560, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 79, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2560, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 83, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2560, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 87, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2640, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 78, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2642, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 55, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2643, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 67, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2644, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 50, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2645, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 15, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2646, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 32, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2647, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 10, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2648, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 21, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2649, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = TRUE, .px = 18, .py = 0, .strafe = 8, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2650, .type = HUNTER, .behavior = HUNT_PLAYER, .aimed = TRUE, .px = 69, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2780, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 29, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2780, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 32, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2780, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 35, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2780, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 38, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2780, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 41, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2780, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 31, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2780, .type = RAPIDFIRE, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 35, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2780, .type = BOMBER, .behavior = STRAFE_VERTICAL, .aimed = FALSE, .px = 39, .py = 3, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 65, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 65, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 65, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 69, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 73, .py = 0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = RAPIDFIRE, .behavior = STATIC, .aimed = FALSE, .px = 73, .py = 2, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2860, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 73, .py = 4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2950, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 58, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2950, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 60, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2950, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 62, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2950, .type = BOMBER, .behavior = STATIC, .aimed = FALSE, .px = 64, .py = 0.0, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2950, .type = RAPIDFIRE, .behavior = STRAFE_HORIZONTAL, .aimed = FALSE, .px = 66, .py = 1.2, .strafe = 6, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2950, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 68, .py = 2.4, .strafe = 0, .fired = FALSE },
    { .trigger = ROW, .trigger_time = 2950, .type = REFLECTOR, .behavior = STATIC, .aimed = FALSE, .px = 70, .py = 3.5999999999999996, .strafe = 0, .fired = FALSE },
  // ---- STAGE 3 BOSS (Mothership)
    { .trigger = ROW, .trigger_time = 3000, .type = MOTHERSHIP, .behavior = MOTHERSHIP_SPECIAL, .aimed = FALSE, .px = 50, .py = 6, .strafe = 0, .fired = FALSE },
};
const int spawn_table_3_count = sizeof(spawn_table_3) / sizeof(spawn_table_3[0]);
spawn_entry spawn_table_4[] = {};
const int spawn_table_4_count = sizeof(spawn_table_4) / sizeof(spawn_table_4[0]);