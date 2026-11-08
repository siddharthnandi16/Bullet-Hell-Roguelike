#ifndef GAMEDATA_H
#define GAMEDATA_H
//Defines for current size of playfield
#define PLAYFIELD_W 100
#define PLAYFIELD_H 26
//Stores current game tick
extern int unsigned long tick;
//Stores current level
extern int current_level;
// This header file stores data about the game's current state
extern bool moving_diagonally;
//Enums for storing game modes
typedef enum game_mode{STORY_MODE, ENDLESS_MODE,LEVEL_SELECT, HIGH_SCORES, MUSIC_ROOM, MODE_QUIT}GameMode;
//Enums for types and states of projectiles
typedef enum proj_type{BULLET,LASER,BOMB,MISSILE,PLASMA,EMP,CHAINLIGHTNING}ProjType;
typedef enum proj_state{NORMAL,SPENT,EXPLODING}ProjState;
//This struct stores data on the projectile
typedef struct Projectile{
    float px,py;
    float old_px, old_py;
    float dx,dy;
    char symbol;
    int width, height;
    int damage; //Determines HP damage dealt
    int age; //Used for bomb detionation timers
    int pierce; //Tells you how many enemies a projectile can pierce before being spent
    int color; // Is 3 for enemy projectiles and 4 for player projectiles
    float strafe; //Used for special bullets that travel in sine-wave pattern
    float turn_rate; //Used for homing projectiles
    float angle;
    ProjType type; //Used for storing the type of weapon this is 
    ProjState state;
    bool player_owned;
}Projectile;
//This struct defines the behavior of weapons which fire projectiles
typedef enum weapon_id{EMPTY_ID, AUTOPISTOL_ID, MACHINEGUN_ID, LASRIFLE_PLAYER_ID, BOMB_PLAYER_ID,
PLASMARIFLE_PLAYER_ID,MISSILE_PLAYER_ID, EMP_ID, LIGHTNING_ID, SHOTGUN_ID, 
GRUNT_WEAPON_ID, LASERCANNON_ID, PLASMACANNON_ID, RAPIDFIRE_RIFLE_ID, LASER_RIFLE_ENEMY_ID,
 BOMB_ENEMY_ID, HUNTER_RIFLE_ID, JET_CANNON_ID, FLYFORT_CANNON_ID, SPIRAL_CANNON_ID,
CARRIER_CANNON_ID, CARRIER_FLAK_ID, FRIGATE_FLAK_ID, FRIGATE_LASER_ID,
MINIGUN_ID, GRAND_CANNON_ID, PLASMA_STORM_ID, MISSILE_STORM_ID}WeaponID;
typedef enum modes{REGULAR, BURST_FIRE, RAPID_FIRE, SUPERCHARGE, CHARGING}FireModes;
const typedef struct WeaponType{
    char *display_name;
    int cooldown_frames;
    int number; //Number of projectiles fired
    float angle; //Angle at which main projectile is fired
    float offset_angle; //Angle at which secondary projectiles are offset from main projectile
    bool omnidirectional; //Checks whether the weapon fires in all directions
    WeaponID weapon_id;
    ProjType type;
    FireModes modes;
}WeaponType;
typedef enum PlayerClass{PLAYER_DEBUG, PLAYER_FIGHTER, PLAYER_FORTRESS, PLAYER_EXPERIMENTAL}PlayerClass;
typedef struct Player{
    float px, py; //Stores the player's position
    float vx, vy; // Stores the player's standard movement speed
    float dx, dy; //Stores the player's current movement direction and speed
    int alive; //Stores whether the player is alive or dead, 1=alive, 0=dead
    int lives; //Stores the number of lives of the player
    bool hasLasers, hasBombs; // Stores whether the player has access to special weapons
    int fire_rate; // Will affect fire rate later on when upgrades are added, currently unused
    char symbol;
    bool speed_mode_fast ; //Making this false halves player movement
    bool q_was_down; //Checks whether the speed-toggle was held down last frame and prevents it from activating if it was
    WeaponID weapon_id;
    WeaponID inventory[10]; 
    int invuln_frames; //Period of invulnerability granted after taking a hit
    int score; //Increments upon destroyed enemies
    enum PlayerClass class;
}Player;
extern Player player;
extern Player fighter_jet;
extern Player flying_fortress;
extern Player experimental_fighter;
extern Player player_backup;
#define MAX_TILE_HEIGHT 26 //Max height of multi-tile enemies
#define MAX_TILE_WIDTH  80 //Max width of multi-tile enemies
typedef struct TileLayout {
    int width, height;                          // Dimensions of the shape
    const char *glyph_rows[MAX_TILE_HEIGHT];     // ASCII art, one string per row
    const int  *color_rows[MAX_TILE_HEIGHT];     // Parallel color_pair IDs, one int array per row
} TileLayout;
typedef enum trigger{ROW, TICK} TriggerType; //Stores whether an event is triggered by rows_scrolled or ticks that have passed
typedef enum type{ GRUNT, RAPIDFIRE, LASER_ENEMY, BOMBER, HUNTER, JET, FLYING_FORTRESS,
     LASER_JET, CARRIER_BOSS, CARRIER_BOSS_FLAK, CARRIER_BOSS_BOMB, FRIGATE1, FRIGATE2, JET_BOSS,
    BATTLESHIP_BOSS, REFLECTOR, BARRIER, FLYFORT_BOSS, CORVETTE, CORVETTE_BOMBER, MOTHERSHIP } EnemyType;
typedef enum state{INACTIVE, DEAD, ALIVE, DYING } EnemyState;
typedef enum behavior{ STATIC, MOVEHORIZONTALLY, MOVEVERTICALLY, STRAFE_HORIZONTAL,
     STRAFE_VERTICAL, HUNT_PLAYER, ZIGZAG, HUNT_PLAYER_FAR, CARRIER_SPECIAL, 
    FRIGATE1_SPECIAL, FRIGATE2_SPECIAL, JET_SPECIAL, BATTLESHIP_SPECIAL,
FLYFORT_SPECIAL, MOTHERSHIP_SPECIAL } EnemyBehavior;
typedef struct Enemy{
float px, py; //Stores current position
float dx, dy; //Stores the enemy's speed
int hp; //Stores the enemy's HP value. If this is zero, its state changes to dead
char symbol; //Stores what ASCII glyph the enemy is represented by
int width, height; //Stores the width and height of the enemy's bounding box
int cooldown_frames; //Controls rate of fire
EnemyType type;
EnemyState state;
EnemyBehavior behavior;
TileLayout *shape; //Shape of enemy. Is always NULL for single-tile enemies
int age; //Increments every frame, determines some enemy behaviors
float anchor_px, anchor_py; //Sets an "anchor point" for strafing enemies
float strafe; //Sets how far a strafing enemy is willing to go from their anchor point
float old_px, old_py; //Stores old positions of enemy for purpose of drawing, now redunant
float fire_px, fire_py; //Stores firing positions for multi-tile enemies
 WeaponType *weapon; //Tells the weapon firing function what weapon to use
 bool aimed; //If true, the enemy aims in the player's general direction
 int is_boss_part; //0 for all non boss enemies
 int is_boss_core; //0 for all non boss enemies
}Enemy;
extern Enemy enemies[];
//Enemy config struct, used for spawning enemies in endless mode
typedef struct {
    EnemyType type;
    EnemyBehavior behavior;
    float strafe;
    bool aimed;
} EnemyConfig;
//Enum for boss states
typedef enum {BOSS_NORMAL, TELEGRAPHING, SPECIAL_ATTACK_1, SPECIAL_ATTACK_2, 
SPECIAL_ATTACK_3, SPECIAL_ATTACK_4, SPECIAL_ATTACK_5, INVULN, BOSS_DYING}BossState;
extern BossState state;
extern int boss_state_timer;
extern bool boss_invulnerable;
extern bool boss_fight_ongoing;
extern bool Level_Complete;
extern char continue_choice;
extern bool continued; //Meant to gate access to a secret level that will be added later
extern GameMode game_mode;
#endif