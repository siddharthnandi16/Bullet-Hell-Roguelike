#include "patterns.h"
#include "gamedata.h"
#include "projectile.h"
#include "parser.h"
#include <math.h>
#include <string.h>
PatternInstance instances[MAX_PATTERNS] = {0};
//Function to spawn a single bullet of a given type
void spawn_bullet(float x, float y, float vx, float vy,
                  const char *name, int special)
{
    int bi = find_bullet_index(name);
    if (bi < 0) return;  /* Unknown bullet name */

    int slot = findfreeprojectileslot();
    if (slot < 0) return;  /* Pool full */

    const BulletType *bt = &bullets[bi];
    Projectile *p = &projectiles[slot];

    p->state        = NORMAL;   /* move_projectiles only updates NORMAL */
    p->player_owned = FALSE;
    p->color        = 3;
    p->px           = x;
    p->py           = y;
    p->old_px       = x;
    p->old_py       = y;
    p->angle        = 90;       /* 90 makes move_projectiles use dx/dy directly */
    p->dx           = vx;
    p->dy           = vy;
    p->age          = 0;
    p->width        = bt->width;
    p->height       = bt->height;
    p->symbol       = bt->symbol;
    p->pierce       = bt->pierce;
    p->strafe       = bt->strafe;
    p->turn_rate    = bt->turn_rate;
    p->damage       = bt->damage;
    p->special      = special;

    strncpy(p->name, bt->name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';
}
//Function to create an instance of a given pattern
int start_pattern(const BulletPattern *pattern, float origin_x, float origin_y)
{
    for (int i = 0; i < MAX_PATTERNS; i++) {
        if (!instances[i].active) {
            instances[i].pattern   = pattern;
            instances[i].frame     = 0;
            instances[i].next_step = 0;
            instances[i].origin_x  = origin_x;
            instances[i].origin_y  = origin_y;
            instances[i].active    = 1;
            return i;
        }
    }
    return -1;  /* No free instance slots */
}
//Function to fire one "step" of a pattern
static void fire_step(const BulletStep *s, float origin_x, float origin_y)
{
    for (int b = 0; b < s->count; b++) {
        float t   = (s->count > 1) ? (float)b / (s->count - 1) : 0.5f;
        float deg = s->angle + s->spread * (t - 0.5f);
        float rad = deg * 3.14159265f / 180.0f;
        spawn_bullet(origin_x, origin_y,
                     cosf(rad) * s->speed,
                     sinf(rad) * s->speed,
                     s->type,
                     s->special);
    }
}
//Function which updates patterns and ends them when their duration runs out
/* Call once per frame per instance. Returns 0 when the instance has finished. */
int update_pattern_instance(PatternInstance *inst)
{
    const BulletPattern *p = inst->pattern;

    if (inst->frame >= p->duration_frames) {
        inst->active = 0;
        return 0;
    }

    while (inst->next_step < p->step_count &&
           p->steps[inst->next_step].time <= inst->frame) {
        fire_step(&p->steps[inst->next_step], inst->origin_x, inst->origin_y);
        inst->next_step++;
    }

    inst->frame++;
    return 1;
}