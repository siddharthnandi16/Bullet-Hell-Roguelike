# Data Files and Loader Reference

Reference for the JSON loader and the data files in `Data/`.

## Loader

### `src/data/json_loader.h` / `.cpp`

A thin wrapper around cJSON. It reads files and reports errors with the file name and key path.

| Name | What it does |
|---|---|
| `DataError` | The exception type for every data problem: missing files, bad JSON, wrong types, unknown keys, and failed range checks. |
| `JsonDocument` | Opens a file, parses it, and frees the tree when it goes out of scope. `root()` returns the top-level node. |
| `JsonNode` | A read-only view of one JSON value. It is valid only while its `JsonDocument` is alive. |
| `with_json_file(path, f)` | Opens a file, passes its root to `f`, and frees the tree after `f` returns. |

`JsonNode` methods:

| Method | What it does |
|---|---|
| `where()` | Returns the file and key path, used in error messages. |
| `fail(msg)` | Throws a `DataError` with that location prefixed. |
| `require_only({...})` | Throws if the object has any key not in the list. This catches typos. |
| `has(key)` | True if the key exists. Use it for optional fields. |
| `child(key)` | Returns the node for a key, throwing if it is missing. |
| `items()` | Returns the elements of an array. |
| `members()` | Returns the key and value pairs of an object. |
| `as_int()`, `as_number()`, `as_string()`, `as_bool()` | Read a value and throw if its type is wrong. `as_int` rejects decimals. |
| `as_raw()` | Returns the node as compact JSON text. Used for scripts, which are parsed later. |
| `get_int(key)` and similar | Shortcut for `child(key).as_int()`, with the same family for number, string, bool, raw, array, and string list. |

### `src/data/content_database.h` / `.cpp`

Turns the JSON files into plain structs, stored in maps keyed by id. It runs once at startup.

`load_content_database(dir)` loads files in dependency order. Each file can only refer to files loaded before it, so references are checked as the data is read. It reads:

1. `tuning.json`
2. `palette.json`
3. `bullet_types.json`, then checks that every bullet's children exist
4. `weapons.json`, `patterns.json`, `enemies.json`, `ships.json`, `upgrades.json`
5. Every `.json` file in `waves/`

Helper functions in the same file:

| Function | What it does |
|---|---|
| `ensure(ok, node, msg)` | Throws a `DataError` with the node's location if `ok` is false. |
| `require_ref(table, node, name, kind)` | Throws if `name` is not a key in `table`. |
| `add_unique(table, def, where)` | Inserts a definition, throwing on a duplicate id. |
| `parse_*` | One per file type. Reads the fields, runs range and reference checks, and returns the struct. |
| `load_table` | Opens a file, reads its array under a given key, and parses each entry. |
| `wave_files` | Lists the `.json` files in `waves/`, sorted by name. |

Scripts are stored as raw JSON text in `script_json` and are not interpreted yet. Shape names, tag names, stat names, and `bomb_type` values are not checked yet either. Those checks come when their registries exist.

## Data Files

All files are in `Data/`, except `waves/`, which is a subfolder.

### `tuning.json`

Global balance values. One object, no array.

| Field | Meaning |
|---|---|
| `continues` | Continues per run. |
| `bombs_start` | Bombs at the start of a run. Must not exceed `bombs_max`. |
| `bombs_max` | Bomb cap. |
| `lives_penalty` | Multiplier reduction per life lost. |
| `multiplier_min`, `multiplier_max` | Clamp range for the payout multiplier. |
| `shop_size` | Number of upgrades offered per shop. |
| `max_bullets` | Size of the bullet pool. |
| `speed_floor_ratio` | Multiple of a wave's `par_seconds` where the speed factor reaches its floor. Must be above 1.0. |

### `palette.json`

Maps color role names to colors. Each value is `[r, g, b]` or `[r, g, b, a]`, with channels from 0 to 255. Other files refer to colors by role name, so you can recolor the game by editing this file.

### `ships.json`

The `"ships"` array. Each ship is one playable ship.

| Field | Meaning |
|---|---|
| `id` | Unique name, used by other files. |
| `width`, `height` | Hitbox size in grid cells. |
| `speed` | Movement speed. Must be positive. |
| `starting_bombs` | Bombs the ship starts with. |
| `bomb_type` | Name of the bomb effect. Not checked yet. |
| `special_rules` | Name of the ship's special behavior. Not checked yet. |
| `starting_weapon` | Weapon id from `weapons.json`. |

### `enemies.json`

The `"enemies"` array. These are reusable enemy bodies. Their behavior scripts live in wave files.

| Field | Meaning |
|---|---|
| `id` | Unique name. |
| `width`, `height` | Body size in cells. |
| `max_health` | Hit points. Must be positive. |
| `weapons` | Weapon ids this enemy fires. |
| `glyph` | One string per row, each exactly `width` characters, with `height` rows. |
| `color` | Palette role name. |
| `contact_damage` | Damage dealt on touching the player. |
| `bomb_immune` | If true, bombs do not damage it. |
| `required_to_clear` | If true, the wave cannot end until this enemy is dead. |
| `tags` | Special behavior names. Not checked yet. |

### `bullet_types.json`

The `"bullets"` array. Bullet templates, which any entity can fire.

| Field | Meaning |
|---|---|
| `id` | Unique name. |
| `width`, `height`, `glyph`, `color` | Same rules as enemies. |
| `base_speed` | Cells per frame. Must be positive. |
| `damage` | Damage per hit. |
| `tags` | Special behavior names. |
| `children` | Bullet ids this bullet can spawn. Each must exist. |
| `script` | An object with a `rules` list, run each frame. Stored as raw JSON for now. |

### `weapons.json`

The `"weapons"` array. Each weapon fires one bullet type.

| Field | Meaning |
|---|---|
| `id`, `display_name` | Internal name and shop/HUD label. |
| `bullet_type` | Bullet id from `bullet_types.json`. |
| `fire_interval_frames` | Frames between shots. Smaller is faster. |
| `starting_level` | Level the weapon starts at, counting from 1. |
| `tags` | Special behavior names. |
| `levels` | List of `{bullet_count, angle_deg}`. The first entry is level 1. `angle_deg` is the total spread across all bullets. |

### `patterns.json`

The `"patterns"` array. A pattern is a timed set of bullets, built from one or more layers, where each layer uses a shape.

| Field | Meaning |
|---|---|
| `id` | Unique name. |
| `origin` | `{offset_x, offset_y}`, the start point relative to the firer, in cells. |
| `aim` | `fixed`, `at_player`, or `facing`. |
| `aim_angle_deg` | Direction used when `aim` is `fixed`. |
| `duration_frames` | Total run time. Must be positive. |
| `emission_interval_frames` | Frames between emissions. Must be positive and no longer than the duration. |
| `repeat` | How many times the pattern runs. At least 1. |
| `tags` | Special behavior names. |
| `layers` | Each layer has `shape`, `start_delay_frames`, `bullet_type`, `overrides` (numeric shape parameters), and optional `origin` and `aim` that override the pattern's values. |

Shape names are not checked until the shape registry exists.

### `upgrades.json`

The `"upgrades"` array. Items sold in the shop.

| Field | Meaning |
|---|---|
| `id`, `display_name`, `description` | Internal name, shop label, and shop text. |
| `kind` | `stat`, `special`, `weapon`, or `life`. |
| `price` | Cost in currency. |
| `max_purchases` | Times it can be bought per run. Defaults to 1. |
| `cooldown_frames` | Cooldown for special upgrades. Only allowed when `kind` is `special`, and required then. |
| `effects` | List of effects, each with a `type`. |
| `rules` | Special behavior names. |

Effect types:

| Type | Fields |
|---|---|
| `stat_change` | `stat`, `amount` |
| `grant_special` | `special` |
| `grant_weapon` | `weapon`, which must exist in `weapons.json` |
| `raise_main_weapon_level` | `amount` |
| `grant_life` | `amount` |

### `waves/*.json`

One file per wave. Each file is a single object.

| Field | Meaning |
|---|---|
| `id` | Unique name. |
| `difficulty` | Integer from 1 to 5, independent of the stage's difficulty. |
| `base_payout` | Currency before the multiplier. |
| `par_seconds` | Clear time that earns full speed factor. |
| `timeout_frames` | The wave ends after this many frames even if enemies remain. |
| `tags` | Special behavior names. |
| `spawns` | List of enemy spawns. |

Each spawn has:

| Field | Meaning |
|---|---|
| `enemy` | Enemy id from `enemies.json`. |
| `start_frame` | Frame at which it appears. Must be below `timeout_frames`. |
| `x`, `y` | Starting position in cells. |
| `script` | The enemy's behavior: `end` (`stop` or `restart`) and `steps`. Stored as raw JSON for now, and its format is not finalized. |
