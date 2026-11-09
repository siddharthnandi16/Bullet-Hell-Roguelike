/* Generated from Data/bullet_types.json. Do not edit by hand. */
#ifndef BULLET_TYPES_H
#define BULLET_TYPES_H

typedef enum {
    BULLET,
    LASER,
    BOMB,
    MISSILE,
    PLASMA,
    EMP,
    CHAINLIGHTNING,
    BULLET_TYPE_COUNT
} ProjType;

static const char *bullet_type_names[BULLET_TYPE_COUNT] = {
    "BULLET",
    "LASER",
    "BOMB",
    "MISSILE",
    "PLASMA",
    "EMP",
    "CHAINLIGHTNING",
};

#endif /* BULLET_TYPES_H */
