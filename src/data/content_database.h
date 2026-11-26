#pragma once
#include <optional>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct TuningDef {
    int continues = 0;
    int bombs_start = 0;
    int bombs_max = 0;
    double lives_penalty = 0.0;
    double multiplier_min = 0.0;
    double multiplier_max = 0.0;
    int shop_size = 0;
    int max_bullets = 0;
    double speed_floor_ratio = 0.0;
};

struct Rgba {
    std::uint8_t r = 255, g = 255, b = 255, a = 255;
};

struct ShipDef {
    std::string id;
    int width = 0;
    int height = 0;
    double speed = 0.0;
    int starting_bombs = 0;
    std::string bomb_type;
    std::string special_rules;
    std::string starting_weapon;
};

struct EnemyDef {
    std::string id;
    int width = 0;
    int height = 0;
    int max_health = 0;
    std::vector<std::string> weapons;
    std::vector<std::string> glyph;
    std::string color;
    int contact_damage = 0;
    bool bomb_immune = false;
    bool required_to_clear = false;
    std::vector<std::string> tags;
};

struct SpawnDef {
    std::string enemy;
    int start_frame = 0;
    int x = 0;
    int y = 0;
    std::string script_json;  // parsed later by the behavior loader
};

struct WaveDef {
    std::string id;
    int difficulty = 0;
    int base_payout = 0;
    double par_seconds = 0.0;
    int timeout_frames = 0;
    std::vector<std::string> tags;
    std::vector<SpawnDef> spawns;
};

struct BulletTypeDef {
    std::string id;
    int width = 0;
    int height = 0;
    double base_speed = 0.0;
    int damage = 0;
    std::vector<std::string> glyph;
    std::string color;
    std::vector<std::string> tags;
    std::vector<std::string> children;
    std::string script_json;  // parsed later by the behavior loader
};

struct WeaponLevel {
    int bullet_count = 0;
    double angle_deg = 0.0;
};

struct WeaponDef {
    std::string id;
    std::string display_name;
    std::string bullet_type;
    int fire_interval_frames = 0;
    int starting_level = 1;
    std::vector<std::string> tags;
    std::vector<WeaponLevel> levels;
};

struct Offset {
    int x = 0;
    int y = 0;
};

struct PatternLayer {
    std::string shape;
    int start_delay_frames = 0;
    std::string bullet_type;
    std::string aim;  // empty means inherit the pattern's aim
    Offset origin;
    std::map<std::string, double> overrides;
};

struct PatternDef {
    std::string id;
    Offset origin;
    std::string aim;
    double aim_angle_deg = 0.0;
    int duration_frames = 0;
    int emission_interval_frames = 0;
    int repeat = 1;
    std::vector<std::string> tags;
    std::vector<PatternLayer> layers;
};

enum class UpgradeKind { Stat, Special, Weapon, Life };
enum class EffectType { StatChange, GrantSpecial, GrantWeapon, RaiseMainWeaponLevel, GrantLife };

struct UpgradeEffect {
    EffectType type;
    std::string target;  // stat, special, or weapon name, empty if unused
    double amount = 0.0;
};

struct UpgradeDef {
    std::string id;
    std::string display_name;
    std::string description;
    UpgradeKind kind = UpgradeKind::Stat;
    int price = 0;
    int max_purchases = 1;
    std::optional<int> cooldown_frames;  // specials only
    std::vector<UpgradeEffect> effects;
    std::vector<std::string> rules;
};

// All loaded content. Maps are keyed by id and built once at startup.
struct ContentDatabase {
    TuningDef tuning;
    std::map<std::string, Rgba> palette;
    std::map<std::string, BulletTypeDef> bullet_types;
    std::map<std::string, WeaponDef> weapons;
    std::map<std::string, PatternDef> patterns;
    std::map<std::string, EnemyDef> enemies;
    std::map<std::string, ShipDef> ships;
    std::map<std::string, UpgradeDef> upgrades;
    std::map<std::string, WaveDef> waves;
};

// Loads every file under dir (for example "Data"). Throws DataError on the first problem.
ContentDatabase load_content_database(const std::string& dir);