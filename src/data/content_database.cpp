#include "data/content_database.h"

#include "data/json_loader.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;

namespace {

void ensure(bool ok, const JsonNode& at, const std::string& message) {
    if (!ok) throw DataError(at.where() + ": " + message);
}

template <typename T>
void require_ref(const std::map<std::string, T>& table, const JsonNode& at,
                 const std::string& name, const char* kind) {
    if (!table.count(name))
        throw DataError(at.where() + ": unknown " + kind + " '" + name + "'");
}

template <typename T>
void add_unique(std::map<std::string, T>& table, T def, const std::string& where) {
    if (table.count(def.id))
        throw DataError(where + ": duplicate id '" + def.id + "'");
    std::string key = def.id;
    table.emplace(std::move(key), std::move(def));
}

std::vector<std::string> optional_strings(const JsonNode& n, const char* key) {
    if (!n.has(key)) return {};
    return n.get_strings(key);
}

void check_aim(const std::string& aim, const JsonNode& at) {
    ensure(aim == "fixed" || aim == "at_player" || aim == "facing", at,
           "aim must be fixed, at_player, or facing");
}

Offset parse_offset(const JsonNode& n) {
    n.require_only({"offset_x", "offset_y"});
    return Offset{n.get_int("offset_x"), n.get_int("offset_y")};
}

std::vector<std::string> parse_glyph(const JsonNode& g, int width, int height) {
    std::vector<std::string> rows;
    for (const JsonNode& r : g.items()) rows.push_back(r.as_string());
    ensure(static_cast<int>(rows.size()) == height, g, "glyph needs one row per height");
    for (const std::string& row : rows)
        ensure(static_cast<int>(row.size()) == width, g,
               "every glyph row must be exactly width characters");
    return rows;
}

TuningDef parse_tuning(const JsonNode& n) {
    n.require_only({"continues", "bombs_start", "bombs_max", "lives_penalty",
                    "multiplier_min", "multiplier_max", "shop_size", "max_bullets", "speed_floor_ratio"});
    TuningDef t;
    t.continues = n.get_int("continues");
    t.bombs_start = n.get_int("bombs_start");
    t.bombs_max = n.get_int("bombs_max");
    t.lives_penalty = n.get_number("lives_penalty");
    t.multiplier_min = n.get_number("multiplier_min");
    t.multiplier_max = n.get_number("multiplier_max");
    t.shop_size = n.get_int("shop_size");
    t.max_bullets = n.get_int("max_bullets");
        t.speed_floor_ratio = n.get_number("speed_floor_ratio");

    ensure(t.continues >= 0, n.child("continues"), "must be 0 or more");
    ensure(t.bombs_start >= 0 && t.bombs_start <= t.bombs_max, n,
           "bombs_start must be between 0 and bombs_max");
    ensure(t.lives_penalty >= 0.0, n.child("lives_penalty"), "must be 0 or more");
    ensure(t.multiplier_min > 0.0 && t.multiplier_min <= t.multiplier_max, n,
           "multiplier_min must be positive and no greater than multiplier_max");
    ensure(t.shop_size >= 1, n.child("shop_size"), "must be at least 1");
    ensure(t.max_bullets >= 1, n.child("max_bullets"), "must be at least 1");
    ensure(t.speed_floor_ratio > 1.0, n.child("speed_floor_ratio"),
           "must be greater than 1.0, so the floor comes after par");
    return t;
}

std::map<std::string, Rgba> parse_palette(const JsonNode& n) {
    std::map<std::string, Rgba> out;
    for (const auto& [role, value] : n.members()) {
        const std::vector<JsonNode> parts = value.items();
        ensure(parts.size() == 3 || parts.size() == 4, value,
               "color must be [r, g, b] or [r, g, b, a]");
        std::uint8_t channels[4] = {0, 0, 0, 255};
        for (std::size_t i = 0; i < parts.size(); ++i) {
            const int v = parts[i].as_int();
            ensure(v >= 0 && v <= 255, parts[i], "channel must be 0 to 255");
            channels[i] = static_cast<std::uint8_t>(v);
        }
        out[role] = Rgba{channels[0], channels[1], channels[2], channels[3]};
    }
    return out;
}

BulletTypeDef parse_bullet_type(const JsonNode& n, const ContentDatabase& db) {
    n.require_only({"id", "width", "height", "base_speed", "damage",
                    "glyph", "color", "tags", "children", "script"});
    BulletTypeDef b;
    b.id = n.get_string("id");
    b.width = n.get_int("width");
    b.height = n.get_int("height");
    ensure(b.width >= 1 && b.height >= 1, n, "width and height must be at least 1");
    b.base_speed = n.get_number("base_speed");
    ensure(b.base_speed > 0.0, n.child("base_speed"), "must be positive");
    b.damage = n.get_int("damage");
    ensure(b.damage >= 0, n.child("damage"), "must be 0 or more");
    b.glyph = parse_glyph(n.child("glyph"), b.width, b.height);
    b.color = n.get_string("color");
    require_ref(db.palette, n, b.color, "palette color");
    b.tags = optional_strings(n, "tags");
    b.children = optional_strings(n, "children");
    b.script_json = n.get_raw("script");
    return b;
}

WeaponDef parse_weapon(const JsonNode& n, const ContentDatabase& db) {
    n.require_only({"id", "display_name", "bullet_type", "fire_interval_frames",
                    "starting_level", "tags", "levels"});
    WeaponDef w;
    w.id = n.get_string("id");
    w.display_name = n.get_string("display_name");
    w.bullet_type = n.get_string("bullet_type");
    require_ref(db.bullet_types, n, w.bullet_type, "bullet type");
    w.fire_interval_frames = n.get_int("fire_interval_frames");
    ensure(w.fire_interval_frames > 0, n.child("fire_interval_frames"), "must be positive");
    w.starting_level = n.get_int("starting_level");
    w.tags = optional_strings(n, "tags");

    for (const JsonNode& l : n.get_array("levels")) {
        l.require_only({"bullet_count", "angle_deg"});
        WeaponLevel lv;
        lv.bullet_count = l.get_int("bullet_count");
        lv.angle_deg = l.get_number("angle_deg");
        ensure(lv.bullet_count >= 1, l.child("bullet_count"), "must be at least 1");
        w.levels.push_back(lv);
    }
    ensure(!w.levels.empty(), n.child("levels"), "needs at least one level");
    ensure(w.starting_level >= 1 && static_cast<std::size_t>(w.starting_level) <= w.levels.size(),
           n.child("starting_level"), "must be a valid level index, counting from 1");
    return w;
}

PatternDef parse_pattern(const JsonNode& n, const ContentDatabase& db) {
    n.require_only({"id", "origin", "aim", "aim_angle_deg", "duration_frames",
                    "emission_interval_frames", "repeat", "tags", "layers"});
    PatternDef p;
    p.id = n.get_string("id");
    p.origin = parse_offset(n.child("origin"));
    p.aim = n.get_string("aim");
    check_aim(p.aim, n.child("aim"));
    p.aim_angle_deg = n.get_number("aim_angle_deg");
    p.duration_frames = n.get_int("duration_frames");
    ensure(p.duration_frames > 0, n.child("duration_frames"), "must be positive");
    p.emission_interval_frames = n.get_int("emission_interval_frames");
    ensure(p.emission_interval_frames > 0 && p.emission_interval_frames <= p.duration_frames,
           n.child("emission_interval_frames"), "must be positive and no longer than duration");
    p.repeat = n.get_int("repeat");
    ensure(p.repeat >= 1, n.child("repeat"), "must be at least 1");
    p.tags = optional_strings(n, "tags");

    for (const JsonNode& l : n.get_array("layers")) {
        l.require_only({"shape", "start_delay_frames", "bullet_type", "overrides", "origin", "aim"});
        PatternLayer layer;
        layer.shape = l.get_string("shape");  // shape names are checked once the shape registry exists
        layer.start_delay_frames = l.get_int("start_delay_frames");
        ensure(layer.start_delay_frames >= 0, l.child("start_delay_frames"), "must be 0 or more");
        layer.bullet_type = l.get_string("bullet_type");
        require_ref(db.bullet_types, l, layer.bullet_type, "bullet type");
        layer.origin = l.has("origin") ? parse_offset(l.child("origin")) : Offset{};
        if (l.has("aim")) {
            layer.aim = l.get_string("aim");
            check_aim(layer.aim, l.child("aim"));
        }
        if (l.has("overrides"))
            for (const auto& [name, value] : l.child("overrides").members())
                layer.overrides[name] = value.as_number();
        p.layers.push_back(std::move(layer));
    }
    ensure(!p.layers.empty(), n.child("layers"), "needs at least one layer");
    return p;
}

EnemyDef parse_enemy(const JsonNode& n, const ContentDatabase& db) {
    n.require_only({"id", "width", "height", "max_health", "weapons", "glyph",
                    "color", "contact_damage", "bomb_immune", "required_to_clear", "tags"});
    EnemyDef e;
    e.id = n.get_string("id");
    e.width = n.get_int("width");
    e.height = n.get_int("height");
    ensure(e.width >= 1 && e.height >= 1, n, "width and height must be at least 1");
    e.max_health = n.get_int("max_health");
    ensure(e.max_health > 0, n.child("max_health"), "must be positive");
    e.weapons = n.get_strings("weapons");
    for (const std::string& w : e.weapons) require_ref(db.weapons, n, w, "weapon");
    e.glyph = parse_glyph(n.child("glyph"), e.width, e.height);
    e.color = n.get_string("color");
    require_ref(db.palette, n, e.color, "palette color");
    e.contact_damage = n.get_int("contact_damage");
    ensure(e.contact_damage >= 0, n.child("contact_damage"), "must be 0 or more");
    e.bomb_immune = n.get_bool("bomb_immune");
    e.required_to_clear = n.get_bool("required_to_clear");
    e.tags = optional_strings(n, "tags");
    return e;
}

ShipDef parse_ship(const JsonNode& n, const ContentDatabase& db) {
    n.require_only({"id", "width", "height", "speed", "starting_bombs",
                    "bomb_type", "special_rules", "starting_weapon"});
    ShipDef s;
    s.id = n.get_string("id");
    s.width = n.get_int("width");
    s.height = n.get_int("height");
    ensure(s.width >= 1 && s.height >= 1, n, "width and height must be at least 1");
    s.speed = n.get_number("speed");
    ensure(s.speed > 0.0, n.child("speed"), "must be positive");
    s.starting_bombs = n.get_int("starting_bombs");
    ensure(s.starting_bombs >= 0 && s.starting_bombs <= db.tuning.bombs_max,
           n.child("starting_bombs"), "must be between 0 and bombs_max in tuning");
    s.bomb_type = n.get_string("bomb_type");
    s.special_rules = n.get_string("special_rules");
    s.starting_weapon = n.get_string("starting_weapon");
    require_ref(db.weapons, n, s.starting_weapon, "weapon");
    return s;
}

EffectType effect_type_from(const std::string& name) {
    if (name == "stat_change") return EffectType::StatChange;
    if (name == "grant_special") return EffectType::GrantSpecial;
    if (name == "grant_weapon") return EffectType::GrantWeapon;
    if (name == "raise_main_weapon_level") return EffectType::RaiseMainWeaponLevel;
    if (name == "grant_life") return EffectType::GrantLife;
    throw DataError("unknown effect type '" + name + "'");
}

UpgradeEffect parse_effect(const JsonNode& e, const ContentDatabase& db) {
    const std::string type = e.get_string("type");
    switch (effect_type_from(type)) {
    case EffectType::StatChange:
        e.require_only({"type", "stat", "amount"});
        return {EffectType::StatChange, e.get_string("stat"), e.get_number("amount")};
    case EffectType::GrantSpecial:
        e.require_only({"type", "special"});
        return {EffectType::GrantSpecial, e.get_string("special"), 0.0};
    case EffectType::GrantWeapon: {
        e.require_only({"type", "weapon"});
        const std::string weapon = e.get_string("weapon");
        require_ref(db.weapons, e, weapon, "weapon");
        return {EffectType::GrantWeapon, weapon, 0.0};
    }
    case EffectType::RaiseMainWeaponLevel:
        e.require_only({"type", "amount"});
        return {EffectType::RaiseMainWeaponLevel, "", static_cast<double>(e.get_int("amount"))};
    case EffectType::GrantLife:
        e.require_only({"type", "amount"});
        return {EffectType::GrantLife, "", static_cast<double>(e.get_int("amount"))};
    }
    e.fail("unreachable effect type");
}

UpgradeKind parse_kind(const JsonNode& n) {
    const std::string k = n.get_string("kind");
    if (k == "stat") return UpgradeKind::Stat;
    if (k == "special") return UpgradeKind::Special;
    if (k == "weapon") return UpgradeKind::Weapon;
    if (k == "life") return UpgradeKind::Life;
    n.child("kind").fail("kind must be stat, special, weapon, or life");
}

UpgradeDef parse_upgrade(const JsonNode& n, const ContentDatabase& db) {
    n.require_only({"id", "display_name", "description", "kind", "price",
                    "max_purchases", "cooldown_frames", "effects", "rules"});
    UpgradeDef u;
    u.id = n.get_string("id");
    u.display_name = n.get_string("display_name");
    u.description = n.get_string("description");
    u.kind = parse_kind(n);
    u.price = n.get_int("price");
    ensure(u.price >= 0, n.child("price"), "must be 0 or more");
    u.max_purchases = n.has("max_purchases") ? n.get_int("max_purchases") : 1;
    ensure(u.max_purchases >= 1, n.child("max_purchases"), "must be at least 1");

    if (n.has("cooldown_frames")) {
        ensure(u.kind == UpgradeKind::Special, n.child("cooldown_frames"),
               "cooldown_frames is only allowed on special upgrades");
        u.cooldown_frames = n.get_int("cooldown_frames");
        ensure(*u.cooldown_frames > 0, n.child("cooldown_frames"), "must be positive");
    }
    ensure(u.kind != UpgradeKind::Special || u.cooldown_frames.has_value(), n,
           "special upgrades need cooldown_frames");

    for (const JsonNode& e : n.get_array("effects")) u.effects.push_back(parse_effect(e, db));
    ensure(!u.effects.empty(), n.child("effects"), "needs at least one effect");
    u.rules = optional_strings(n, "rules");
    return u;
}

WaveDef parse_wave(const JsonNode& n, const ContentDatabase& db) {
    n.require_only({"id", "difficulty", "base_payout", "par_seconds",
                    "timeout_frames", "tags", "spawns"});
    WaveDef w;
    w.id = n.get_string("id");
    w.difficulty = n.get_int("difficulty");
    ensure(w.difficulty >= 1 && w.difficulty <= 5, n.child("difficulty"), "must be 1 to 5");
    w.base_payout = n.get_int("base_payout");
    ensure(w.base_payout >= 0, n.child("base_payout"), "must be 0 or more");
    w.par_seconds = n.get_number("par_seconds");
    ensure(w.par_seconds > 0.0, n.child("par_seconds"), "must be positive");
    w.timeout_frames = n.get_int("timeout_frames");
    ensure(w.timeout_frames > 0, n.child("timeout_frames"), "must be positive");
    w.tags = optional_strings(n, "tags");

    for (const JsonNode& s : n.get_array("spawns")) {
        s.require_only({"enemy", "start_frame", "x", "y", "script"});
        SpawnDef sp;
        sp.enemy = s.get_string("enemy");
        require_ref(db.enemies, s, sp.enemy, "enemy");
        sp.start_frame = s.get_int("start_frame");
        ensure(sp.start_frame >= 0 && sp.start_frame < w.timeout_frames,
               s.child("start_frame"), "must be at least 0 and below timeout_frames");
        sp.x = s.get_int("x");
        sp.y = s.get_int("y");
        sp.script_json = s.get_raw("script");
        w.spawns.push_back(std::move(sp));
    }
    return w;
}

void check_bullet_children(const ContentDatabase& db) {
    for (const auto& [id, bullet] : db.bullet_types)
        for (const std::string& child : bullet.children)
            if (!db.bullet_types.count(child))
                throw DataError("bullet_types.json: bullet '" + id +
                                "' has unknown child '" + child + "'");
}

template <typename T, typename Parse>
void load_table(const std::string& path, const char* key,
                std::map<std::string, T>& table, Parse parse) {
    with_json_file(path, [&](const JsonNode& root) {
        for (const JsonNode& entry : root.get_array(key))
            add_unique(table, parse(entry), entry.where());
    });
}

std::vector<fs::path> wave_files(const std::string& dir) {
    const fs::path wave_dir = fs::path(dir) / "waves";
    if (!fs::is_directory(wave_dir))
        throw DataError(wave_dir.string() + ": missing waves folder");

    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(wave_dir))
        if (entry.path().extension() == ".json") files.push_back(entry.path());
    std::sort(files.begin(), files.end());
    return files;
}

}  // namespace

ContentDatabase load_content_database(const std::string& dir) {
    ContentDatabase db;

    // Load order follows references: each file only refers to files loaded before it.
    with_json_file(dir + "/tuning.json", [&](const JsonNode& n) { db.tuning = parse_tuning(n); });
    with_json_file(dir + "/palette.json", [&](const JsonNode& n) { db.palette = parse_palette(n); });

    load_table(dir + "/bullet_types.json", "bullets", db.bullet_types,
               [&](const JsonNode& n) { return parse_bullet_type(n, db); });
    check_bullet_children(db);

    load_table(dir + "/weapons.json", "weapons", db.weapons,
               [&](const JsonNode& n) { return parse_weapon(n, db); });
    load_table(dir + "/patterns.json", "patterns", db.patterns,
               [&](const JsonNode& n) { return parse_pattern(n, db); });
    load_table(dir + "/enemies.json", "enemies", db.enemies,
               [&](const JsonNode& n) { return parse_enemy(n, db); });
    load_table(dir + "/ships.json", "ships", db.ships,
               [&](const JsonNode& n) { return parse_ship(n, db); });
    load_table(dir + "/upgrades.json", "upgrades", db.upgrades,
               [&](const JsonNode& n) { return parse_upgrade(n, db); });

    for (const fs::path& file : wave_files(dir)) {
        with_json_file(file.string(), [&](const JsonNode& n) {
            add_unique(db.waves, parse_wave(n, db), n.where());
        });
    }

    return db;
}