#include <pdcurses.h>
#include <math.h>
#include <stdlib.h>
#include "gamedata.h"
#include  "enemy.h"
#include "projectile.h"
#include "sound.h"
#include "miniaudio.h"
#define MAX_PROJECTILES 2000
#define PROJ_HITBOX_MARGIN 1 //Degree of leniency for player projectiles
int Check_Collisions(Player *player, Enemy *enemies, Projectile *projectiles){
//First loop for enemy-projectile collisions. Enemies only collide with player projectiles
for (int i = 0; i < MAX_ENEMIES; i++){
    for (int p = 0; p < MAX_PROJECTILES; p++){
        if (projectiles[p].player_owned == TRUE && projectiles[p].state != SPENT && enemies[i].state == ALIVE){
  if (enemies[i].shape == NULL){    
int y_min = (int)fminf(projectiles[p].old_py, projectiles[p].py) -PROJ_HITBOX_MARGIN;
int y_max = (int)fmaxf(projectiles[p].old_py, projectiles[p].py) +PROJ_HITBOX_MARGIN;
int x_min = (int)fminf(projectiles[p].old_px, projectiles[p].px) -PROJ_HITBOX_MARGIN;
int x_max = (int)fmaxf(projectiles[p].old_px, projectiles[p].px) +PROJ_HITBOX_MARGIN;
//Handling non-laser projectile types
    if(projectiles[i].type != LASER){
if ( abs((int)enemies[i].px - (int)projectiles[p].px) <= PROJ_HITBOX_MARGIN &&
    (int)enemies[i].py >= y_min && (int)enemies[i].py <= y_max
&& (int)enemies[i].px >= x_min && (int)enemies[i].px <= x_max){
if (enemies[i].type == REFLECTOR) {
    projectiles[p].player_owned = FALSE;   
    projectiles[p].color = 3;
    projectiles[p].dx = -projectiles[p].dx;
    projectiles[p].dy = -projectiles[p].dy;
    projectiles[p].old_px = enemies[i].px;
    projectiles[p].old_py = enemies[i].py;
    projectiles[p].angle += 180.0f;
    enemies[i].hp = enemies[i].hp - projectiles[p].damage;
    continue;
}
enemies[i].hp = enemies[i].hp - projectiles[p].damage;
projectiles[p].pierce--;
if (projectiles[p].type == BOMB){
    projectiles[p].state = EXPLODING;
}
    }
    
   if(enemies[i].hp <= 0){
enemies[i].state = DEAD;
player->score++;
};
}
//Special code for laser-type projectiles
if(projectiles[p].type == LASER && projectiles[p].state != SPENT){
if(abs((int)enemies[i].px - (int)projectiles[p].px)  <= 1){
    enemies[i].hp = enemies[i].hp - projectiles[p].damage;
projectiles[p].pierce--;
 if(enemies[i].hp <= 0){
enemies[i].state = DEAD;
player->score++;
};
}
}
}
//Collision block for multi tile enemies
if (enemies[i].shape != NULL){
    if (boss_invulnerable == TRUE && enemies[i].is_boss_part > 0){
        continue;
    }
if(projectiles[p].type != LASER){
//Non-lenient collision for multi tile enemies
int y_min_2 = (int)fminf(projectiles[p].old_py, projectiles[p].py);
int y_max_2 = (int)fmaxf(projectiles[p].old_py, projectiles[p].py);
int x_min_2 = (int)fminf(projectiles[p].old_px, projectiles[p].px);
int x_max_2 = (int)fmaxf(projectiles[p].old_px, projectiles[p].px);
int enemy_left   = enemies[i].px;
int enemy_right  = enemies[i].px + enemies[i].shape->width - 1;
int enemy_top    = enemies[i].py;
int enemy_bottom = enemies[i].py + enemies[i].shape->height - 1;
if(enemy_left <= x_max_2 && enemy_right >= x_min_2
&& enemy_top <= y_max_2 && enemy_bottom >= y_min_2){
//Code to check if point hit was a blank space
int steps = 4; // tune based on how fast your fastest projectile moves per frame
bool hit_solid = false;
for (int s = 0; s <= steps && !hit_solid; s++) {
    float t = (float)s / steps;
    float sample_px = projectiles[p].old_px + t * (projectiles[p].px - projectiles[p].old_px);
    float sample_py = projectiles[p].old_py + t * (projectiles[p].py - projectiles[p].old_py);

    int local_col = (int)sample_px - (int)enemies[i].px;
    int local_row = (int)sample_py - (int)enemies[i].py;

    if (local_row >= 0 && local_row < enemies[i].shape->height &&
        local_col >= 0 && local_col < enemies[i].shape->width) {
        if (enemies[i].shape->glyph_rows[local_row][local_col] != ' ') {
            hit_solid = true;
        }
    }
}
if (hit_solid) {
    ma_sound_start(&loaded_sounds[Enemy_down]);
 enemies[i].hp = enemies[i].hp - projectiles[p].damage;
projectiles[p].pierce--;

 if(enemies[i].hp <= 0){
enemies[i].state = DEAD;
player->score += 5;
if (enemies[i].is_boss_part == TRUE) player->score += 20;
if(enemies[i].is_boss_core == TRUE){
    player->score += 50;
    current_level++;
    Level_Complete = TRUE;
}
 }
}
}
}
if(projectiles[p].type == LASER && projectiles[p].state != SPENT){
    ma_sound_start(&loaded_sounds[Enemy_down]);
if(abs((int)enemies[i].px - (int)projectiles[p].px)  <= 1){
    enemies[i].hp = enemies[i].hp - projectiles[p].damage;
projectiles[p].pierce--;
 if(enemies[i].hp <= 0){
enemies[i].state = DEAD;
player->score += 5;
};
}
}
}
}    
}
}
//Second loop for player-projectile collisions
if (player->invuln_frames > 0){player->invuln_frames--;}
for (int p = 0; p < MAX_PROJECTILES; p++){
if (projectiles[p].player_owned == FALSE && projectiles[p].state != SPENT){
   int y_min = (int)fminf(projectiles[p].old_py, projectiles[p].py);
int y_max = (int)fmaxf(projectiles[p].old_py, projectiles[p].py);
int x_min = (int)fminf(projectiles[p].old_px, projectiles[p].px);
int x_max = (int)fmaxf(projectiles[p].old_px, projectiles[p].px);
if(projectiles[p].type == LASER){
   
if((int)player->px == (int)projectiles[p].px && player->invuln_frames <= 0){
     ma_sound_start(&loaded_sounds[Player_life_lost]);
player->lives--;
 player->invuln_frames = 60;  
projectiles[p].pierce--;
}
}
else {
if ((int)player->px == (int)projectiles[p].px &&
    (int)player->py >= y_min && (int)player->py <= y_max
&& (int)player->px >= x_min && (int)player->px <= x_max
&& player->invuln_frames <= 0){
    ma_sound_start(&loaded_sounds[Player_life_lost]);
    player->lives--;
    player->invuln_frames = 60;  
    projectiles[p].pierce--;
}
}
}
}
//Third loop for player-enemy collisions
for (int i = 0; i < MAX_ENEMIES; i++){
    if (enemies[i].state == ALIVE){
        if (enemies[i].px == player->px && enemies[i].py == player->py
             && player->invuln_frames <= 0){
            ma_sound_start(&loaded_sounds[Player_life_lost]);
             player->lives--;
    player->invuln_frames = 60; 
        }
    }
}
return 1;
}