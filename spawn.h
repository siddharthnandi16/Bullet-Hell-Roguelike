#ifndef  SPAWN_H
#define SPAWN_H
typedef struct {
TriggerType trigger;
EnemyType type;
EnemyBehavior behavior;
 //Stores whether the spawns has already occured
 bool fired;
float px;
float py;
float strafe;
int trigger_time; 
bool aimed;
}spawn_entry;
extern spawn_entry spawn_table_1[];
extern const int spawn_table_1_count;
extern spawn_entry spawn_table_2[];
extern const int spawn_table_2_count;
extern spawn_entry spawn_table_3[];
extern const int spawn_table_3_count;
extern spawn_entry spawn_table_4[];
extern const int spawn_table_4_count;

#endif