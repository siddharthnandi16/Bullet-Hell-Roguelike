#include "gamedata.h"
#include "enemy.h"
#ifndef PROJECTILE_H
#define PROJECTILE_H
#define MAX_PROJECTILES 2000
extern Projectile projectiles_backup[MAX_ENEMIES];
extern Projectile projectiles[MAX_PROJECTILES];
extern const WeaponType autopistol;
extern const WeaponType machinegun;
extern const WeaponType laserrifle;
extern const WeaponType bomblauncher;
extern const WeaponType plasmarifle;
extern const WeaponType missilelauncher;
extern const WeaponType empbomb;
extern const WeaponType lightning;
extern const WeaponType shotgun;
extern const WeaponType lasercannon;
extern const WeaponType plasmacannon;
extern const WeaponType GRUNT_RIFLE;
extern const WeaponType RAPIDFIRE_RIFLE;
extern const WeaponType LASER_RIFLE_ENEMY;
extern const WeaponType BOMB_ENEMY_WEAPON;
extern const WeaponType HUNTER_RIFLE;
extern const WeaponType JET_CANNON;
extern const WeaponType FLYFORT_CANNON;
extern const WeaponType spiral_cannon;
extern const WeaponType CARRIER_CANNON;
extern const WeaponType CARRIER_FLAK;
extern const WeaponType FRIGATE_FLAK;
extern const WeaponType FRIGATE_LASER;
extern const WeaponType Minigun;
extern const WeaponType Grand_Cannon;
extern const WeaponType Plasma_Storm;
extern const WeaponType missilestorm;
void fire_weapon(const WeaponType *weapon, float px, float py, float angle, bool player_owned);
const WeaponType* get_weapon_template(WeaponID weapon_id);
int findfreeprojectileslot(void);
void move_projectiles(Projectile *projectiles, int max_x, int max_y);
void render_projectiles(Projectile *projectiles);
void erase_projectiles(Projectile *projectiles);
#endif