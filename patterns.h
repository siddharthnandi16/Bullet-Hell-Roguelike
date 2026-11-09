#ifndef PATTERN_H
#define PATTERN_H
#define MAX_PATTERNS 100

//One burst of bullets
typedef struct {
    int   time;        /* frame, relative to pattern start */
    float angle;       /* degrees, 0 = right */
    float speed;
    int   count;       /* bullets fired in a ring/fan */
    float spread;      /* total fan width in degrees; 360 = full ring */
    const char* type; //Type of bullet to be spawned
    int   special;  /* special behaviour ID, 0 = none */
} BulletStep;

//One pattern defintion consisting of a given number of bullet steps
typedef struct {
    BulletStep *steps; /* must be sorted by time */
    int         step_count;
    int         duration_frames;
} BulletPattern;

//One instance of given pattern
typedef struct {
    const BulletPattern *pattern;
    int   frame;
    int   next_step;   /* index of the next step not yet fired */
    float origin_x, origin_y;
    int   active;
} PatternInstance;
extern PatternInstance instances[MAX_PATTERNS];
int start_pattern(const BulletPattern *pattern, float origin_x, float origin_y);
static void fire_step(const BulletStep *s, float origin_x, float origin_y);
int update_pattern_instance(PatternInstance *inst);
#endif