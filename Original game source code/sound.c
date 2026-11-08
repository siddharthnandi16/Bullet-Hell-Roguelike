#define MINIAUDIO_IMPLEMENTATION
#include <pdcurses.h>
#include <string.h>
#include  "gamedata.h"
#include "sound.h"
#include "miniaudio.h"
#include <stdio.h>
ma_engine engine;   //Initialisation for miniaudio engine
//Initalises the audio and returns whether it worked successfully
bool init_audio(void) {
    fprintf(stderr, "Initialising audio");
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
    fprintf(stderr, "Engine init failed: %d\n", result);
        return FALSE;
    }
    return TRUE;
}
//Shuts down the audio
void shutdown_audio(void) {
    ma_engine_uninit(&engine);
}
void handle_mute_toggle(void) {
    static bool m_was_down = false;
    static bool audio_muted = false;

    bool mute_key_down = (GetAsyncKeyState('M') & 0x8000) != 0;

    if (mute_key_down && !m_was_down) {
        audio_muted = !audio_muted;
        ma_engine_set_volume(&engine, audio_muted ? 0.0f : 1.0f);
    }
    m_was_down = mute_key_down;
}
/* Commented out due to being uneccessary and overcomplicated
//Stores all of the sounds used by the game
typedef struct Sound_Pool{
    char *path;
    int sound_effect_id;
    bool loop; 
    char *description;
    char *sound;
}Sound_Pool;
Sound_Pool sound_pool[MAX_SOUNDS] = {
[0] = {
    .path = NULL,
    .sound_effect_id = 0,
    .loop = FALSE,
    .description = "Placeholder.",
    .sound = "Placeholder."
};
}*/
//Table for storing sounds for level 1
sound_entry sound_table_1[]={
    [0] ={
.trigger = ROW, .is_song=TRUE, .loop = TRUE, .fired = FALSE, .trigger_time = 0, .sound_effect_id = 0,
    }
};

ma_sound loaded_sounds[MAX_SOUNDS] = {};
// Old version of the function. Stored as a restore point in case something goes wrong
void InitialiseSoundEffects(ma_sound *loaded_sounds){ 
    ma_result Titlescreen_BGM_result = ma_sound_init_from_file(
    &engine,
    "Mercury.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Titlescreen_MUSIC]
);
ma_sound_set_looping(&loaded_sounds[Titlescreen_MUSIC], TRUE);
    ma_result level_1_BGM_result = ma_sound_init_from_file(
    &engine,
    "Mars.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Level_1]
);
ma_sound_set_looping(&loaded_sounds[Level_1], TRUE);
ma_sound_set_looping(&loaded_sounds[Titlescreen_MUSIC], TRUE);
   ma_result Level_2_result = ma_sound_init_from_file(
    &engine,
    "Venus.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Level_2]
);
ma_sound_set_looping(&loaded_sounds[Level_2], TRUE);
ma_result Level_3_result = ma_sound_init_from_file(
    &engine,
    "BossMain.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Level_3]
);
ma_sound_set_looping(&loaded_sounds[Level_3], TRUE);
    ma_result BossBGM_result = ma_sound_init_from_file(
    &engine,
    "BossMain.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Boss_Theme]
);
  ma_result BossIntro_result = ma_sound_init_from_file(
    &engine,
    "BossIntro.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Boss_Intro]
);
    ma_result MachineGun_result = ma_sound_init_from_file(
    &engine,
    "D_24P.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Machine_Gun]
);
    ma_result Laser_result = ma_sound_init_from_file(
    &engine,
    "sfx_wpn_laser11.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Laser_sound]
);
    ma_result Bomb_result = ma_sound_init_from_file(
    &engine,
    "sfx_exp_medium2.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Bomb_sound]
);
    ma_result Missile_result = ma_sound_init_from_file(
    &engine,
    "sfx_wpn_missilelaunch.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Missile_sound]
);
    ma_result Player_life_lost_result = ma_sound_init_from_file(
    &engine,
    "sfx_deathscream_alien4.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Player_life_lost]
);
    ma_result Enemy_down_result = ma_sound_init_from_file(
    &engine,
    "sfx_deathscream_robot3.wav",
    0,
    NULL,
    NULL,
    &loaded_sounds[Enemy_down]
);

}  
//Plays and updates sound effects, redudant since miniaudio already does this
void PlaySoundEffect(ma_sound *sound){
ma_sound_start(sound);
}
//Example of how to call the above function
//PlaySoundEffect(&loaded_sounds[Titlescreen]);