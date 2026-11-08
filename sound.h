#ifndef SOUND_H
#define SOUND_H
#define MAX_SOUNDS 200
#include "miniaudio.h"
extern ma_sound loaded_sounds[MAX_SOUNDS];
extern ma_engine engine;
typedef enum sound_list{
Titlescreen_MUSIC,
Level_1,
Level_2,
Level_3,
Level_4,
Level_5,
Level_6,
Level_Secret,
Level_Endless,
Level_Start_Fanfare,
Boss_Intro,
Boss_Theme,
Miniboss_Theme,
Miniboss_Intro,
Engine_Hum,
Machine_Gun,
Laser_sound,
Bomb_sound,
Missile_sound,
Player_life_lost,
Enemy_down
}sound_list;
typedef struct {
TriggerType trigger;
bool is_song; //True for songs, false for sound effects
bool loop; //Determines whether the song will loop or not, TRUE if it loops
bool fired;
int trigger_time; 
int sound_effect_id; //All sounds effects in the game will be given a unique ID
}sound_entry;
extern sound_entry sound_table_1[];
bool init_audio(void);
void shutdown_audio(void);
void InitialiseSoundEffects(ma_sound *loaded_sounds);
void PlaySoundEffect(ma_sound *sound); 
void handle_mute_toggle(void);
#endif