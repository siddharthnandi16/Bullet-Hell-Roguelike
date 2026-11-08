#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include  "gamedata.h"
#include <string.h>
#include "parser.h"

WeaponType weapons[MAX_WEAPON_TYPES] = {0};
int weapon_count = 0;
//Various helper functions used in the weapon loading function
static int LookupEnum(const EnumEntry *table, int count, const char *s, int fallback) {
    if (s == NULL) return fallback;
    for (int i = 0; i < count; i++) {
        if (strcmp(s, table[i].name) == 0) return table[i].value;
    }
    fprintf(stderr, "[%s]len=%zu\n", s, strlen(s));
    fprintf(stderr, "Unknown enum string: %s\n", s);
    return fallback;
}

ProjType StringToProjType(const char *s) {
    static const EnumEntry table[] = {
        {"BULLET", BULLET}, {"LASER", LASER}, {"BOMB", BOMB},
        {"MISSILE", MISSILE}, {"PLASMA", PLASMA}, {"EMP", EMP},
        {"CHAINLIGHTNING", CHAINLIGHTNING}
    };
    return (ProjType)LookupEnum(table, sizeof table / sizeof table[0], s, BULLET);
}

FireModes StringToFireModes(const char *s) {
    static const EnumEntry table[] = {
        {"REGULAR",REGULAR}, {"BURST_FIRE", BURST_FIRE},
        {"RAPID_FIRE", RAPID_FIRE}, {"SUPERCHARGE", SUPERCHARGE},
        {"CHARGING", CHARGING}
    };
    return (FireModes)LookupEnum(table, sizeof table / sizeof table[0], s, REGULAR);
}

WeaponID StringToWeaponID(const char *s) {
    static const EnumEntry table[] = {
        {"EMPTY_ID", EMPTY_ID}, {"AUTOPISTOL_ID", AUTOPISTOL_ID},
        {"MACHINEGUN_ID", MACHINEGUN_ID}, {"LASRIFLE_PLAYER_ID", LASRIFLE_PLAYER_ID},
        {"BOMB_PLAYER_ID", BOMB_PLAYER_ID}, {"PLASMARIFLE_PLAYER_ID", PLASMARIFLE_PLAYER_ID},
        {"MISSILE_PLAYER_ID", MISSILE_PLAYER_ID}, {"EMP_ID", EMP_ID},
        {"LIGHTNING_ID", LIGHTNING_ID}, {"SHOTGUN_ID", SHOTGUN_ID},
        {"GRUNT_WEAPON_ID", GRUNT_WEAPON_ID}, {"LASERCANNON_ID", LASERCANNON_ID},
        {"PLASMACANNON_ID", PLASMACANNON_ID}, {"RAPIDFIRE_RIFLE_ID", RAPIDFIRE_RIFLE_ID},
        {"LASER_RIFLE_ENEMY_ID", LASER_RIFLE_ENEMY_ID}, {"BOMB_ENEMY_ID", BOMB_ENEMY_ID},
        {"HUNTER_RIFLE_ID", HUNTER_RIFLE_ID}, {"JET_CANNON_ID", JET_CANNON_ID},
        {"FLYFORT_CANNON_ID", FLYFORT_CANNON_ID}, {"SPIRAL_CANNON_ID", SPIRAL_CANNON_ID},
        {"CARRIER_CANNON_ID", CARRIER_CANNON_ID}, {"CARRIER_FLAK_ID", CARRIER_FLAK_ID},
        {"FRIGATE_FLAK_ID", FRIGATE_FLAK_ID}, {"FRIGATE_LASER_ID", FRIGATE_LASER_ID},
        {"MINIGUN_ID", MINIGUN_ID}, {"GRAND_CANNON_ID", GRAND_CANNON_ID},
        {"PLASMA_STORM_ID", PLASMA_STORM_ID}, {"MISSILE_STORM_ID", MISSILE_STORM_ID}
    };
    return (WeaponID)LookupEnum(table, sizeof table / sizeof table[0], s, EMPTY_ID);
}
void ParseWeaponArray(cJSON *array, WeaponType *weapons, int *i, int max) {
    cJSON *item;
    cJSON_ArrayForEach(item, array) {
        if (*i >= max) break;
        WeaponType *w = &weapons[*i];

        cJSON *vname = cJSON_GetObjectItemCaseSensitive(item, "var_name");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "display_name");
        cJSON *cd   = cJSON_GetObjectItemCaseSensitive(item, "cooldown_frames");
        cJSON *num  = cJSON_GetObjectItemCaseSensitive(item, "number");
        cJSON *ang  = cJSON_GetObjectItemCaseSensitive(item, "angle");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(item, "type");
        cJSON *mode = cJSON_GetObjectItemCaseSensitive(item, "modes");
        cJSON *id   = cJSON_GetObjectItemCaseSensitive(item, "weapon_id");
        cJSON *off  = cJSON_GetObjectItemCaseSensitive(item, "offset_angle");
cJSON *omni = cJSON_GetObjectItemCaseSensitive(item, "omnidirectional");

        if (cJSON_IsString(vname)) w->var_name = strdup(vname->valuestring);
        if (cJSON_IsString(name)) w->display_name = strdup(name->valuestring);
        if (cJSON_IsNumber(cd))   w->cooldown_frames = cd->valueint;
        if (cJSON_IsNumber(num))  w->number = num->valueint;
        if (cJSON_IsNumber(ang))  w->angle = (float)ang->valuedouble;

        if (cJSON_IsString(type)) w->type = StringToProjType(type->valuestring);
        if (cJSON_IsString(mode)) w->modes = StringToFireModes(mode->valuestring);
        if (cJSON_IsString(id))   w->weapon_id = StringToWeaponID(id->valuestring);
        if (cJSON_IsNumber(off))  w->offset_angle = (float)off->valuedouble;
        if (cJSON_IsBool(omni))   w->omnidirectional = cJSON_IsTrue(omni);
        (*i)++;
    }
}
//Function to load weapons
void LoadWeapons(){
FILE* weapon_file = fopen("Data/weapons.json", "rb");
if(weapon_file == NULL) {
        fprintf(stderr, "ERROR:Weapon file not opened!");
        return;   
    }
fseek(weapon_file, 0, SEEK_END); 
long int size = ftell(weapon_file);
 rewind(weapon_file);
char *weapons_raw = malloc(size + 1);
 if (weapons_raw == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
         free(weapons_raw);
        return ;
    }
size_t n = fread(weapons_raw, 1, size, weapon_file);
weapons_raw[n] = '\0';
fclose(weapon_file);
cJSON *root = cJSON_Parse(weapons_raw);
if (root == NULL) {
    fprintf(stderr, "JSON parse error\n");
    free(weapons_raw);
    return;
}

cJSON *player = cJSON_GetObjectItemCaseSensitive(root, "player_weapons");
cJSON *enemy  = cJSON_GetObjectItemCaseSensitive(root, "enemy_weapons");

int i = 0;
if (cJSON_IsArray(player)) ParseWeaponArray(player, weapons, &i, 100);
if (cJSON_IsArray(enemy))  ParseWeaponArray(enemy,  weapons, &i, 100);
weapon_count = i;  

cJSON_Delete(root);  
//Weapon counting is currently bugged
 fprintf(stderr, "\n Succesfully loaded weapons. Weapon count: %d", weapon_count, "     \n");
free(weapons_raw);
}
//Function to find weapons from strings
const WeaponType *find_weapon(const char *name) {
    if (!name) return NULL;
    for (int i = 0; i < weapon_count; i++)
        if (weapons[i].var_name != NULL && strcmp(weapons[i].var_name, name) == 0)
            return &weapons[i];
    return NULL;
}



//Array of structs that stores enemy data
EnemyDefs enemy_defs[MAX_ENEMY_DEFS] = {0};
int enemy_def_count = 0;
//Helper function to get enemy behaviors from the JSON strings
EnemyBehavior StringToEnemyBehavior(const char *s) {
    static const EnumEntry table[] = {
        {"STATIC", STATIC}, {"MOVEHORIZONTALLY", MOVEHORIZONTALLY},
        {"MOVEVERTICALLY", MOVEVERTICALLY}, {"STRAFE_HORIZONTAL", STRAFE_HORIZONTAL},
        {"STRAFE_VERTICAL", STRAFE_VERTICAL}, {"HUNT_PLAYER", HUNT_PLAYER},
        {"ZIGZAG", ZIGZAG}, {"HUNT_PLAYER_FAR", HUNT_PLAYER_FAR},
        {"CARRIER_SPECIAL", CARRIER_SPECIAL}, {"FRIGATE1_SPECIAL", FRIGATE1_SPECIAL},
        {"FRIGATE2_SPECIAL", FRIGATE2_SPECIAL}, {"JET_SPECIAL", JET_SPECIAL},
        {"BATTLESHIP_SPECIAL", BATTLESHIP_SPECIAL}, {"FLYFORT_SPECIAL", FLYFORT_SPECIAL},
        {"MOTHERSHIP_SPECIAL", MOTHERSHIP_SPECIAL}
    };
    return (EnemyBehavior)LookupEnum(table, sizeof table / sizeof table[0], s, STATIC);
}
//Function to parse enemies from the file into an array
void LoadEnemyDefs(void) {
    FILE *f = fopen("Data/enemies.json", "rb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Enemy file not opened!\n");
        return;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *raw = malloc(size + 1);
    if (raw == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return;
    }
    size_t n = fread(raw, 1, size, f);
    raw[n] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(raw);
    free(raw);
    if (root == NULL) {
        fprintf(stderr, "JSON parse error\n");
        return;
    }

    int i = 0;
    cJSON *item;
    cJSON_ArrayForEach(item, root) {          // iterates top-level keys in file order
        if (i >= MAX_ENEMY_DEFS) break;
        EnemyDefs *e = &enemy_defs[i];

        cJSON *dx  = cJSON_GetObjectItemCaseSensitive(item, "dx");
        cJSON *dy  = cJSON_GetObjectItemCaseSensitive(item, "dy");
        cJSON *hp  = cJSON_GetObjectItemCaseSensitive(item, "hp");
        cJSON *sym = cJSON_GetObjectItemCaseSensitive(item, "symbol");
        cJSON *w   = cJSON_GetObjectItemCaseSensitive(item, "width");
        cJSON *h   = cJSON_GetObjectItemCaseSensitive(item, "height");
        cJSON *cd  = cJSON_GetObjectItemCaseSensitive(item, "cooldown_frames");
        cJSON *beh = cJSON_GetObjectItemCaseSensitive(item, "behavior");
        cJSON *wep = cJSON_GetObjectItemCaseSensitive(item, "weapon");
        cJSON *path = cJSON_GetObjectItemCaseSensitive(item, "sprite_path");

        e->type = i;                           // order of appearance, not the EnemyType enum
        if (cJSON_IsNumber(dx))  e->dx = (float)dx->valuedouble;
        if (cJSON_IsNumber(dy))  e->dy = (float)dy->valuedouble;
        if (cJSON_IsNumber(hp))  e->hp = hp->valueint;
        if (cJSON_IsString(sym) && sym->valuestring[0] != '\0') e->symbol = sym->valuestring[0];
        if (cJSON_IsString(sym) && sym->valuestring[0] != '\0') e->symbol = sym->valuestring[0];
        e->sprite_path = cJSON_IsString(path) ? strdup(path->valuestring) : NULL;
        if (cJSON_IsNumber(w))   e->width = w->valueint;
        if (cJSON_IsNumber(h))   e->height = h->valueint;
        if (cJSON_IsNumber(cd))  e->cooldown_frames = cd->valueint;
        e->behavior = cJSON_IsString(beh) ? StringToEnemyBehavior(beh->valuestring) : STATIC;
        e->weapon = cJSON_IsString(wep) ? strdup(wep->valuestring) : NULL;

        i++;
    }
    enemy_def_count = i;

    cJSON_Delete(root);
     fprintf(stderr, "\n Succesfully loaded enemy data. Enemy type count: %d", enemy_def_count, "     \n");
}
PlayerDefs player_defs[MAX_PLAYER_TYPES] = {0};
int player_def_count = 0;

static PlayerClass StringToPlayerClass(const char *s) {
    static const EnumEntry table[] = {
        {"PLAYER_DEBUG", PLAYER_DEBUG}, {"PLAYER_FIGHTER", PLAYER_FIGHTER},
        {"PLAYER_FORTRESS", PLAYER_FORTRESS}, {"PLAYER_EXPERIMENTAL", PLAYER_EXPERIMENTAL}
    };
    return (PlayerClass)LookupEnum(table, sizeof table / sizeof table[0], s, PLAYER_DEBUG);
}
//Loads player data from the json file
void LoadPlayerDefs(void) {
    FILE *f = fopen("Data/players.json", "rb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Player file not opened!\n");
        return;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *raw = malloc(size + 1);
    if (raw == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return;
    }
    size_t n = fread(raw, 1, size, f);
    raw[n] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(raw);
    free(raw);
    if (!cJSON_IsArray(root)) {
        fprintf(stderr, "JSON parse error in players.json\n");
        cJSON_Delete(root);
        return;
    }

    int i = 0;
    cJSON *item;
    cJSON_ArrayForEach(item, root) {
        if (i >= MAX_PLAYER_TYPES) break;
        PlayerDefs *p = &player_defs[i];
        memset(p, 0, sizeof *p);

        cJSON *name  = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *cls   = cJSON_GetObjectItemCaseSensitive(item, "class");
        cJSON *vx    = cJSON_GetObjectItemCaseSensitive(item, "vx");
        cJSON *vy    = cJSON_GetObjectItemCaseSensitive(item, "vy");
        cJSON *lives = cJSON_GetObjectItemCaseSensitive(item, "lives");
        cJSON *las   = cJSON_GetObjectItemCaseSensitive(item, "hasLasers");
        cJSON *bmb   = cJSON_GetObjectItemCaseSensitive(item, "hasBombs");
        cJSON *fr    = cJSON_GetObjectItemCaseSensitive(item, "fire_rate");
        cJSON *sym   = cJSON_GetObjectItemCaseSensitive(item, "symbol");
        cJSON *wep   = cJSON_GetObjectItemCaseSensitive(item, "weapon");
        cJSON *inv   = cJSON_GetObjectItemCaseSensitive(item, "inventory");

        if (cJSON_IsString(name)) strncpy(p->name, name->valuestring, sizeof p->name - 1);
        p->class = cJSON_IsString(cls) ? StringToPlayerClass(cls->valuestring) : PLAYER_DEBUG;
        if (cJSON_IsNumber(vx))    p->vx = (float)vx->valuedouble;
        if (cJSON_IsNumber(vy))    p->vy = (float)vy->valuedouble;
        if (cJSON_IsNumber(lives)) p->lives = lives->valueint;
        p->hasLasers = cJSON_IsTrue(las);
        p->hasBombs  = cJSON_IsTrue(bmb);
        if (cJSON_IsNumber(fr))    p->fire_rate = fr->valueint;
        p->symbol = (cJSON_IsString(sym) && sym->valuestring[0] != '\0') ? sym->valuestring[0] : '@';
        p->weapon = cJSON_IsString(wep) ? strdup(wep->valuestring) : NULL;

        int k = 0;
        cJSON *entry;
        cJSON_ArrayForEach(entry, inv) {
            if (k >= INV_SIZE) break;
            if (cJSON_IsString(entry)) p->inventory[k++] = strdup(entry->valuestring);
        }
        i++;
    }
    player_def_count = i;

    cJSON_Delete(root);
    fprintf(stderr, "\n Successfully loaded player data. Player type count: %d", player_def_count);
}

// Builds a runtime Player from a def. Call after LoadWeapons().
void PlayerFromDef(Player *p, const PlayerDefs *d) {
    memset(p, 0, sizeof *p);
    p->px = PLAYFIELD_W / 2;
    p->py = (PLAYFIELD_H / 3) * 2;
    p->vx = p->dx = d->vx;
    p->vy = p->dy = d->vy;
    p->alive = 1;
    p->lives = d->lives;
    p->hasLasers = d->hasLasers;
    p->hasBombs = d->hasBombs;
    p->fire_rate = d->fire_rate;
    p->symbol = d->symbol;
    p->speed_mode_fast = true;
    p->class = d->class;

    p->weapon_name = d->weapon;
    p->weapon = find_weapon(d->weapon);
    if (p->weapon) p->weapon_id = p->weapon->weapon_id;

    for (int i = 0; i < INV_SIZE; i++) {
        p->inventory_names[i] = d->inventory[i];
        p->inventory_data[i]  = find_weapon(d->inventory[i]);
        p->inventory[i] = p->inventory_data[i] ? p->inventory_data[i]->weapon_id : EMPTY_ID;
    }
}