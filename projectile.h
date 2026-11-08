#include "gamedata.h"
#include "enemy.h"
#ifndef PROJECTILE_H
#define PROJECTILE_H
#define MAX_PROJECTILES 2000
extern Projectile projectiles_backup[MAX_ENEMIES];
extern Projectile projectiles[MAX_PROJECTILES];
extern  WeaponType autopistol;
extern  WeaponType machinegun;
extern  WeaponType laserrifle;
extern  WeaponType bomblauncher;
extern  WeaponType plasmarifle;
extern  WeaponType missilelauncher;
extern  WeaponType empbomb;
extern  WeaponType lightning;
extern  WeaponType shotgun;
extern  WeaponType lasercannon;
extern  WeaponType plasmacannon;
extern  WeaponType GRUNT_RIFLE;
extern  WeaponType RAPIDFIRE_RIFLE;
extern  WeaponType LASER_RIFLE_ENEMY;
extern  WeaponType BOMB_ENEMY_WEAPON;
extern  WeaponType HUNTER_RIFLE;
extern  WeaponType JET_CANNON;
extern  WeaponType FLYFORT_CANNON;
extern  WeaponType spiral_cannon;
extern  WeaponType CARRIER_CANNON;
extern  WeaponType CARRIER_FLAK;
extern  WeaponType FRIGATE_FLAK;
extern  WeaponType FRIGATE_LASER;
extern  WeaponType Minigun;
extern  WeaponType Grand_Cannon;
extern  WeaponType Plasma_Storm;
extern  WeaponType missilestorm;
void fire_weapon(const WeaponType *weapon, float px, float py, float angle, bool player_owned);
const WeaponType* get_weapon_template(WeaponID weapon_id);
int findfreeprojectileslot(void);
void move_projectiles(Projectile *projectiles, int max_x, int max_y);
void render_projectiles(Projectile *projectiles);
void erase_projectiles(Projectile *projectiles);
#endif