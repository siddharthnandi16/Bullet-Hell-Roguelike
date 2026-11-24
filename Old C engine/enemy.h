#ifndef ENEMY_H
#define ENEMY_H
#define MAX_ENEMIES 250
//Backup of enemy spawning pool 
extern Enemy enemies_backup[MAX_ENEMIES];   
int findfreeslot(void);
const Enemy* get_template(EnemyType type);
void spawn_enemy(EnemyType type, EnemyBehavior behavior, bool aimed, float px, float py, float strafe);
void move_enemy(Enemy *enemies, Player * player, int max_x, int max_y);
void erase_enemies(Enemy *enemies);
void render_enemies(Enemy *enemies);
void fire_enemies(Enemy *enemies, Player *player);
#endif